#include "System/KdSystem.h"

#include "KdStandardShader.h"


bool KdStandardShader::Init()
{
	//-------------------------------------
	// 頂点シェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdStandardShader_VS.inc"

// 頂点シェーダー作成
		if (FAILED(D3D.GetDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS))) {
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}

		// １頂点の詳細な情報
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// 頂点入力レイアウト作成
		if (FAILED(D3D.GetDev()->CreateInputLayout(
			&layout[0],			// 入力エレメント先頭アドレス
			layout.size(),		// 入力エレメント数
			&compiledBuffer[0],				// 頂点バッファのバイナリデータ
			sizeof(compiledBuffer),			// 上記のバッファサイズ
			&m_inputLayout))					// 
			) {
			assert(0 && "CreateInputLayout失敗");
			Release();
			return false;
		}

	}

	//-------------------------------------
	// ピクセルシェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdStandardShader_PS.inc"

		if (FAILED(D3D.GetDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS))) {
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// 定数バッファ作成
	//-------------------------------------
	m_cb0.Create();
	m_cb1_Material.Create();

	return true;
}

void KdStandardShader::Release()
{
	KdSafeRelease(m_VS);
	KdSafeRelease(m_PS);
	KdSafeRelease(m_inputLayout);
	m_cb0.Release();
	m_cb1_Material.Release();

}


// 基本色設定

inline void KdStandardShader::SetBaseColor(const Math::Vector4& color)
{
	m_cb1_Material.Work().BaseColor = color;
}

void KdStandardShader::SetToDevice()
{
	// 頂点シェーダをセット
	D3D.GetDevContext()->VSSetShader(m_VS, 0, 0);
	// 頂点レイアウトをセット
	D3D.GetDevContext()->IASetInputLayout(m_inputLayout);

	// ピクセルシェーダをセット
	D3D.GetDevContext()->PSSetShader(m_PS, 0, 0);

	// 定数バッファをセット
	D3D.GetDevContext()->VSSetConstantBuffers(0, 1, m_cb0.GetAddress());
	D3D.GetDevContext()->PSSetConstantBuffers(0, 1, m_cb0.GetAddress());

	D3D.GetDevContext()->VSSetConstantBuffers(1, 1, m_cb1_Material.GetAddress());
	D3D.GetDevContext()->PSSetConstantBuffers(1, 1, m_cb1_Material.GetAddress());

}

void KdStandardShader::DrawModel(const KdModel* model, const float _alpha, const bool _rgbFlg, const Math::Vector3 _rgb, const std::vector<Math::Matrix>* nodeTransforms)
{
	if (model == nullptr)return;

	//if (mesh == nullptr)return;

	// 全ノード内のメッシュを描画
	for (UINT nodei = 0; nodei < model->GetAllNodes().size(); nodei++)
	{
		// ノード
		const KdModel::Node& node = model->GetAllNodes()[nodei];
		// メッシュが存在しない場合はスキップ
		if (node.Mesh == nullptr)continue;

		// メッシュ情報をセット
		node.Mesh->SetToDevice();

		// 行列
		if (nodeTransforms && nodei < nodeTransforms->size()) {
			m_cb0.Work().mW = (*nodeTransforms)[nodei] * m_mTransform;
		}
		else {
			m_cb0.Work().mW = node.LocalTransform * m_mTransform;
		}

		// 定数バッファ書き込み
		m_cb0.Write();

		// 全サブセット
		//std::shared_ptr<const KdMaterial> material;

		for (UINT subi = 0; subi < node.Mesh->GetSubsets().size(); subi++)
		{
			// 面が１枚も無い場合はスキップ
			if (node.Mesh->GetSubsets()[subi].FaceCount == 0)continue;

			// マテリアルセット
			//const KdMaterial& material = materials[mesh->GetSubsets()[subi].MaterialNo];
			// マテリアルセット
			int matNo = -1;
			if (matNo != node.Mesh->GetSubsets()[subi].MaterialNo)
			{
				//material = node.Mesh->GetSubsets()[subi].MaterialData;
				KdMaterial material = model->GetMaterials()[node.Mesh->GetSubsets()[subi].MaterialNo];

				//-----------------------
				// マテリアル情報を定数バッファへ書き込む
				//-----------------------
				material.BaseColor.w *= _alpha;

				if (_rgbFlg == true)
				{
					material.BaseColor.x = _rgb.x;
					material.BaseColor.y = _rgb.y;
					material.BaseColor.z = _rgb.z;
				}

				m_cb1_Material.Work().BaseColor = material.BaseColor;
				m_cb1_Material.Work().Emissive = material.Emissive;
				m_cb1_Material.Work().Metallic = material.Metallic;
				m_cb1_Material.Work().Roughness = material.Roughness;
				m_cb1_Material.Write();

				//-----------------------
				// テクスチャセット
				//-----------------------

				ID3D11ShaderResourceView* srvs[3] = {};

				// BaseColor
				srvs[0] = material.BaseColorTex->GetSRView();
				// Emissive
				srvs[1] = material.EmissiveTex->GetSRView();
				// Metallic Roughness
				srvs[2] = material.MetallicRoughnessTex->GetSRView();

				// セット
				D3D.GetDevContext()->PSSetShaderResources(0, _countof(srvs), srvs);
			}

			//-----------------------
			// サブセット描画
			//-----------------------
			node.Mesh->DrawSubset(subi);
		}

	}
}


