#include "System/KdSystem.h"

#include "KdModel.h"

#include "KdGLTFLoader.h"

//=============================================================
//
// Mesh
//
//=============================================================

void KdMesh::SetToDevice() const
{
	// 頂点バッファセット
	UINT stride = sizeof(KdMeshVertex);	// 1頂点のサイズ
	UINT offset = 0;					// オフセット
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_vb.GetAddress(), &stride, &offset);

	// インデックスバッファセット
	D3D.GetDevContext()->IASetIndexBuffer(m_ib.GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

	//プリミティブ・トポロジーをセット
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool KdMesh::Create(const std::vector<KdMeshVertex>& vertices, const std::vector<KdMeshFace>& faces, const std::vector<KdMeshSubset>& subsets)
{
	Release();

	//------------------------------
	// サブセット情報
	//------------------------------
	m_subsets = subsets;

	//------------------------------
	// 頂点バッファ作成
	//------------------------------
	if(vertices.size() > 0)
	{
		// 書き込むデータ
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &vertices[0];				// バッファに書き込む頂点配列の先頭アドレス
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		// バッファ作成
		if (FAILED(m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(KdMeshVertex) * vertices.size(), D3D11_USAGE_DEFAULT, &initData)))
		{
			Release();
			return false;
		}

		// 座標のみの配列
		m_positions.resize(vertices.size());
		for (UINT i = 0; i < m_positions.size(); i++)
		{
			m_positions[i] = vertices[i].Pos;
		}

		// AA境界データ作成
		DirectX::BoundingBox::CreateFromPoints(m_aabb, m_positions.size(), &m_positions[0], sizeof(Math::Vector3));
		// 境界球データ作成
		DirectX::BoundingSphere::CreateFromPoints(m_bs, m_positions.size(), &m_positions[0], sizeof(Math::Vector3));
	}	

	//------------------------------
	// インデックスバッファ作成
	//------------------------------
	if(faces.size() > 0)
	{
		// 書き込むデータ
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &faces[0];				// バッファに書き込む頂点配列の先頭アドレス
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		// バッファ作成
		if (FAILED(m_ib.Create(D3D11_BIND_INDEX_BUFFER, faces.size() * sizeof(KdMeshFace), D3D11_USAGE_DEFAULT, &initData)))
		{
			Release();
			return false;
		}

		// 面情報コピー
		m_faces = faces;
	}

	return true;
}


void KdMesh::DrawSubset(int subsetNo) const
{
	// 範囲外のサブセットはスキップ
	if (subsetNo >= (int)m_subsets.size())return;
	// 面数が0なら描画スキップ
	if (m_subsets[subsetNo].FaceCount == 0)return;

	// 描画
	D3D.GetDevContext()->DrawIndexed(m_subsets[subsetNo].FaceCount * 3, m_subsets[subsetNo].FaceStart * 3, 0);
}

bool KdModel::Load(const std::string& filename)
{
	Release();

	// モデルファイルのあるフォルダ名取得
	std::string nowDir = KdGetDirFromPath(filename);
	nowDir += "/";

	//-------------------------------
	// glTFモデル読み込み
	//-------------------------------
	auto model = KdLoadGLTFModel(filename);
	if (model == nullptr)return false;

	// 全マテリアル
	m_materials.resize(model->Materials.size());
	for (UINT mti = 0; mti < m_materials.size(); mti++)
	{
		// GLTFマテリアルデータ
		const auto& gltfMaterial = model->Materials[mti];

		// マテリアルデータセット
		m_materials[mti].Name = gltfMaterial.Name;

		// 基本色
		m_materials[mti].BaseColor = gltfMaterial.BaseColor;
		m_materials[mti].BaseColorTex = std::make_shared<KdTexture>();
		if (m_materials[mti].BaseColorTex->Load(nowDir + gltfMaterial.BaseColorTexture) == false)
		{
			m_materials[mti].BaseColorTex = D3D.GetWhiteTex();
		}


		// 金属性・粗さ
		m_materials[mti].Metallic = gltfMaterial.Metallic;
		m_materials[mti].Roughness = gltfMaterial.Roughness;
		m_materials[mti].MetallicRoughnessTex = std::make_shared<KdTexture>();
		if (m_materials[mti].MetallicRoughnessTex->Load(nowDir + gltfMaterial.MetallicRoughnessTexture) == false)
		{
			m_materials[mti].MetallicRoughnessTex = D3D.GetWhiteTex();
		}


		// 自己発光
		m_materials[mti].Emissive = gltfMaterial.Emissive;
		m_materials[mti].EmissiveTex = std::make_shared<KdTexture>();
		if (m_materials[mti].EmissiveTex->Load(nowDir + gltfMaterial.EmissiveTexture) == false)
		{
			m_materials[mti].EmissiveTex = D3D.GetWhiteTex();
		}


		// 法線マップ
		m_materials[mti].NormalTex = std::make_shared<KdTexture>();
		if (m_materials[mti].NormalTex->Load(nowDir + gltfMaterial.NormalTexture) == false)
		{

		}

	}

	//-------------------------------
	// 全ノード作成
	//-------------------------------
	m_allNodes.resize(model->Nodes.size());

	for (UINT ni = 0; ni < model->Nodes.size(); ni++)
	{
		// glTFノードデータ
		const auto& srcNode = model->Nodes[ni];

		// ノード基本情報
//		std::shared_ptr <KdGLTFNode> newNode = std::make_shared<KdGLTFNode>();
//		m_allNodes.push_back(newNode);

		m_allNodes[ni].Name = srcNode->Name;
		m_allNodes[ni].LocalTransform = srcNode->LocalTransform;
		m_allNodes[ni].Offset = ni;

//		newNode->Name = srcNode->Name;						// 名前
//		newNode->Offset = srcNode->Offset;					// 
//		newNode->LocalTransform = srcNode->LocalTransform;	// 行列
//		newNode->Transform = srcNode->WorldTransform;		// 行列

		// メッシュ
		if (srcNode->IsMesh)
		{
			m_allNodes[ni].Mesh = std::make_shared<KdMesh>();
			// 作成
			m_allNodes[ni].Mesh->Create(srcNode->Mesh.Vertices, srcNode->Mesh.Faces, srcNode->Mesh.Subsets);
		}

	}

	/*
	//-------------------
	// 親子構築
	//-------------------
	for (UINT ni = 0; ni < model->Nodes.size(); ni++)
	{
		const auto& srcNode = model->Nodes[ni];

		// 子登録
		const auto& srcChildren = srcNode->Children;
		m_allNodes[ni].Children.resize(srcChildren.size());
		for (UINT ci = 0; ci < srcChildren.size(); ci++)
		{
			int childIndex = srcChildren[ci]->Offset;

			m_allNodes[ni]->Children[ci] = m_allNodes[childIndex];
		}
	}
	*/

	//-------------------
	// ルートノードのみの参照
	//-------------------
	m_onlyRootNodes.resize(model->RootNodes.size());
	for (UINT ni = 0; ni < m_onlyRootNodes.size(); ni++)
	{
		int idx = model->RootNodes[ni]->Offset;
		m_onlyRootNodes[ni] = m_allNodes[idx];
	}

	//-------------------
	// アニメーションデータ
	//-------------------
	//m_animations = model->Animations;

	// 
	/*
	for (UINT ni = 0; ni < model->Nodes.size(); ni++)
	{
		// glTFノードデータ
		const auto& srcNode = model->Nodes[ni];

		// メッシュ
		if (srcNode->IsMesh)
		{
			m_mesh = std::make_shared<KdMesh>();
			m_mesh->Create(srcNode->Mesh.Vertices, srcNode->Mesh.Faces, srcNode->Mesh.Subsets);
//			break;
		}
	}
	*/
	return true;
}
