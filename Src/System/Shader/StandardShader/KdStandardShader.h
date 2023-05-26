#pragma once

//============================================================
//
// 基本シェーダ
//
//============================================================
class KdStandardShader
{
public:

	//================================================
	// 設定・取得
	//================================================

	// ワールド行列セット
	void SetWorldMatrix(const Math::Matrix& m)
	{
		//		m_cb0.Work().mW = m;
		m_mTransform = m;
	}
	void SetWorldMatrixMw(const Math::Matrix& m)
	{
		m_cb0.Work().mW = m;
	}

	// UVタイリング設定
	void SetUVTiling(const Math::Vector2& tiling)
	{
		m_cb0.Work().UVTiling = tiling;
	}
	// UVオフセット設定
	void SetUVOffset(const Math::Vector2& offset)
	{
		m_cb0.Work().UVOffset = offset;
	}

	// ライト有効/無効
	void SetLightEnable(bool enable)
	{
		m_cb0.Work().LightEnable = enable;
	}

	// アルファテストの閾値設定
	// ・alpha	… この値以下のアルファ値のピクセルは描画されなくなる
	void SetAlphaTestThreshold(float alpha)
	{
		m_cb0.Work().AlphaTestThreshold = alpha;
	}

	//---------------------
	// 材質パラメータ設定
	//---------------------

	// 基本色設定
	void SetBaseColor(const Math::Vector4& color);

	// 自己発光色設定
	void SetEmissive(const Math::Vector3& color)
	{
		m_cb1_Material.Work().Emissive = color;
	}

	// 粗さの設定
	void SetRoughness(float roughness)
	{
		m_cb1_Material.Work().Roughness = roughness;
	}

	// 金属度の設定
	void SetMetallic(float metallic)
	{
		m_cb1_Material.Work().Metallic = metallic;
	}


	// Set系でセットしたデータを、実際に定数バッファへ書き込む
	void WriteToCB()
	{
		m_cb0.Write();
		m_cb1_Material.Write();
	}



	//================================================
	// 描画
	//================================================

	// このシェーダをデバイスへセット
	void SetToDevice();

	// メッシュ描画
	// ・mesh			… 描画するメッシュ
	// ・materials		… 使用する材質配列
	void DrawModel(const KdModel* model, const float _alpha = 1.0f, const bool _rgbFlg=false, const Math::Vector3 _rgb = { 1,1,1 }, const std::vector<Math::Matrix>* nodeTransforms = nullptr);

	//================================================
	// 初期化・解放
	//================================================

	// 初期化
	bool Init();
	// 解放
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

	//リムライト_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
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

	// 3Dモデル用シェーダ
	ID3D11VertexShader* m_VS = nullptr;				// 頂点シェーダー
	ID3D11InputLayout* m_inputLayout = nullptr;	// 頂点入力レイアウト

	ID3D11PixelShader* m_PS = nullptr;				// ピクセルシェーダー

	// 行列保持用
	Math::Matrix		m_mTransform;

	// 定数バッファ
	//  ※定数バッファは、パッキング規則というものを厳守しなければならない
	//  <パッキング規則> 参考：https://docs.microsoft.com/ja-jp/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules
	//  ・構造体のサイズは16バイトの倍数にする。
	//  ・各項目(変数)は、16バイト境界をまたぐような場合、次のベクトルに配置される。

	// 定数バッファ(オブジェクト単位更新)
	struct cbObject
	{
		Math::Matrix		mW;		// ワールド行列　行列は16バイトx4バイトの64バイトなのでピッタリ。

		// UV操作
		Math::Vector2		UVOffset = { 0,0 };
		Math::Vector2		UVTiling = { 1,1 };

		// ライト有効
		int					LightEnable = 1;
		// アルファテストの閾値
		float				AlphaTestThreshold = 0;

		//光学迷彩有効化
		int OpticalCamoEnable = 0;

		//リムライト_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
		int LimLightEnable = 0;
		float LimLightX = 2;
		Math::Vector3 LimLightColor = { 1,1,1 };

		//ディゾルブ有効
		int					DissolveEnable = 0;
		float				dissolvePow;

		//ディザリング有効
		int					DitherEnable = 1;

		int					tmp;
	};
	KdConstantBuffer<cbObject>	m_cb0;

	// 定数バッファ(マテリアル単位更新)
	struct cb {
		Math::Vector4	BaseColor;
		Math::Vector3	Emissive;
		float			Metallic;
		float			Roughness;

		float			tmp[3];
	};
	KdConstantBuffer<cb>	m_cb1_Material;

};


