#include "../inc_KdCommon.hlsli"
#include "inc_KdStandardShader.hlsli"

// �e�N�X�`��
Texture2D g_baseTex : register(t0); // �x�[�X�J���[�e�N�X�`��
Texture2D g_emissiveTex : register(t1); // �G�~�b�V�u�e�N�X�`��
Texture2D g_mrTex : register(t2); // ���^���b�N/���t�l�X�e�N�X�`��

Texture2D g_sceenTex : register(t3); //�Q�[����ʂ̃e�N�X�`��

Texture2D g_dissolveTex : register(t4); //�Q�[����ʂ̃e�N�X�`��

Texture2D g_ditherTex : register(t5);


// �T���v��
SamplerState g_ss : register(s0);

/*
// Unity��Build-In Shader�ł̎�@
// Roughness����SpecularPower���Z�o
float RoughnessToSpecPower(float roughness)
{
    float trueRoughness = roughness * roughness; // �w�p�I�ɐ��������t�l�X�����߂�
    float sq = max(0.0001, trueRoughness * trueRoughness);
    float n = (2.0 / sq) - 2.0;
    // Roughness��1.0��NdotH���[���̏ꍇ�ɔ�������\���̂���pow(0,0)�̃P�[�X��h�~
    n = max(n, 0.0001);
    return n;
}
*/

// BlinnPhong NDF
// �ElightDir    �c ���C�g�̕���
// �EvCam        �c �s�N�Z������J�����ւ̕���
// �Enormal      �c �@��
// �EspecPower   �c ���˂̉s��
float BlinnPhong(float3 lightDir, float3 vCam, float3 normal, float specPower)
{
    float3 H = normalize(-lightDir + vCam);
    float NdotH = saturate(dot(normal, H)); // �J�����̊p�x��(0�`1)
    float spec = pow(NdotH, specPower);

    // ���K��Blinn-Phong
    return spec * ((specPower + 2) / (2 * 3.1415926535));
}

//================================
// �s�N�Z���V�F�[�_
//================================
float4 main(VSOutput In) : SV_Target0
{
    // �J�����ւ̕���
    float3 vCam = g_CamPos - In.wPos;
    float camDist = length(vCam); // �J���� - �s�N�Z������
    vCam = normalize(vCam);

    // �@�����K��
    float3 wN = normalize(In.wN);

    if (g_OpticalCamoEnable)
    {
        //�e�N�X�`���̃T�C�Y�擾
        float w = 0;
        float h = 0;
        g_sceenTex.GetDimensions(w, h); //1280x720
        
        //�@�����炸����Z�o
        float2 offset = wN.xy / 50;
        
        //�L�����̕\���ʒu�̃e�N�X�`�����W��UV���W�Ɍ���
        float2 uv = float2(In.Pos.x / w, In.Pos.y / h);
        
        //�e�N�X�`������F���擾
        float4 col = g_sceenTex.Sample(g_ss, uv + offset);
        
        //�����킩��₷���F������
        col.r *= 2.15f;
        col.g *= 2.0f;
        col.b *= 2.15f;
        
        return col;

    }

    //------------------------------------------
    // �ގ��F
    //------------------------------------------

    float4 mr = g_mrTex.Sample(g_ss, In.UV);
    // ������
    float metallic = mr.b * g_Material.Metallic;
    // �e��
    float roughness = mr.g * g_Material.Roughness;


    // �ގ��̐F
    float4 baseColor = g_baseTex.Sample(g_ss, In.UV) * g_Material.BaseColor * In.Color;

    // �A���t�@�e�X�g
    if (baseColor.a <= g_AlphaTestThreshold)
        discard;

     //------------------------------------------
    // dissolve�f�B�]���u
   //------------------------------------------
    if (g_dissolveEnable)
    {
        
        float dissolveColor = g_dissolveTex.Sample(g_ss, In.UV);
        if (dissolveColor.r < g_dissolvePow)
        {
        //�s�N�Z�����ʂ��o�͂��Ȃ��i�j���j
        //�߂�l��߂��Ȃ����^�[��return
            discard;
        }
        
        //���ڋ���
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
    // ���C�e�B���O
    //
    //------------------------------------------
    // �ŏI�I�ȐF
    float3 color = 0;

    // ���C�g�L����
    if (g_LightEnable)
    {
		// �ގ��̊g�U�F�@������قǍގ��̐F�ɂȂ�A�����قǊg�U�F�͖����Ȃ�
        const float3 baseDiffuse = lerp(baseColor.rgb, float3(0, 0, 0), metallic);
		// �ގ��̔��ːF�@������قǌ��̐F�����̂܂ܔ��˂��A�����قǍގ��̐F�����
        const float3 baseSpecular = lerp(0.04, baseColor.rgb, metallic);

		//------------------
		// ���s��
		//------------------

		// Diffuse(�g�U��) ���K��Lambert���g�p
		{
			// ���̕����Ɩ@���̕����Ƃ̊p�x�������̋����ɂȂ�
            float lightDiffuse = dot(-g_DL_Dir, wN);
            lightDiffuse = saturate(lightDiffuse); // �}�C�i�X�l��0�ɂ���@0(��)�`1(��)�ɂȂ�
            
			// ���K��Lambert
            lightDiffuse /= 3.1415926535;

			// ���̐F * �ގ��̊g�U�F
            color += (g_DL_Color * lightDiffuse) * baseDiffuse * g_Material.BaseColor.a;
        }

		// Specular(���ːF) ���K��Blinn-Phong NDF���g�p

			// ���˂������̋��������߂�

			// ���t�l�X����ABlinn-Phong�p��SpecularPower�����߂�
			// Call of Duty : Black Ops��FarCry3�ł̂ł̎�@���g�p
			// specPower = 2^(13*g)�@�O���X�lg = 0�`1��1�`8192�̃X�y�L�������x�����߂�
			// �Q�l�Fhttps://hanecci.hatenadiary.org/entry/20130511/p1
        float smoothness = 1.0 - roughness; // ���t�l�X���t�]�����u���炩�v���ɂ���
        float specPower = pow(2, 13 * smoothness); // 1�`8192

		// Unity��Build-In Shader�ł̎�@
		//float spec = RoughnessToSpecPower(roughness);
		{
			// Blinn-Phong NDF
            float spec = BlinnPhong(g_DL_Dir, vCam, wN, specPower);

            spec = step(1, spec) * 10;
            
			// ���̐F * ���ˌ��̋��� * �ގ��̔��ːF * ���K���W��
            color += (g_DL_Color * spec) * baseSpecular * g_Material.BaseColor.a;
        }


        //------------------
        // �_��
        //------------------
		{
            for (int pi = 0; pi < g_PL_Num; pi++)
            {

                float3 PL_Dir;

				//�_�����Ɍ������x�N�g��
                PL_Dir = g_PL[pi].pos - In.wPos;

                float len;
				//�_�����̋���
                len = length(PL_Dir);

				//���ʔ��a���ǂ���
                if (len < g_PL[pi].radius)
                {

					//�_�����̕�����normalize
                    PL_Dir = normalize(PL_Dir);

                    float lightDiffuse;
                    float lightAttenuation;
					//�g�U
                    lightDiffuse = saturate(dot(normalize(wN), PL_Dir));
                    
                    
					// ���K��Lambert
                    lightDiffuse /= 3.1415926535;

					//������
                    lightAttenuation = saturate(1.0f - (len / g_PL[pi].radius));

					//�f�B�t���[�Y����
                    lightDiffuse *= lightAttenuation;

                    color += g_PL[pi].color * lightDiffuse * baseDiffuse * g_Material.BaseColor.a;

					//�X�y�L����
                    float spec = BlinnPhong(-PL_Dir, vCam, wN, specPower);
                    
                    spec *= lightAttenuation;
                    color += (g_PL[pi].color * spec) * baseSpecular * g_Material.BaseColor.a;
                }
            }
        }
        
        //------------------
        //�X�|�b�g���C�g
        //------------------
        {
            for (int si = 0; si < g_SL_Num; si++)
            {
                //�����Ɍ������x�N�g��
                float3 toSLDir = g_SL[si].pos - In.wPos;
                //�����̋���
                float len = length(toSLDir);
                
                //���ʔ͈͓����ǂ���
                if (len < g_SL[si].radius)
                {
                    toSLDir = normalize(toSLDir);
                    
                    float rad = acos(saturate(dot(toSLDir, -g_SL[si].dir)));
                    if (rad < g_SL[si].angle)
                    {
                        float angleIn = g_SL[si].angle * 0.8;
                        float angleSide = 1 - ((rad - angleIn) / (g_SL[si].angle - angleIn));
                        //���̌�����
                            
                        float lightAttenuation = saturate(1.0f - (len / g_SL[si].radius)) * angleSide;
                            
                        //�g�U
                        float lightDiffuse = saturate(dot(normalize(wN), toSLDir));
                            
                       //�f�B�t���[�Y����
                        lightDiffuse *= lightAttenuation;
                            
                        color += g_SL[si].color * lightDiffuse * baseDiffuse * g_Material.BaseColor.a;

					//�X�y�L����
                        float spec = BlinnPhong(-toSLDir, vCam, wN, specPower);
                        spec *= lightAttenuation;
                        color += (g_SL[si].color * spec) * baseSpecular * g_Material.BaseColor.a;
                    }
                }
                
            }
        }
        //------------------
        // ����
        //------------------
        color += g_AmbientLight * baseColor.rgb * g_Material.BaseColor.a;

        //------------------
        // �G�~�b�V�u
        //------------------
        color += g_emissiveTex.Sample(g_ss, In.UV).rgb * g_Material.Emissive * g_Material.BaseColor.a;
    }
    // ���C�g������
    else
    {
        // �ގ��̐F�����̂܂܎g�p
        color = baseColor.rgb * g_Material.BaseColor.a;
    }

    if (g_LimLightEnable)
    {
        //�������C�g
        float limLightPow = dot(normalize(In.wPos - g_CamPos), wN);
        limLightPow = 1 - abs(limLightPow);
        color.rgb += float3(g_LimLightColor) * pow(limLightPow, g_LimLightX);
    }
    
     //------------------------------------------
    // �f�B�U�����O�i�f�B�U�����E�A���t�@�f�B�U�j
    //------------------------------------------
    if (g_ditherEnable)
    {
        float w = 0;
        float h = 0;
        g_ditherTex.GetDimensions(w, h);
        
        //�e�N�Z���T�C�Y
        float tw = 1.0f / w;
        float th = 1.0f / h;
        
        float dither = g_ditherTex.Sample(g_ss, float2(tw * fmod(In.Pos.x, w), (th * fmod(In.Pos.y, h))));
        
        //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
        //�f�B�U�������J�n����J�����̋���
        float ditherDist = 4.0;
        
        float range = max(0, camDist - ditherDist);
        
        //�����Z�o
        float rate = 1.0f - min(1.0f, range);
        
        clip(dither - 1.0f * rate);
    }
    
    //------------------------------------------
    // �����t�H�O
    //------------------------------------------
    if (g_DistanceFogEnable)
    {
		// �t�H�O 1(�߂�)�`0(����)
        float f = saturate(1.0 / exp(camDist * g_DistanceFogDensity));
        // �K�p
        color.rgb = lerp(g_DistanceFogColor, color.rgb, f);
    }

    
    //------------------------------------------
    // �o��
    //------------------------------------------
    return float4(color, baseColor.a);

}
