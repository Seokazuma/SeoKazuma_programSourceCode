#include"Headers.h"

bool ModelIntersects(const KdModel& _model, const Math::Matrix& _mat, const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec);

Wall::Wall(GameScene* _gameScene)
{
	m_gameScene = _gameScene;

	m_model[Map::wall] = MODELMANAGER.GetModel("Data/Wall/blockWall1.gltf");
	m_model[Map::intersection] = MODELMANAGER.GetModel("Data/Wall/blockArch1.gltf");
	m_model[Map::space] = MODELMANAGER.GetModel("Data/Wall/block1.gltf");
	m_model[Map::door] = MODELMANAGER.GetModel("Data/Wall/blockDoor.gltf");
	m_invisibleModel = MODELMANAGER.GetModel("Data/Wall/blockStart_invisible.gltf");

	m_mapWallTex = TEXTUREMANAGER.GetTexture("Data/Wall/mapWall.png");
	m_mapSpaceTex = TEXTUREMANAGER.GetTexture("Data/Wall/mapSpace.png");
	m_mapPlayerTex = TEXTUREMANAGER.GetTexture("Data/Wall/mapPlayerPoint.png");

	//マップ座標設定
	for (int x = 0; x < 27; x++)
	{
		for (int z = 0; z < 26; z++)
		{
			m_mat[x][z] = DirectX::XMMatrixTranslation(x * 10.0f, 0, z * 10.0f);
		}
	}
	for (int x = 0; x < 7; x++)
	{
		for (int z = 0; z < 4; z++)
		{
			m_startMat[x][z] = DirectX::XMMatrixTranslation(x * 10.0f + 60.0f, 0, z * 10.0f - 40.0f);
		}
	}

	m_lanternPow = 10;
	m_lanternFlg = false;

	m_gameLevel = m_gameScene->GetGameLevel();

	//マップ読み込み
	for (int x = 0; x < 27; x++)
	{
		for (int z = 0; z < 26; z++)
		{
			//レベルに合ったマップ生成
			if (m_gameLevel == 1)
			{
				m_map[x][z] = Map::level1Map[x][z];
			}
			else if (m_gameLevel == 2)
			{
				m_map[x][z] = Map::level2Map[x][z];
			}
			else if (m_gameLevel == 3)
			{
				m_map[x][z] = Map::level3Map[x][z];
			}
			else
			{
				m_map[x][z] = 1;
			}
		}
	}
}

Wall::~Wall()
{
}

void Wall::Update()
{
	//ランタンの光表現
	if (m_lanternFlg == false)
	{
		m_lanternPow += 0.05f;
		if (m_lanternPow > 10)
		{
			m_lanternFlg = true;
		}
	}
	if (m_lanternFlg == true)
	{
		m_lanternPow -= 0.05f;
		if (m_lanternPow < 5)
		{
			m_lanternFlg = false;
		}
	}

}

void Wall::Draw()
{

	DirectX::BoundingSphere sphere;
	sphere.Radius = 30.0f;
	Math::Vector3 camPos = m_gameScene->GetCamPos();
	for (int x = 0; x < 27; x++)
	{
		for (int z = 0; z < 26; z++)
		{
			sphere.Center = m_mat[x][z].Translation();
			if (m_gameScene->GetViewArea().Intersects(sphere))
			{
				SHADER.m_standardShader.SetWorldMatrix(m_mat[x][z]);
				SHADER.m_standardShader.DrawModel(m_model[m_map[x][z]]);
				if (m_map[x][z] == Map::intersection)//ランタン
				{
					SHADER.AddPointLight(Math::Vector3{ 2.5,2,1 }, m_mat[x][z].Translation() + Math::Vector3(0, 10, 0), m_lanternPow);
				}

			}
		}
	}
	for (int x = 0; x < 7; x++)
	{
		for (int z = 0; z < 4; z++)
		{
			sphere.Center = m_startMat[x][z].Translation();
			if (m_gameScene->GetViewArea().Intersects(sphere))
			{
				SHADER.m_standardShader.SetWorldMatrix(m_startMat[x][z]);
				SHADER.m_standardShader.DrawModel(m_model[m_startMap[x][z]]);
				if (m_startMap[x][z] == Map::intersection)//ランタン
				{
					SHADER.AddPointLight(Math::Vector3{ 2.5,2,1 }, m_startMat[x][z].Translation() + Math::Vector3(0, 10, 0), m_lanternPow);
				}
			}
		}
	}
	//スタート地点のコリジョンモデル
	SHADER.m_standardShader.SetWorldMatrix(m_startMat[3][3]);
	SHADER.m_standardShader.DrawModel(m_invisibleModel, 0);

}

void Wall::MapTexDraw()
{
	int mapNum = 0;
	for (int x = 0; x < 27; x++)
	{
		for (int z = 0; z < 26; z++)
		{
			if (m_map[x][z] == Map::wall)
			{
				SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(-200.0f + ((float)x * 20.0f), -200.0f + ((float)z * 20.0f), 0));
				SHADER.m_spriteShader.DrawTex(m_mapWallTex, Math::Rectangle(0, 0, 20, 20), 0.5f);
			}
			else
			{
				SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(-200.0f + ((float)x * 20.0f), -200.0f + ((float)z * 20.0f), 0));
				SHADER.m_spriteShader.DrawTex(m_mapSpaceTex, Math::Rectangle(0, 0, 20, 20), 0.5f);
			}
			mapNum++;
		}
	}
	for (int x = 0; x < 7; x++)
	{
		for (int z = 0; z < 4; z++)
		{
			if (m_startMap[x][z] == Map::wall)
			{
				SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(-80.0f + ((float)x * 20.0f), -280.0f + ((float)z * 20.0f), 0));
				SHADER.m_spriteShader.DrawTex(m_mapWallTex, Math::Rectangle(0, 0, 20, 20), 0.5f);
			}
			else
			{
				SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(-80.0f + ((float)x * 20.0f), -280.0f + ((float)z * 20.0f), 0));
				SHADER.m_spriteShader.DrawTex(m_mapSpaceTex, Math::Rectangle(0, 0, 20, 20), 0.5f);
			}
			mapNum++;
		}
	}
	const Player& player = m_gameScene->GetPlayer();

	SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(-200.0f + ((float)player.GetPos().x * 2.0f), -200.0f + ((float)player.GetPos().z * 2.0f), 0));
	SHADER.m_spriteShader.DrawTex(m_mapPlayerTex, Math::Rectangle(0, 0, 20, 20));

}

const void Wall::HitWall(Math::Vector3& _pos, float& _size, bool& _upXHit, bool& _downXHit, bool& _upZHit, bool& _downZHit, Math::Matrix& _wallXMat, Math::Matrix& _wallZMat, float& _wallsize)const
{
	//壁の大きさ
	_wallsize = 5;
	//送られてきた座標をマップ行列に変換
	int nowMapX = (((int)_pos.x + 5) / 10);
	int nowMapZ = (((int)_pos.z + 5) / 10);

	float sizeSum;
	Math::Vector3 dis;

	//前後左右が壁かどうか
	if (m_map[nowMapX + 1][nowMapZ] == Map::wall)
	{
		CollisionDetection(m_mat[nowMapX + 1][nowMapZ], _pos, _size, _wallsize, dis, sizeSum, _upXHit, _wallXMat);
	}
	else
	{
		_upXHit = false;
	}
	if (m_map[nowMapX - 1][nowMapZ] == Map::wall)
	{
		CollisionDetection(m_mat[nowMapX - 1][nowMapZ], _pos, _size, _wallsize, dis, sizeSum, _downXHit, _wallXMat);

	}
	else
	{
		_downXHit = false;
	}
	if (m_map[nowMapX][nowMapZ + 1] == Map::wall)
	{
		CollisionDetection(m_mat[nowMapX][nowMapZ + 1], _pos, _size, _wallsize, dis, sizeSum, _upZHit, _wallZMat);
	}
	else
	{
		_upZHit = false;
	}
	if (m_map[nowMapX][nowMapZ - 1] == Map::wall)
	{
		CollisionDetection(m_mat[nowMapX][nowMapZ - 1], _pos, _size, _wallsize, dis, sizeSum, _downZHit, _wallZMat);
	}
	else
	{
		_downZHit = false;
	}

}

const void Wall::CollisionDetection(Math::Matrix _mat, Math::Vector3& _pos, float& _size, float& _wallsize, Math::Vector3& _dis, float& _sizeSum, bool& _hit, Math::Matrix& _wallMat) const
{
	//壁との距離
	_dis = { _mat._41 - _pos.x,0,_mat._43 - _pos.z };
	if (_dis.x < 0)
	{
		_dis.x *= -1;
	}
	if (_dis.z < 0)
	{
		_dis.z *= -1;
	}
	//プレイヤーと壁のサイズ合成
	_sizeSum = _size + _wallsize;

	//当たった
	if (_dis.x < _sizeSum && _dis.z < _sizeSum)
	{
		_hit = true;
		_wallMat = _mat;
	}
	else
	{
		_hit = false;
	}
}

const int Wall::ChecMapGrid(const int& _x, const int& _z) const
{
	//スタート地点進入防止
	if (_x == 9 && _z == 0)
	{
		return Map::wall;
	}
	return m_map[_x][_z];
}

const Math::Matrix Wall::NearDestination(const Math::Matrix& _mat)const
{
	//送られてきた座標をマップ行列に変換
	int mapX = (int)_mat._41 / 10;
	int mapZ = (int)_mat._43 / 10;

	float minDis = 1000;
	Math::Matrix nearMat;

	int intersectionNum = GetIntersectionNum();

	for (int i = 0; i < intersectionNum; i++)
	{
		Math::Vector3 intersection = GetIntersectionPos(i);
		//交差地点以外ははじく
		if (m_map[(int)intersection.x][(int)intersection.z] != Map::intersection)continue;
		//送られてきた座標と交差地点の距離
		Math::Vector3 destinationVec = m_mat[(int)intersection.x][(int)intersection.z].Translation() - _mat.Translation();
		float destinationDis = destinationVec.Length();

		//一番近い距離入れる
		if (minDis >= destinationDis)
		{
			minDis = destinationDis;
			nearMat = m_mat[(int)intersection.x][(int)intersection.z];
		}
	}
	return nearMat;
}

const bool Wall::CheckWall(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec) const
{
	bool hit;
	hit = ModelIntersects(*m_invisibleModel, m_startMat[3][3], _rayPos, _rayVec, _dis, _normalVec);
	return hit;
}

const int Wall::GetIntersectionNum() const
{
	int intersectionNum = 0;
	Math::Vector3 intersectionPos[100] = {};
	for (int x = 0; x < 27; x++)
	{
		for (int z = 0; z < 26; z++)
		{
			//交差地点だったら
			if (m_map[x][z] == Map::intersection)
			{
				//交差地点数える
				intersectionNum++;
			}
		}
	}
	return intersectionNum;
}

const Math::Vector3 Wall::GetIntersectionPos(const int& _intersectionNum) const
{
	int intersectionNum = 0;
	Math::Vector3 intersectionPos = {};

	bool flg = true;
	for (int x = 0; x < 27; x++)
	{
		for (int z = 0; z < 26; z++)
		{
			if (m_map[x][z] == Map::intersection)
			{
				intersectionNum++;
			}
			//for文で調べている交差地点番号と送られてきた交差地点番号が一致したら
			if (intersectionNum == _intersectionNum)
			{
				//一致した座標を入れる
				intersectionPos = { (float)x,0,(float)z };
				flg = false;
				break;
			}
		}
		if (flg == false)break;
	}

	return intersectionPos;
}
