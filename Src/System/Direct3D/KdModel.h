#pragma once

//==========================================================
// メッシュ用 頂点情報
//==========================================================
struct KdMeshVertex
{
	Math::Vector3	Pos;						// 座標
	Math::Vector2	UV;							// UV
	Math::Vector3	Normal;						// 法線
	unsigned int	Color = 0xFFFFFFFF;			// RGBA色(容量削減のため、各色0～255のUINT型)
};

//==========================================================
// メッシュ用 面情報
//==========================================================
struct KdMeshFace
{
	UINT Idx[3];				// 三角形を構成する頂点のIndex
};

//==========================================================
// メッシュ用 サブセット情報
//==========================================================
struct KdMeshSubset
{
	UINT		MaterialNo = 0;		// マテリアルNo

	UINT		FaceStart = 0;		// 面Index　このマテリアルで使用されている最初の面のIndex
	UINT		FaceCount = 0;		// 面数　FaceStartから、何枚の面が使用されているかの
};

//==========================================================
//
// メッシュクラス
//
//==========================================================
class KdMesh
{
public:

	//=================================================
	// 取得・設定
	//=================================================

	// サブセット情報配列を取得
	const std::vector<KdMeshSubset>&	GetSubsets() const { return m_subsets; }

	// 頂点の座標配列を取得
	const std::vector<Math::Vector3>&	GetVertexPositions() const { return m_positions; }
	// 面の配列を取得
	const std::vector<KdMeshFace>&		GetFaces() const { return m_faces; }

	// 軸平行境界ボックス取得
	const DirectX::BoundingBox&			GetBoundingBox() const { return m_aabb; }
	// 境界球取得
	const DirectX::BoundingSphere&		GetBoundingSphere() const { return m_bs; }

	// メッシュデータをデバイスへセットする
	void SetToDevice() const;


	//=================================================
	// 作成・解放
	//=================================================

	// メッシュ作成
	// ・vertices		… 頂点配列
	// ・faces			… 面インデックス情報配列
	// ・subsets		… サブセット情報配列
	// 戻り値			… 成功：true
	bool Create(const std::vector<KdMeshVertex>& vertices, const std::vector<KdMeshFace>& faces, const std::vector<KdMeshSubset>& subsets);

	// 解放
	void Release()
	{
		m_vb.Release();
		m_ib.Release();
		m_subsets.clear();
		m_positions.clear();
		m_faces.clear();
	}

	~KdMesh()
	{
		Release();
	}

	//=================================================
	// 処理
	//=================================================

	// 指定サブセットを描画
	void DrawSubset(int subsetNo) const;

	// 
	KdMesh() {}

private:

	// 頂点バッファ
	KdBuffer						m_vb;
	// インデックスバッファ
	KdBuffer						m_ib;

	// サブセット情報
	std::vector<KdMeshSubset>		m_subsets;

	// 境界データ
	DirectX::BoundingBox			m_aabb;	// 軸平行境界ボックス
	DirectX::BoundingSphere			m_bs;	// 境界球

	// 座標のみの配列(複製)
	std::vector<Math::Vector3>		m_positions;
	// 面情報のみの配列(複製)
	std::vector<KdMeshFace>			m_faces;

private:
	// コピー禁止用
	KdMesh(const KdMesh& src) = delete;
	void operator=(const KdMesh& src) = delete;
};

//==========================================================
// マテリアル(glTFベースのPBRマテリアル)
//==========================================================
struct KdMaterial
{
	//---------------------------------------
	// 材質データ
	//---------------------------------------

	// 名前
	std::string					Name;

	// 基本色
	std::shared_ptr<KdTexture>	BaseColorTex;				// 基本色テクスチャ
	Math::Vector4				BaseColor = { 1,1,1,1 };	// 基本色のスケーリング係数(RGBA)

	// 金属性、粗さ
	std::shared_ptr<KdTexture>	MetallicRoughnessTex;		// B:金属製 G:粗さ
	float						Metallic = 0.0f;			// 金属性のスケーリング係数
	float						Roughness = 1.0f;			// 粗さのスケーリング係数

	// 自己発光
	std::shared_ptr<KdTexture>	EmissiveTex;				// 自己発光テクスチャ
	Math::Vector3				Emissive = { 0,0,0 };		// 自己発光のスケーリング係数(RGB)

	// 法線マップ
	std::shared_ptr<KdTexture>	NormalTex;

	KdMaterial()
	{
		BaseColorTex			= D3D.GetWhiteTex();
		MetallicRoughnessTex	= D3D.GetWhiteTex();
		EmissiveTex				= D3D.GetWhiteTex();
	}
};

//============================
// アニメーション
//============================
struct KdAnimationData
{
	// アニメーション名
	std::string				Name;
	// アニメの長さ
	float					MaxLength = 0;

	// クォータニオンキー
	struct KeyQuaternion {
		float				Time = 0;		// 時間
		Math::Quaternion	Quat;			// クォータニオンデータ

		Math::Quaternion	PrevTangent;	// CUBICSPLINE補間などで使用
		Math::Quaternion	NextTangent;	// CUBICSPLINE補間などで使用
	};
	// ベクトルキー
	struct KeyVector3 {
		float				Time = 0;		// 時間
		Math::Vector3		Vec;			// 3Dベクトルデータ

		Math::Vector3		PrevTangent;	// CUBICSPLINE補間などで使用
		Math::Vector3		NextTangent;	// CUBICSPLINE補間などで使用
	};
	// １ノードのアニメーションデータ
	struct Node
	{
		int							NodeOffset = -1;	// 対象ノードのOffset

		// 各チャンネル
		std::vector<KeyQuaternion>	Rotations;		// 回転キーリスト
		std::vector<KeyVector3>		Scales;			// 拡大キーリスト
		std::vector<KeyVector3>		Translations;	// 位置キーリスト
	};

	// アニメーションデータ
	std::vector<std::shared_ptr<Node>>		Nodes;

};

class KdModel
{
public:
	struct Node;
	using kdGLTFNode = std::shared_ptr<Node>;

	bool Load(const std::string& filename);

	//==========================================================
	// 取得
	//==========================================================

	// 全ノード配列取得
	const std::vector<Node>& GetAllNodes() const { return m_allNodes; }
	// 全ノード中のルートノードのみの参照配列取得
	const std::vector<Node>& GetOnlyRootNodes() const { return m_onlyRootNodes; }
	// マテリアル配列取得
	const std::vector<KdMaterial>& GetMaterials() const { return m_materials; }
	// アニメーションデータリスト
	//const std::vector<KdAnimationData>& GetAnimations() const { return m_animations; }


	void Release()
	{
//		m_mesh = nullptr;
		m_materials.clear();
		m_allNodes.clear();
		m_onlyRootNodes.clear();
	}

	struct Node
	{
		// ノード名
		std::string				Name;
		// オフセット
		int						Offset = -1;
		// 座標
		Math::Matrix			LocalTransform;
		// メッシュ
		std::shared_ptr <KdMesh>	Mesh;
	};

private:
//	std::shared_ptr <KdMesh>	m_mesh;
	// 全ノード配列
	std::vector<Node>				m_allNodes;
	// m_allNodes中、ルートノードのみの参照
	std::vector<Node>				m_onlyRootNodes;

	// マテリアル配列
	std::vector<KdMaterial>			m_materials;

	// アニメーションデータリスト
//	std::vector<KdAnimationData>	m_animations;
};
