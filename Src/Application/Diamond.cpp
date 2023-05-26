#include"Headers.h"

Diamond::Diamond(GameScene* _gameScene,Math::Matrix _mat)
{
	m_mat = _mat;
	m_gameScene = _gameScene;
	m_model=MODELMANAGER.GetModel("Data/diamond/diamond.gltf");

	m_animeFrame = 0;
	m_ang = 0;
	m_startMat = m_mat;

	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_mat._41, m_mat._42 + 1, m_mat._43);
	Math::Matrix rotMat = DirectX::XMMatrixRotationRollPitchYaw(
		0,
		DirectX::XMConvertToRadians(180),
		0);
	m_endMat = rotMat * transMat;

	m_playerDis = 0;
}

Diamond::~Diamond()
{
}

void Diamond::Update()
{
	if (m_aliveFlg != true)
	{
		return;
	}
	DirectX::BoundingSphere sphere;
	sphere.Radius = 15.0f;

	sphere.Center = m_mat.Translation();
	if (m_gameScene->GetViewArea().Intersects(sphere))
	{
		const Player& player = m_gameScene->GetPlayer();

		Math::Vector3 playerVec = m_mat.Translation() - player.GetPos();

		m_playerDis = playerVec.Length();

		m_treasureType = 0;

		if (m_playerDis <= 2)
		{
			m_treasureType = TreasureType::diamond;
			m_aliveFlg = false;
		}

		m_animeFrame += 0.05f;
		if (m_animeFrame > 1)
		{
			m_animeFrame = 1.0f;
		}
		if (m_animeFrame < 0.2f)
		{
			m_animeFrame = 0.2f;
		}


		AnimeProc();

		m_animeFrame = (sin(DirectX::XMConvertToRadians(m_ang)) + 1) / 2;
		m_ang++;
		if (m_ang > 360)
		{
			m_ang -= 360;
			AnimeProc();
		}
	}
}

void Diamond::Draw()
{
	if (m_aliveFlg != true)
	{
		return;
	}

	DirectX::BoundingSphere sphere;
	sphere.Radius = 15.0f;

	sphere.Center = m_mat.Translation();
	if (m_gameScene->GetViewArea().Intersects(sphere))
	{
		SHADER.m_standardShader.SetWorldMatrix(m_mat);
		SHADER.m_standardShader.DrawModel(m_model);

		if (m_playerDis <= 70)
		{
			SHADER.AddPointLight(Math::Vector3{ 4, 4,6 }, Math::Vector3{ m_mat._41,m_animeFrame + 2.0f,m_mat._43 }, (m_animeFrame + 1.0f) * 3.0f);
		}
	}
}

void Diamond::AnimeProc()
{
	//クォータニオン用意
	Math::Quaternion startQua;
	Math::Quaternion endQua;
	Math::Quaternion nowQua;

	//最初と最後の行列からクオータニオンを作成
	startQua = DirectX::XMQuaternionRotationMatrix(m_startMat);
	endQua = DirectX::XMQuaternionRotationMatrix(m_endMat);

	//中間行列を求める(Sleap = エス　ラープ
	nowQua = DirectX::XMQuaternionSlerp(startQua, endQua, m_animeFrame);
	m_mat = DirectX::XMMatrixRotationQuaternion(nowQua);

	//中間座標を求める
	Math::Vector3 startPos;
	Math::Vector3 endPos;
	Math::Vector3 nowPos;

	//最初と最後の座標を求める
	startPos = m_startMat.Translation();
	endPos = m_endMat.Translation();

	//中間座標を求める（Lerpラープ）
	nowPos = DirectX::XMVectorLerp(startPos, endPos, m_animeFrame);

	//中間座標を行列にセット
	m_mat.Translation(nowPos);
}
