#include "inc_KdSpriteShader.hlsli"

// テクスチャ
Texture2D g_inputTex : register(t0);
// サンプラ
SamplerState g_ss : register(s0);


//============================================
// 2D描画 ピクセルシェーダ
//============================================
float4 main(VSOutput In) : SV_Target0
{

    if (g_EdgeParamEnable)
    {
		//テクスチャのサイズを取得
        int texW, texH;
        g_inputTex.GetDimensions(texW, texH);
        float tw = g_texel / texW;
        float th = g_texel / texH;
	
    //テクスチャの色を取得（Z情報を取得
        float z = g_inputTex.Sample(g_ss, In.UV).r;
    
    //隣接しているテクスチャの色
        float z1 = g_inputTex.Sample(g_ss, In.UV + float2(-tw, -th)).r;
        float z2 = g_inputTex.Sample(g_ss, In.UV + float2(0, -th)).r;
        float z3 = g_inputTex.Sample(g_ss, In.UV + float2(tw, -th)).r;
        float z4 = g_inputTex.Sample(g_ss, In.UV + float2(-tw, 0)).r;
        float z5 = g_inputTex.Sample(g_ss, In.UV + float2(tw, 0)).r;
        float z6 = g_inputTex.Sample(g_ss, In.UV + float2(-tw, th)).r;
        float z7 = g_inputTex.Sample(g_ss, In.UV + float2(0, th)).r;
        float z8 = g_inputTex.Sample(g_ss, In.UV + float2(tw, th)).r;
    
    //差分
        float v = 0;
    //saturate=値を0～1の間にしてくれる　
    //max(A,B)=AとBを比較して大きい方を返す
        v += saturate(max(0, z1 - z) / g_value);
        v += saturate(max(0, z2 - z) / g_value);
        v += saturate(max(0, z3 - z) / g_value);
        v += saturate(max(0, z4 - z) / g_value);
        v += saturate(max(0, z5 - z) / g_value);
        v += saturate(max(0, z6 - z) / g_value);
        v += saturate(max(0, z7 - z) / g_value);
        v += saturate(max(0, z8 - z) / g_value);
    //pow(A,B)=AをB乗してくれる
        v = pow(v, g_pow);
    
        return float4(0, 0, 0, v);
    }
    else
    {
        // テクスチャ色取得
        float4 texCol = g_inputTex.Sample(g_ss, In.UV);
        //アルファテスト
        if (texCol.a < 0.1f)
        discard;

	    // テクスチャ色 * 指定色
        return texCol * g_color;
    }
	
}
