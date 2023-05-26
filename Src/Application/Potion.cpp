#include"Headers.h"

Potion::Potion(GameScene* _gameScene, Math::Matrix _mat)
{
	m_mat = _mat;
	m_gameScene = _gameScene;
	m_model= MODELMANAGER.GetModel("Data/Item/BluePotion.gltf");

	m_portionCollar = rand() % 3+1;

	if (m_portionCollar == TreasureType::speedPotion)
	{
		m_model = MODELMANAGER.GetModel("Data/Item/BluePotion.gltf");
	}
	if (m_portionCollar == TreasureType::recoveryPotion)
	{
		m_model = MODELMANAGER.GetModel("Data/Item/GreenPotion.gltf");
	}
	if (m_portionCollar == TreasureType::translucentPotion)
	{
		m_model = MODELMANAGER.GetModel("Data/Item/PurplePotion.gltf");
	}

	m_playerSpeedUp = false;

	m_playerDis = 0;

	m_speedUpTime = 5;

	m_animeFrame = 0;
	m_ang = 0;
	m_startMat = m_mat;

	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_mat._41, m_mat._42 + 1, m_mat._43);
	Math::Matrix rotMat = DirectX::XMMatrixRotationRollPitchYaw(
		0,
		DirectX::XMConvertToRadians(180),
		0);
	m_endMat = rotMat * transMat;
}

Potion::~Potion()
{
}

void Potion::Update()
{
	if (m_aliveFlg != true)
	{
		return;
	}

	//������J�����O
	DirectX::BoundingSphere sphere;
	sphere.Radius = 15.5f;
	sphere.Center = m_mat.Translation();

	if (m_gameScene->GetViewArea().Intersects(sphere))
	{
		const Player& player = m_gameScene->GetPlayer();

		Math::Vector3 playerVec = m_mat.Translation() - player.GetPos();

		m_playerDis = playerVec.Length();

		m_treasureType = 0;

		if (player.GetUsePotionFlg() == false)
		{
			const Player& player = m_gameScene->GetPlayer();

			//�X�s�[�h
			if (m_playerDis <= 2 && m_portionCollar == TreasureType::speedPotion)
			{
				m_treasureType = TreasureType::speedPotion;
				m_aliveFlg = false;
			}
			//��
			else if (m_playerDis <= 2 && m_portionCollar == TreasureType::recoveryPotion && player.GetHp() < PlayerStatus::Hp)
			{
				m_treasureType = TreasureType::recoveryPotion;
				m_aliveFlg = false;
			}
			//������
			else if (m_playerDis <= 2 && m_portionCollar == TreasureType::translucentPotion)
			{
				m_treasureType = TreasureType::translucentPotion;

				m_aliveFlg = false;
			}
			else
			{
				m_treasureType = 0;
			}
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

void Potion::Draw()
{
	if (m_aliveFlg != true)
	{
		return;
	}

	//������J�����O
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

void Potion::AnimeProc()
{
	//�N�H�[�^�j�I���p��
	Math::Quaternion startQua;
	Math::Quaternion endQua;
	Math::Quaternion nowQua;

	//�ŏ��ƍŌ�̍s�񂩂�N�I�[�^�j�I�����쐬
	startQua = DirectX::XMQuaternionRotationMatrix(m_startMat);
	endQua = DirectX::XMQuaternionRotationMatrix(m_endMat);

	//���ԍs������߂�(Sleap = �G�X�@���[�v
	nowQua = DirectX::XMQuaternionSlerp(startQua, endQua, m_animeFrame);
	m_mat = DirectX::XMMatrixRotationQuaternion(nowQua);

	//���ԍ��W�����߂�
	Math::Vector3 startPos;
	Math::Vector3 endPos;
	Math::Vector3 nowPos;

	//�ŏ��ƍŌ�̍��W�����߂�
	startPos = m_startMat.Translation();
	endPos = m_endMat.Translation();

	//���ԍ��W�����߂�iLerp���[�v�j
	nowPos = DirectX::XMVectorLerp(startPos, endPos, m_animeFrame);

	//���ԍ��W���s��ɃZ�b�g
	m_mat.Translation(nowPos);
}