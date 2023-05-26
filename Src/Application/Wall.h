#pragma once

class Wall
{
public:
	Wall(GameScene* _gameScene);
	~Wall();

	void Update();
	void Draw();

	//マップ表示用
	void MapTexDraw();

	//前後左右に壁があるか判定
	const void HitWall(Math::Vector3& _pos, float& _size, bool& _upXHit, bool& _downXHit, bool& _upZHit, bool& _downZHit,Math::Matrix& _wallXMat,Math::Matrix& _wallZMat,float& _wallsize)const;
	//壁との当たり判定
	const void CollisionDetection(Math::Matrix _mat, Math::Vector3& _pos, float& _size, float& _wallsize, Math::Vector3& _dis, float& _sizeSum,bool& _hit,Math::Matrix& _wallMat)const;
	//調べたい座標がマップ上の何かを調べる
	const int ChecMapGrid(const int& _x, const int& _z)const;
	//一番近い交差地点を探す
	const Math::Matrix NearDestination(const Math::Matrix& _mat)const;
	//壁レイ判定
	const bool CheckWall(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec) const;
	//マップ上の交差地点の数を調べる
	const int GetIntersectionNum()const;
	//敵から送られてきた_spawnNumで敵のスポーン地点を決める
	const Math::Vector3 GetIntersectionPos(const int& _intersectionNum)const;

private:


	KdModel* m_model[4];
	KdModel* m_invisibleModel;

	//表示するマップのテクスチャ
	KdTexture* m_mapWallTex;
	KdTexture* m_mapSpaceTex;
	KdTexture* m_mapPlayerTex;

	//交差地点にある光の強弱
	float m_lanternPow;
	bool m_lanternFlg;

	//スタート地点のマップ情報
	int m_startMap[7][4] = { {0,0,0,0},
							{0,2,2,2},
							{0,2,2,2},
							{3,2,2,2},
							{0,2,2,2},
							{0,2,2,2},
							{0,0,0,0} };

	//ゲームレベル
	int m_gameLevel;

	//メインマップ
	int m_map[27][26];

	//マップ座標
	Math::Matrix m_startMat[7][4];
	Math::Matrix m_mat[27][26];
	Math::Matrix m_invisibleMat;

	GameScene* m_gameScene;
};
