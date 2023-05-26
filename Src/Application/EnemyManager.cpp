#include"Application/Headers.h"

EnemyManager::EnemyManager(GameScene* _gameScene)
{
	m_gameScene = _gameScene;

	const Wall& wall = m_gameScene->GetWall();

	Math::Matrix enemyMat[100] = {};

	int enemyNum = 0;

	//���x���ɍ������G�̐�
	if (m_gameScene->GetGameLevel() == 1)
	{
		enemyNum = EnemyStatus::Level1EnemyNum;
	}
	if (m_gameScene->GetGameLevel() == 2)
	{
		enemyNum = EnemyStatus::Level2EnemyNum;
	}
	if (m_gameScene->GetGameLevel() == 3)
	{
		enemyNum = EnemyStatus::Level3EnemyNum;
	}

	//�����X�|�[���ł���ꏊ�̐�
	int spawnPointNum = wall.GetIntersectionNum();
	int enemySetNum[100] = {};

	int num = 0;
	while (1)
	{
		//�X�|�[������ꏊ�������_���Ɍ��߂�
		enemySetNum[num] = rand() % spawnPointNum + 1;
		bool skipFlg = false;
		if (num > 0)
		{
			for (int i = num - 1; i >= 0; i--)
			{
				//���̓G�Ɣ���ĂȂ���
				if (enemySetNum[num] == enemySetNum[i])
				{
					skipFlg = true;
					break;
				}
			}
			//����Ă�����������
			if (skipFlg == true)continue;
		}
		num++;
		if (num >= enemyNum)break;
	}
	for (int i = 0; i < enemyNum; i++)
	{
		Math::Vector3 enemyPos = { 0,0,0 };

		//�X�|�[���n�_�ݒ�
		enemyPos = wall.GetIntersectionPos(enemySetNum[i]);
		enemyMat[i] = DirectX::XMMatrixTranslation(enemyPos.x * 10.0f, 0, enemyPos.z * 10.0f);
		m_enemy.push_back(new Enemy(m_gameScene, enemyMat[i]));
	}

	m_damage = 0;

	m_enemySize = 1.5f;

	m_hitPlayerFlg = false;
}

EnemyManager::~EnemyManager()
{
	for (UINT i = 0; i < m_enemy.size(); i++)
	{
		delete m_enemy[i];
	}
	m_enemy.clear();
}

void EnemyManager::Update()
{
	m_damage = 0;

	m_hitPlayerFlg = false;
	for (UINT i = 0; i < m_enemy.size(); i++)
	{
		m_enemy[i]->Update();

		if (m_enemy[i]->GetDamageFlg() == true)
		{
			//�m�b�N�o�b�N�p�̕���
			m_vec = m_enemy[i]->GetVec();
			//�_���[�W�^����
			++m_damage;
		}

		bool hitFlg = false;
		int hitNum = 0;
		for (UINT j = 0; j < m_enemy.size(); j++)
		{
			if (i == j)continue;
			Math::Vector3 vec = m_enemy[i]->GetMat().Translation() - m_enemy[j]->GetMat().Translation();
			//�G���m�̓����蔻��
			if (vec.Length() < 3)
			{
				hitFlg = true;

				float pushPow = 3 - vec.Length();
				vec.Normalize();
				vec *= pushPow;

				vec = vec * 0.5f;

				m_enemy[i]->SetHitVec(vec, hitNum);

				hitNum++;
			}
		}
		m_enemy[i]->SetHitFlg(hitFlg);

		hitNum = 0;
		const Player& player = m_gameScene->GetPlayer();
		Math::Vector3 vec = m_enemy[i]->GetMat().Translation() - player.GetMat().Translation();
		//�G�ƃv���C���[�̓����蔻��
		if (vec.Length() < 3)
		{
			m_hitPlayerFlg = true;

			float pushPow = 3 - vec.Length();
			vec.Normalize();
			vec *= pushPow;

			m_enemy[i]->SetHitVec(vec * 0.8f, hitNum);
			m_hitPlayer = (-(vec * 0.2f));
			hitNum++;
		}
	}
}

void EnemyManager::Draw()
{
	m_gameScene;
	for (UINT i = 0; i < m_enemy.size(); i++)
	{
		m_enemy[i]->Draw();
	}

}