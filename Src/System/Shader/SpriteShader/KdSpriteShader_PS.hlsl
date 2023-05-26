#include "inc_KdSpriteShader.hlsli"

// �e�N�X�`��
Texture2D g_inputTex : register(t0);
// �T���v��
SamplerState g_ss : register(s0);


//============================================
// 2D�`�� �s�N�Z���V�F�[�_
//============================================
float4 main(VSOutput In) : SV_Target0
{

    if (g_EdgeParamEnable)
    {
		//�e�N�X�`���̃T�C�Y���擾
        int texW, texH;
        g_inputTex.GetDimensions(texW, texH);
        float tw = g_texel / texW;
        float th = g_texel / texH;
	
    //�e�N�X�`���̐F���擾�iZ�����擾
        float z = g_inputTex.Sample(g_ss, In.UV).r;
    
    //�אڂ��Ă���e�N�X�`���̐F
        float z1 = g_inputTex.Sample(g_ss, In.UV + float2(-tw, -th)).r;
        float z2 = g_inputTex.Sample(g_ss, In.UV + float2(0, -th)).r;
        float z3 = g_inputTex.Sample(g_ss, In.UV + float2(tw, -th)).r;
        float z4 = g_inputTex.Sample(g_ss, In.UV + float2(-tw, 0)).r;
        float z5 = g_inputTex.Sample(g_ss, In.UV + float2(tw, 0)).r;
        float z6 = g_inputTex.Sample(g_ss, In.UV + float2(-tw, th)).r;
        float z7 = g_inputTex.Sample(g_ss, In.UV + float2(0, th)).r;
        float z8 = g_inputTex.Sample(g_ss, In.UV + float2(tw, th)).r;
    
    //����
        float v = 0;
    //saturate=�l��0�`1�̊Ԃɂ��Ă����@
    //max(A,B)=A��B���r���đ傫������Ԃ�
        v += saturate(max(0, z1 - z) / g_value);
        v += saturate(max(0, z2 - z) / g_value);
        v += saturate(max(0, z3 - z) / g_value);
        v += saturate(max(0, z4 - z) / g_value);
        v += saturate(max(0, z5 - z) / g_value);
        v += saturate(max(0, z6 - z) / g_value);
        v += saturate(max(0, z7 - z) / g_value);
        v += saturate(max(0, z8 - z) / g_value);
    //pow(A,B)=A��B�悵�Ă����
        v = pow(v, g_pow);
    
        return float4(0, 0, 0, v);
    }
    else
    {
        // �e�N�X�`���F�擾
        float4 texCol = g_inputTex.Sample(g_ss, In.UV);
        //�A���t�@�e�X�g
        if (texCol.a < 0.1f)
        discard;

	    // �e�N�X�`���F * �w��F
        return texCol * g_color;
    }
	
}
