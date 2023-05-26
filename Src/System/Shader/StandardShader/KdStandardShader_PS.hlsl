#include "../inc_KdCommon.hlsli"
#include "inc_KdStandardShader.hlsli"

// テクスチャ
Texture2D g_baseTex : register(t0); // ベースカラーテクスチャ
Texture2D g_emissiveTex : register(t1); // エミッシブテクスチャ
Texture2D g_mrTex : register(t2); // メタリック/ラフネステクスチャ

Texture2D g_sceenTex : register(t3); //ゲーム画面のテクスチャ

Texture2D g_dissolveTex : register(t4); //ゲーム画面のテクスチャ

Texture2D g_ditherTex : register(t5);


// サンプラ
SamplerState g_ss : register(s0);

/*
// UnityのBuild-In Shaderでの手法
// RoughnessからSpecularPowerを算出
float RoughnessToSpecPower(float roughness)
{
    float trueRoughness = roughness * roughness; // 学術的に正しいラフネスを求める
    float sq = max(0.0001, trueRoughness * trueRoughness);
    float n = (2.0 / sq) - 2.0;
    // Roughnessが1.0でNdotHがゼロの場合に発生する可能性のあるpow(0,0)のケースを防止
    n = max(n, 0.0001);
    return n;
}
*/

// BlinnPhong NDF
// ・lightDir    … ライトの方向
// ・vCam        … ピクセルからカメラへの方向
// ・normal      … 法線
// ・specPower   … 反射の鋭さ
float BlinnPhong(float3 lightDir, float3 vCam, float3 normal, float specPower)
{
    float3 H = normalize(-lightDir + vCam);
    float NdotH = saturate(dot(normal, H)); // カメラの角度差(0～1)
    float spec = pow(NdotH, specPower);

    // 正規化Blinn-Phong
    return spec * ((specPower + 2) / (2 * 3.1415926535));
}

//================================
// ピクセルシェーダ
//================================
float4 main(VSOutput In) : SV_Target0
{
    // カメラへの方向
    float3 vCam = g_CamPos - In.wPos;
    float camDist = length(vCam); // カメラ - ピクセル距離
    vCam = normalize(vCam);

    // 法線正規化
    float3 wN = normalize(In.wN);

    if (g_OpticalCamoEnable)
    {
        //テクスチャのサイズ取得
        float w = 0;
        float h = 0;
        g_sceenTex.GetDimensions(w, h); //1280x720
        
        //法線からずれを算出
        float2 offset = wN.xy / 50;
        
        //キャラの表示位置のテクスチャ座標をUV座標に交換
        float2 uv = float2(In.Pos.x / w, In.Pos.y / h);
        
        //テクスチャから色を取得
        float4 col = g_sceenTex.Sample(g_ss, uv + offset);
        
        //少しわかりやすい色を入れる
        col.r *= 2.15f;
        col.g *= 2.0f;
        col.b *= 2.15f;
        
        return col;

    }

    //------------------------------------------
    // 材質色
    //------------------------------------------

    float4 mr = g_mrTex.Sample(g_ss, In.UV);
    // 金属性
    float metallic = mr.b * g_Material.Metallic;
    // 粗さ
    float roughness = mr.g * g_Material.Roughness;


    // 材質の色
    float4 baseColor = g_baseTex.Sample(g_ss, In.UV) * g_Material.BaseColor * In.Color;

    // アルファテスト
    if (baseColor.a <= g_AlphaTestThreshold)
        discard;

     //------------------------------------------
    // dissolveディゾルブ
   //------------------------------------------
    if (g_dissolveEnable)
    {
        
        float dissolveColor = g_dissolveTex.Sample(g_ss, In.UV);
        if (dissolveColor.r < g_dissolvePow)
        {
        //ピクセル結果を出力しない（破棄）
        //戻り値を戻さないリターンreturn
            discard;
        }
        
        //境目強調
        if (dissolveColor.r < g_dissolvePow + 0.02f)
        {
            return float4(1, 0, 0, 1);
        }
        if (dissolveColor.r < g_dissolvePow + 0.04f)
        {
            return float4(1, 0, 0.6, 1);
        }
        if (dissolveColor.r < g_dissolvePow + 0.06f)
        {
            return float4(0.9, 0.1, 0.9, 1);
        }
        if (dissolveColor.r < g_dissolvePow + 0.08f)
        {
            return float4(0.9, 0.1, 0.9, 1);
        }
        if (dissolveColor.r < g_dissolvePow + 0.1f)
        {
            return float4(0.6, 0, 1, 1);
        }
        if (dissolveColor.r < g_dissolvePow + 0.12f)
        {
            return float4(0, 0, 1, 1);
        }
    }
    
    //------------------------------------------
    //
    // ライティング
    //
    //------------------------------------------
    // 最終的な色
    float3 color = 0;

    // ライト有効時
    if (g_LightEnable)
    {
		// 材質の拡散色　非金属ほど材質の色になり、金属ほど拡散色は無くなる
        const float3 baseDiffuse = lerp(baseColor.rgb, float3(0, 0, 0), metallic);
		// 材質の反射色　非金属ほど光の色をそのまま反射し、金属ほど材質の色が乗る
        const float3 baseSpecular = lerp(0.04, baseColor.rgb, metallic);

		//------------------
		// 平行光
		//------------------

		// Diffuse(拡散光) 正規化Lambertを使用
		{
			// 光の方向と法線の方向との角度さが光の強さになる
            float lightDiffuse = dot(-g_DL_Dir, wN);
            lightDiffuse = saturate(lightDiffuse); // マイナス値は0にする　0(暗)～1(明)になる
            
			// 正規化Lambert
            lightDiffuse /= 3.1415926535;

			// 光の色 * 材質の拡散色
            color += (g_DL_Color * lightDiffuse) * baseDiffuse * g_Material.BaseColor.a;
        }

		// Specular(反射色) 正規化Blinn-Phong NDFを使用

			// 反射した光の強さを求める

			// ラフネスから、Blinn-Phong用のSpecularPowerを求める
			// Call of Duty : Black OpsやFarCry3でのでの手法を使用
			// specPower = 2^(13*g)　グロス値g = 0～1で1～8192のスペキュラ強度を求める
			// 参考：https://hanecci.hatenadiary.org/entry/20130511/p1
        float smoothness = 1.0 - roughness; // ラフネスを逆転させ「滑らか」さにする
        float specPower = pow(2, 13 * smoothness); // 1～8192

		// UnityのBuild-In Shaderでの手法
		//float spec = RoughnessToSpecPower(roughness);
		{
			// Blinn-Phong NDF
            float spec = BlinnPhong(g_DL_Dir, vCam, wN, specPower);

            spec = step(1, spec) * 10;
            
			// 光の色 * 反射光の強さ * 材質の反射色 * 正規化係数
            color += (g_DL_Color * spec) * baseSpecular * g_Material.BaseColor.a;
        }


        //------------------
        // 点光
        //------------------
		{
            for (int pi = 0; pi < g_PL_Num; pi++)
            {

                float3 PL_Dir;

				//点光源に向いたベクトル
                PL_Dir = g_PL[pi].pos - In.wPos;

                float len;
				//点光源の距離
                len = length(PL_Dir);

				//効果半径かどうか
                if (len < g_PL[pi].radius)
                {

					//点光源の方向をnormalize
                    PL_Dir = normalize(PL_Dir);

                    float lightDiffuse;
                    float lightAttenuation;
					//拡散
                    lightDiffuse = saturate(dot(normalize(wN), PL_Dir));
                    
                    
					// 正規化Lambert
                    lightDiffuse /= 3.1415926535;

					//減衰率
                    lightAttenuation = saturate(1.0f - (len / g_PL[pi].radius));

					//ディフューズ減衰
                    lightDiffuse *= lightAttenuation;

                    color += g_PL[pi].color * lightDiffuse * baseDiffuse * g_Material.BaseColor.a;

					//スペキュラ
                    float spec = BlinnPhong(-PL_Dir, vCam, wN, specPower);
                    
                    spec *= lightAttenuation;
                    color += (g_PL[pi].color * spec) * baseSpecular * g_Material.BaseColor.a;
                }
            }
        }
        
        //------------------
        //スポットライト
        //------------------
        {
            for (int si = 0; si < g_SL_Num; si++)
            {
                //光源に向いたベクトル
                float3 toSLDir = g_SL[si].pos - In.wPos;
                //光源の距離
                float len = length(toSLDir);
                
                //効果範囲内かどうか
                if (len < g_SL[si].radius)
                {
                    toSLDir = normalize(toSLDir);
                    
                    float rad = acos(saturate(dot(toSLDir, -g_SL[si].dir)));
                    if (rad < g_SL[si].angle)
                    {
                        float angleIn = g_SL[si].angle * 0.8;
                        float angleSide = 1 - ((rad - angleIn) / (g_SL[si].angle - angleIn));
                        //光の減衰率
                            
                        float lightAttenuation = saturate(1.0f - (len / g_SL[si].radius)) * angleSide;
                            
                        //拡散
                        float lightDiffuse = saturate(dot(normalize(wN), toSLDir));
                            
                       //ディフューズ減衰
                        lightDiffuse *= lightAttenuation;
                            
                        color += g_SL[si].color * lightDiffuse * baseDiffuse * g_Material.BaseColor.a;

					//スペキュラ
                        float spec = BlinnPhong(-toSLDir, vCam, wN, specPower);
                        spec *= lightAttenuation;
                        color += (g_SL[si].color * spec) * baseSpecular * g_Material.BaseColor.a;
                    }
                }
                
            }
        }
        //------------------
        // 環境光
        //------------------
        color += g_AmbientLight * baseColor.rgb * g_Material.BaseColor.a;

        //------------------
        // エミッシブ
        //------------------
        color += g_emissiveTex.Sample(g_ss, In.UV).rgb * g_Material.Emissive * g_Material.BaseColor.a;
    }
    // ライト無効時
    else
    {
        // 材質の色をそのまま使用
        color = baseColor.rgb * g_Material.BaseColor.a;
    }

    if (g_LimLightEnable)
    {
        //リムライト
        float limLightPow = dot(normalize(In.wPos - g_CamPos), wN);
        limLightPow = 1 - abs(limLightPow);
        color.rgb += float3(g_LimLightColor) * pow(limLightPow, g_LimLightX);
    }
    
     //------------------------------------------
    // ディザリング（ディザ抜き・アルファディザ）
    //------------------------------------------
    if (g_ditherEnable)
    {
        float w = 0;
        float h = 0;
        g_ditherTex.GetDimensions(w, h);
        
        //テクセルサイズ
        float tw = 1.0f / w;
        float th = 1.0f / h;
        
        float dither = g_ditherTex.Sample(g_ss, float2(tw * fmod(In.Pos.x, w), (th * fmod(In.Pos.y, h))));
        
        //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
        //ディザ抜きを開始するカメラの距離
        float ditherDist = 4.0;
        
        float range = max(0, camDist - ditherDist);
        
        //割合算出
        float rate = 1.0f - min(1.0f, range);
        
        clip(dither - 1.0f * rate);
    }
    
    //------------------------------------------
    // 距離フォグ
    //------------------------------------------
    if (g_DistanceFogEnable)
    {
		// フォグ 1(近い)～0(遠い)
        float f = saturate(1.0 / exp(camDist * g_DistanceFogDensity));
        // 適用
        color.rgb = lerp(g_DistanceFogColor, color.rgb, f);
    }

    
    //------------------------------------------
    // 出力
    //------------------------------------------
    return float4(color, baseColor.a);

}
