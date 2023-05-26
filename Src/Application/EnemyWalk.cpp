#include"Application/Headers.h"

EnemyWalk::EnemyWalk(Enemy* _enemy, GameScene* _gameScene)
{
	m_enemy = _enemy;
	m_currentspot = {};
	m_nextspotMat = m_enemy->GetSpotMat();
	m_mapX = 0;
	m_mapZ = 0;
	m_direction = 0;
	m_stopTime = 3.0f * 60.0f;
	m_moveFlg = true;

	m_whistleSound = std::make_shared<KdSoundEffect>();
	m_whistleSound->Load("Data/Enemy/SE/whistle.WAV");
	m_whistleInst = m_whistleSound->CreateInstance(true);
	//true�ɂ����3D�T�E���h
	//3D�T�E���h�̃p�����[�^���Z�b�g
	m_whistleInst->SetCurveDistanceScaler(1.0f);//������
	m_whistleInst->SetPitch(0.5f);
	m_whistleInst->SetVolume(1.0f);

	m_jump = 1.0f;
	m_discoverFlg = false;
}


BaseEnemy* EnemyWalk::Action(GameScene* _gameScene, Enemy* _enemy)
{
	Math::Matrix mat = _enemy->GetMat();
	Math::Vector3 vec = _enemy->GetVec();

	_enemy->SetAttackFlg(false);
	//�G�X�s�[�h_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	_enemy->SetMovePow(EnemyStatus::movePow);

	//�G�����x�N�g��_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), mat);
	_enemy->SetVec(vec);

	//�v���C���[����_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	if (LookPlayer(_gameScene, mat, vec) == true)
	{
		if (m_discoverFlg == false)
		{
			m_stopTime = 0.5f * 60.0f;
			m_whistleInst->Play3D(mat.Translation(), false);
		}
		m_discoverFlg = true;
	}
	if (m_discoverFlg == true)
	{
		const Player& player = _gameScene->GetPlayer();

		Math::Vector3 targetVec;
		targetVec = player.GetMat().Translation() - mat.Translation();

		_enemy->SetRotAngY(m_enemy->RotationAngle(vec, targetVec));

		_enemy->SetMovePow(0);

		m_jump -= 0.1f;

		mat._42 += m_jump;

		if (mat._42 <= 0)
		{
			mat._42 = 0;

			m_stopTime--;
			if (m_stopTime <= 0)
			{
				return new EnemyTrack(m_enemy);
			}

		}
	}


	//�G����_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

	Math::Vector3 nextVec = m_nextspotMat.Translation() - mat.Translation();

	float dis = nextVec.Length();

	int mapX = ((int)mat._41 + 5) / 10;
	int mapZ = ((int)mat._43 + 5) / 10;

	APP.m_console.AddLog("%d::%d", mapX, mapZ);
	//���̖ړI�n
	if (dis < 2.0f && m_moveFlg == true)
	{
		m_moveFlg = false;
		m_stopTime = 3.0f * 60.0f;
		m_nextspotMat = NextSpot(_gameScene, mat);
	}

	if (m_moveFlg == false)
	{
		_enemy->SetMovePow(0);
		--m_stopTime;
		//�ړI�n�ɂ����炵�΂炭�~�܂�
		if (m_stopTime <= 0.0f)
		{
			m_moveFlg = true;
			_enemy->SetMovePow(EnemyStatus::movePow);
		}
	}

	if (LookPlayer(_gameScene, mat, vec) == false)
	{
		//���̖ړI�n�֕����]��
		_enemy->SetRotAngY(m_enemy->RotationAngle(vec, nextVec));
	}

	_enemy->SetMat(mat);

	//�X�|�b�g���C�g
	SHADER.AddSpotLighit(mat.Translation() + Math::Vector3(0, 1, 0), vec, 20, DirectX::XMConvertToRadians(30), { 2,2,2 });

	return nullptr;

}

const bool EnemyWalk::LookPlayer(GameScene* _gameScene, const Math::Matrix& _mat, const Math::Vector3& _nowVec)
{
	//��l�������H_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�G���猩����l���ւ̕����x�N�g��
	const Player& player = _gameScene->GetPlayer();

	Math::Vector3 targetVec;
	targetVec = player.GetMat().Translation() - _mat.Translation();

	//�G�̎���
	Math::Vector3 dot = DirectX::XMVector3Dot(_nowVec, targetVec);
	if (dot.x > 1)
	{
		dot.x = 1.0f;
	}
	if (dot.x < -1)
	{
		dot.x = -1.0f;
	}

	float ang = DirectX::XMConvertToDegrees(acos(dot.x));

	//����p
	float viewAng = 90.0;
	float cos = cosf(DirectX::XMConvertToRadians(viewAng / 2.0f));
	//���싗��
	float viewingdis = targetVec.Length();



	if (((viewingdis <= 15 && cos > ang) || viewingdis <= 5) && player.GetTranslucentFlg() == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const Math::Matrix EnemyWalk::NextSpot(GameScene* _gameScene, const Math::Matrix& _mat)
{
	//���̍��W���}�b�v�̍s��֕ϊ�
	int mapX = ((int)_mat._41 + 5) / 10;
	int mapZ = ((int)_mat._43 + 5) / 10;

	int map[4]{};

	int num = 0;
	int route[4]{};
	Math::Matrix nextMat;
	int8_t routeFlg = 0;

	//�Ȃ������
	enum Direction
	{
		DownX,
		DownZ,
		UpX,
		UpZ,
	};

	const Wall& wall = _gameScene->GetWall();

	//4�����ɕǂ����邩����
	map[DownX] = wall.ChecMapGrid(mapX - 1, mapZ);
	map[DownZ] = wall.ChecMapGrid(mapX, mapZ - 1);
	map[UpX] = wall.ChecMapGrid(mapX + 1, mapZ);
	map[UpZ] = wall.ChecMapGrid(mapX, mapZ + 1);

	//�ǂ��Ȃ����������ł͂Ȃ�������i�߂铹�Ɣ���
	if (map[DownX] != 0 && m_direction != UpX)
	{
		route[num] = DownX;
		++num;
	}
	if (map[DownZ] != 0 && m_direction != UpZ)
	{
		route[num] = DownZ;
		++num;
	}
	if (map[UpX] != 0 && m_direction != DownX)
	{
		route[num] = UpX;
		++num;
	}
	if (map[UpZ] != 0 && m_direction != DownZ)
	{
		route[num] = UpZ;
		++num;
	}

	//�i�ޓ��������_���őI��
	if (num > 0)
	{
		m_direction = route[rand() % num];
	}

	nextMat = wall.NearDestination(_mat);
	for (int direction = 0; direction < 4; direction++)
	{
		if (m_direction == direction)
		{
			//�����n�_�T��
			for (int i = 1; i < 27; i++)
			{
				int spot;
				//�I�΂ꂽ�����̌����n�_��T��
				if (m_direction == DownX) { spot = wall.ChecMapGrid(mapX - i, mapZ); }
				if (m_direction == DownZ) { spot = wall.ChecMapGrid(mapX, mapZ - i); }
				if (m_direction == UpX) { spot = wall.ChecMapGrid(mapX + i, mapZ); }
				if (m_direction == UpZ) { spot = wall.ChecMapGrid(mapX, mapZ + i); }

				//�����n�_������
				if (spot == 1)
				{
					//���̃X�|�b�g�Z�b�g
					if (m_direction == DownX)
					{
						nextMat = DirectX::XMMatrixTranslation((float)((mapX - i) * 10), 0, (float)(mapZ * 10));
					}
					if (m_direction == DownZ)
					{
						nextMat = DirectX::XMMatrixTranslation((float)(mapX * 10), 0, (float)((mapZ - i) * 10));
					}
					if (m_direction == UpX)
					{
						nextMat = DirectX::XMMatrixTranslation((float)((mapX + i) * 10), 0, (float)(mapZ * 10));
					}
					if (m_direction == UpZ)
					{
						nextMat = DirectX::XMMatrixTranslation((float)(mapX * 10), 0, (float)((mapZ + i) * 10));
					}
					break;
				}
			}
		}
	}

	return nextMat;

}
