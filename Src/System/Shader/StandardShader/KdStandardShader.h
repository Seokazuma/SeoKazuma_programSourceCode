#pragma once

//============================================================
//
// ��{�V�F�[�_
//
//============================================================
class KdStandardShader
{
public:

	//================================================
	// �ݒ�E�擾
	//================================================

	// ���[���h�s��Z�b�g
	void SetWorldMatrix(const Math::Matrix& m)
	{
		//		m_cb0.Work().mW = m;
		m_mTransform = m;
	}
	void SetWorldMatrixMw(const Math::Matrix& m)
	{
		m_cb0.Work().mW = m;
	}

	// UV�^�C�����O�ݒ�
	void SetUVTiling(const Math::Vector2& tiling)
	{
		m_cb0.Work().UVTiling = tiling;
	}
	// UV�I�t�Z�b�g�ݒ�
	void SetUVOffset(const Math::Vector2& offset)
	{
		m_cb0.Work().UVOffset = offset;
	}

	// ���C�g�L��/����
	void SetLightEnable(bool enable)
	{
		m_cb0.Work().LightEnable = enable;
	}

	// �A���t�@�e�X�g��臒l�ݒ�
	// �Ealpha	�c ���̒l�ȉ��̃A���t�@�l�̃s�N�Z���͕`�悳��Ȃ��Ȃ�
	void SetAlphaTestThreshold(float alpha)
	{
		m_cb0.Work().AlphaTestThreshold = alpha;
	}

	//---------------------
	// �ގ��p�����[�^�ݒ�
	//---------------------

	// ��{�F�ݒ�
	void SetBaseColor(const Math::Vector4& color);

	// ���Ȕ����F�ݒ�
	void SetEmissive(const Math::Vector3& color)
	{
		m_cb1_Material.Work().Emissive = color;
	}

	// �e���̐ݒ�
	void SetRoughness(float roughness)
	{
		m_cb1_Material.Work().Roughness = roughness;
	}

	// �����x�̐ݒ�
	void SetMetallic(float metallic)
	{
		m_cb1_Material.Work().Metallic = metallic;
	}


	// Set�n�ŃZ�b�g�����f�[�^���A���ۂɒ萔�o�b�t�@�֏�������
	void WriteToCB()
	{
		m_cb0.Write();
		m_cb1_Material.Write();
	}



	//================================================
	// �`��
	//================================================

	// ���̃V�F�[�_���f�o�C�X�փZ�b�g
	void SetToDevice();

	// ���b�V���`��
	// �Emesh			�c �`�悷�郁�b�V��
	// �Ematerials		�c �g�p����ގ��z��
	void DrawModel(const KdModel* model, const float _alpha = 1.0f, const bool _rgbFlg=false, const Math::Vector3 _rgb = { 1,1,1 }, const std::vector<Math::Matrix>* nodeTransforms = nullptr);

	//================================================
	// �������E���
	//================================================

	// ������
	bool Init();
	// ���
	void Release();
	// 
	~KdStandardShader()
	{
		Release();
	}

	void SetDitherEnable(bool enable)
	{
		m_cb0.Work().DitherEnable = enable;
	}


	void SetOpticalCamoEnable(bool _enable)
	{
		m_cb0.Work().OpticalCamoEnable = _enable;
	}

	void SetDissolveEnable(bool enable)
	{
		m_cb0.Work().DissolveEnable = enable;
	}

	void SetDissolvePower(float _pow)
	{
		if (_pow > 1.0f)
		{
			_pow = 1.0f;
		}
		if (_pow < 0.0f)
		{
			_pow = 0.0f;
		}

		m_cb0.Work().dissolvePow = _pow;
	}

	//�������C�g_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	void SetLimLightEnable(bool _enable)
	{
		m_cb0.Work().LimLightEnable = _enable;
	}
	void SetLimLightX(float _x)
	{
		m_cb0.Work().LimLightX = _x;
	}
	void SetLimLightColor(Math::Vector3 _color)
	{
		m_cb0.Work().LimLightColor = _color;
	}

private:

	// 3D���f���p�V�F�[�_
	ID3D11VertexShader* m_VS = nullptr;				// ���_�V�F�[�_�[
	ID3D11InputLayout* m_inputLayout = nullptr;	// ���_���̓��C�A�E�g

	ID3D11PixelShader* m_PS = nullptr;				// �s�N�Z���V�F�[�_�[

	// �s��ێ��p
	Math::Matrix		m_mTransform;

	// �萔�o�b�t�@
	//  ���萔�o�b�t�@�́A�p�b�L���O�K���Ƃ������̂����炵�Ȃ���΂Ȃ�Ȃ�
	//  <�p�b�L���O�K��> �Q�l�Fhttps://docs.microsoft.com/ja-jp/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules
	//  �E�\���̂̃T�C�Y��16�o�C�g�̔{���ɂ���B
	//  �E�e����(�ϐ�)�́A16�o�C�g���E���܂����悤�ȏꍇ�A���̃x�N�g���ɔz�u�����B

	// �萔�o�b�t�@(�I�u�W�F�N�g�P�ʍX�V)
	struct cbObject
	{
		Math::Matrix		mW;		// ���[���h�s��@�s���16�o�C�gx4�o�C�g��64�o�C�g�Ȃ̂Ńs�b�^���B

		// UV����
		Math::Vector2		UVOffset = { 0,0 };
		Math::Vector2		UVTiling = { 1,1 };

		// ���C�g�L��
		int					LightEnable = 1;
		// �A���t�@�e�X�g��臒l
		float				AlphaTestThreshold = 0;

		//���w���ʗL����
		int OpticalCamoEnable = 0;

		//�������C�g_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
		int LimLightEnable = 0;
		float LimLightX = 2;
		Math::Vector3 LimLightColor = { 1,1,1 };

		//�f�B�]���u�L��
		int					DissolveEnable = 0;
		float				dissolvePow;

		//�f�B�U�����O�L��
		int					DitherEnable = 1;

		int					tmp;
	};
	KdConstantBuffer<cbObject>	m_cb0;

	// �萔�o�b�t�@(�}�e���A���P�ʍX�V)
	struct cb {
		Math::Vector4	BaseColor;
		Math::Vector3	Emissive;
		float			Metallic;
		float			Roughness;

		float			tmp[3];
	};
	KdConstantBuffer<cb>	m_cb1_Material;

};


