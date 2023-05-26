#include"Headers.h"

Player::Player(GameScene* _gameScene)
{
	m_gameScene = _gameScene;

	m_model = MODELMANAGER.GetModel("Data/Player/PlayerBody.gltf");
	m_rightArmModel = MODELMANAGER.GetModel("Data/Player/PlayerRightArm.gltf");
	m_leftArmModel = MODELMANAGER.GetModel("Data/Player/PlayerLeftArm.gltf");
	m_rightLegModel = MODELMANAGER.GetModel("Data/Player/PlayerRighitLeg.gltf");
	m_leftLegModel = MODELMANAGER.GetModel("Data/Player/PlayerLeftLeg.gltf");
	m_cloakModel = MODELMANAGER.GetModel("Data/Player/PlayerCloak.gltf");

	m_hpGreenTex = TEXTUREMANAGER.GetTexture("Data/Player/hpBarGreen.png");
	m_hpRedTex = TEXTUREMANAGER.GetTexture("Data/Player/hpBarRed.png");
	m_hpBarTex = TEXTUREMANAGER.GetTexture("Data/Player/hpBar.png");

	m_pos = { 90, 0, -20 };
	m_mat = DirectX::XMMatrixTranslation(m_pos.x, 0, -m_pos.z);
	m_rightArmMat = DirectX::XMMatrixTranslation(m_mat._41, m_mat._42 + 2.5f, m_mat._43);
	m_leftArmMat = DirectX::XMMatrixTranslation(m_mat._41, m_mat._42 + 2.5f, m_mat._43);
	m_rightLegMat = DirectX::XMMatrixTranslation(m_mat._41, m_mat._42 + 1.3f, m_mat._43);
	m_leftLegMat = DirectX::XMMatrixTranslation(m_mat._41, m_mat._42 + 1.3f, m_mat._43);
	m_cloakMat = DirectX::XMMatrixTranslation(m_mat._41, m_mat._42 + 2.6f, m_mat._43 - 0.34f);

	m_dissolveTex.Load("Data/Player/dissolve.png");
	D3D.GetDevContext()->PSSetShaderResources(4, 1, m_dissolveTex.GetSRViewAddress());

	m_damageSound = std::make_shared<KdSoundEffect>();
	m_damageSound->Load("Data/Player/damageSound.WAV");
	m_damageInst = m_damageSound->CreateInstance(false);
	m_damageInst->SetVolume(0.1f);

	m_walkSound = std::make_shared<KdSoundEffect>();
	m_walkSound->Load("Data/Player/walkSound.WAV");
	m_walkInst = m_walkSound->CreateInstance(false);
	m_walkInst->SetVolume(0.1f);

	m_recoverySound = std::make_shared<KdSoundEffect>();
	m_recoverySound->Load("Data/Item/recovery.WAV");
	m_recoveryInst = m_recoverySound->CreateInstance(false);
	m_recoveryInst->SetVolume(0.5f);

	m_toDrinkSound = std::make_shared<KdSoundEffect>();
	m_toDrinkSound->Load("Data/Item/recoveryToDrink.WAV");
	m_toDrinkkInst = m_toDrinkSound->CreateInstance(false);
	m_toDrinkkInst->SetVolume(0.5f);

	m_speedUpSound = std::make_shared<KdSoundEffect>();
	m_speedUpSound->Load("Data/Item/speedUpSound.WAV");
	m_speedUpInst = m_speedUpSound->CreateInstance(false);
	m_speedUpInst->SetVolume(0.5f);

	m_translucentBgm = std::make_shared<KdSoundEffect>();
	m_translucentBgm->Load("Data/Item/translucentBgm.WAV");
	m_translucentBgmInst = m_translucentBgm->CreateInstance(false);
	m_translucentBgmInst->SetVolume(0.5f);

	m_translucentSound = std::make_shared<KdSoundEffect>();
	m_translucentSound->Load("Data/Item/translucentSound.WAV");
	m_translucentInst = m_translucentSound->CreateInstance(false);
	m_translucentInst->SetVolume(0.3f);

	m_ang = 0;

	m_statusFlg = 0;

	m_walkFlg = false;

	m_limbsAng = 0;
	m_cloakAng = 0;

	m_hp = PlayerStatus::Hp;
	m_initialHp = PlayerStatus::Hp;

	m_usePotion = false;
	m_moveFlg = false;
	m_damageFlg = false;

	m_knockBack = PlayerStatus::knockBackPow;
	m_knockUp = PlayerStatus::knockUpPow;

	m_portionTime = 60 * 5;



	m_movePow = PlayerStatus::movePow;
	m_transparency = 1;

	m_dissolvePow = 0;

	m_camAngle = 70;

	m_vec = { 0,0,1 };
}

Player::~Player()
{
}

void Player::Update(float _camAngY)
{
	m_camAngY = _camAngY;
	m_vec = { 0,0,1 };
	m_moveFlg = false;

	//���������i�ړ��L�[
	MoveKey();

	//�����Ă�����
	if (m_moveFlg == true)
	{
		//�����Ă������
		NowLook();
	}
	//�葫�̓����i�}���g��
	Animation();

	//�|�[�V�����g�p
	UsePotion();

	//PlayerHp
	int oldHp = m_hp;

	const EnemyManager& enemy = m_gameScene->GetEnemyManager();
	//�G����_���[�W�󂯂���HP-
	m_hp -= enemy.GetDamager();

	//�m�b�N�o�b�N_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	if (m_hp < oldHp)
	{
		m_damageInst->Play(false);
		m_damageFlg = true;
	}
	if (m_damageFlg == true)
	{
		Math::Vector3 enemyVec = enemy.GetNowVec() * m_knockBack;
		m_pos.x += enemyVec.x;
		m_pos.y += m_knockUp;
		m_pos.z += enemyVec.z;
		m_knockBack -= 0.1f;
		m_knockUp -= 0.1f;
		if (m_knockBack <= 0)
		{
			m_knockBack = 0;
		}
		if (m_pos.y <= 0)
		{
			m_pos.y = 0;
			m_knockBack = PlayerStatus::knockBackPow;
			m_knockUp = PlayerStatus::knockUpPow;
			m_damageFlg = false;
		}
	}//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

	//�Ǔ����蔻��_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	const Wall& wall = m_gameScene->GetWall();

	bool upXHit, downXHit, upZHit, downZHit;
	float size = 2, wallSize;
	Math::Matrix wallXMat, wallZMat;
	wall.HitWall(m_pos, size, upXHit, downXHit, upZHit, downZHit, wallXMat, wallZMat, wallSize);

	if (upXHit == true)
	{
		m_pos.x = (wallXMat._41 - wallSize) - size;
	}
	if (downXHit == true)
	{
		m_pos.x = (wallXMat._41 + wallSize) + size;
	}
	if (upZHit == true)
	{
		m_pos.z = (wallZMat._43 - wallSize) - size;
	}
	if (downZHit == true)
	{
		m_pos.z = (wallZMat._43 + wallSize) + size;
	}

	//�X�^�[�g���������̕�
	if (m_pos.z < 0)
	{
		Math::Vector3 wallNormal;//���C�����������ʂ̖@��

		bool hit;
		hit = wall.CheckWall(m_pos, m_vec, m_wallDis, wallNormal);

		if (hit)//�i�H��ɏ�Q��������
		{

			Math::Vector3 dot;
			dot = DirectX::XMVector3Dot(wallNormal, -m_vec);

			float limit;
			limit = 3.0f / dot.x;

			if (limit < 0)
			{
				limit *= -1;
			}

			if (m_wallDis < limit)
			{
				m_pos += wallNormal * ((limit - m_wallDis) * dot.x);
			}
		}
	}
	Math::Matrix rotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_ang));
	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_mat = rotMat * transMat;

	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

	//�G�ƃv���C���[�̓����蔻��_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	const EnemyManager& enemyMan = m_gameScene->GetEnemyManager();
	if (enemy.GetPlayerHitFlg() == true)
	{
		m_mat._41 += enemyMan.GetPlayerHitVec().x;
		m_mat._43 += enemyMan.GetPlayerHitVec().z;
	}
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

	//����
	Math::Matrix transArmMat;
	Math::Matrix rotRArmMat;
	transArmMat = DirectX::XMMatrixTranslation(0, 2.5f, 0);
	rotRArmMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_limbsAng));
	m_rightArmMat = rotRArmMat * transArmMat * m_mat;

	Math::Matrix rotLArmMat;
	rotLArmMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-m_limbsAng));
	m_leftArmMat = rotLArmMat * transArmMat * m_mat;

	Math::Matrix transLegMat;
	Math::Matrix rotRLegMat;
	transLegMat = DirectX::XMMatrixTranslation(0, 1.3f, 0);
	rotRLegMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-m_limbsAng));
	m_rightLegMat = rotRLegMat * transLegMat * m_mat;

	Math::Matrix rotLLegMat;
	rotLLegMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_limbsAng));
	m_leftLegMat = rotLLegMat * transLegMat * m_mat;

	Math::Matrix transCloakMat;
	Math::Matrix rotCloakMat;
	transCloakMat = DirectX::XMMatrixTranslation(0, 2.6f, -0.34f);
	rotCloakMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_cloakAng));
	m_cloakMat = rotCloakMat * transCloakMat * m_mat;
}



void Player::Draw()
{

	//�������|�[�V�����g�p��
	if (m_statusFlg & 1 << TreasureType::translucentPotion)
	{
		if (m_portionTime >= 250)
		{
			m_transparency -= 0.02f;
		}
		else if (m_portionTime < 50)
		{
			m_transparency += 0.02f;
		}
		else
		{
			m_transparency = 0.3f;
		}

		//�������p���f��
		SHADER.m_standardShader.SetWorldMatrix(m_mat);
		SHADER.m_standardShader.DrawModel(m_model, m_transparency);

		SHADER.m_standardShader.SetWorldMatrix(m_rightArmMat);
		SHADER.m_standardShader.DrawModel(m_rightArmModel, m_transparency);

		SHADER.m_standardShader.SetWorldMatrix(m_leftArmMat);
		SHADER.m_standardShader.DrawModel(m_leftArmModel, m_transparency);

		SHADER.m_standardShader.SetWorldMatrix(m_rightLegMat);
		SHADER.m_standardShader.DrawModel(m_rightLegModel, m_transparency);

		SHADER.m_standardShader.SetWorldMatrix(m_leftLegMat);
		SHADER.m_standardShader.DrawModel(m_leftLegModel, m_transparency);

		SHADER.m_standardShader.SetWorldMatrix(m_cloakMat);
		SHADER.m_standardShader.DrawModel(m_cloakModel, m_transparency);
	}

	//�f�B�]���u���ʏ�\���p���f��
	if (m_transparency > 0)
	{
		Math::Vector3 damageRGB = { 1,1,1 };
		if (m_damageFlg == true)
		{
			damageRGB = { 1,0,0 };
		}

		SHADER.m_standardShader.SetDissolveEnable(true);

		SHADER.m_standardShader.SetWorldMatrix(m_mat);
		SHADER.m_standardShader.DrawModel(m_model, 1, m_damageFlg, damageRGB);

		SHADER.m_standardShader.SetWorldMatrix(m_rightArmMat);
		SHADER.m_standardShader.DrawModel(m_rightArmModel, 1, m_damageFlg, damageRGB);

		SHADER.m_standardShader.SetWorldMatrix(m_leftArmMat);
		SHADER.m_standardShader.DrawModel(m_leftArmModel, 1, m_damageFlg, damageRGB);

		SHADER.m_standardShader.SetWorldMatrix(m_rightLegMat);
		SHADER.m_standardShader.DrawModel(m_rightLegModel, 1, m_damageFlg, damageRGB);

		SHADER.m_standardShader.SetWorldMatrix(m_leftLegMat);
		SHADER.m_standardShader.DrawModel(m_leftLegModel, 1, m_damageFlg, damageRGB);

		SHADER.m_standardShader.SetWorldMatrix(m_cloakMat);
		SHADER.m_standardShader.DrawModel(m_cloakModel, 1, m_damageFlg, damageRGB);

		SHADER.m_standardShader.SetDissolveEnable(false);
	}
}

void Player::HpBarDraw()
{
	float a = m_initialHp / 2.0f;
	float b = m_initialHp / 2.0f;

	float scaling = m_hp / a;
	float trans = 200-(200 - (100 * scaling));


	SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(-400, -300, 0));
	SHADER.m_spriteShader.DrawTex(m_hpBarTex, Math::Rectangle(0, 0, 460, 60));

	SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixScaling(scaling, 1, 0) * DirectX::XMMatrixTranslation(trans - 575, -300, 0));
	SHADER.m_spriteShader.DrawTex(m_hpGreenTex, Math::Rectangle(0, 0, 200, 50));

}

const void Player::UsePotion()
{
	const TreasureManager& treMan = m_gameScene->GetTreMan();

	//�|�[�V�����g���Ă��Ȃ�������
	if ((!(m_statusFlg & 1 << TreasureType::speedPotion)) &&
		(!(m_statusFlg & 1 << TreasureType::recoveryPotion)) &&
		(!(m_statusFlg & 1 << TreasureType::translucentPotion)))
	{
		m_usePotion = false;
	}
	else//�g�p��
	{
		m_usePotion = true;
	}
	//���̃|�[�V�������g�p���͎��Ȃ�
	//�X�s�[�h
	if (treMan.GetTreasureType() == TreasureType::speedPotion && m_usePotion == false)
	{
		m_speedUpInst->Play(false);
		m_portionTime = 60 * PlayerStatus::speedTime;
		m_statusFlg |= (1 << TreasureType::speedPotion);
		m_usePotion = true;
	}

	//��
	if ((treMan.GetTreasureType() == TreasureType::recoveryPotion) && (m_usePotion == false) && (m_hp < PlayerStatus::Hp))
	{
		if (m_hp < PlayerStatus::Hp)
		{
			m_toDrinkkInst->Play(false);
			m_portionTime = 60 * PlayerStatus::recoveryTime;
			m_hp += 3;
			m_statusFlg |= (1 << TreasureType::recoveryPotion);
			if (m_hp > PlayerStatus::Hp)
			{
				m_hp = PlayerStatus::Hp;
			}
			m_usePotion = true;
		}
	}

	//������
	if (treMan.GetTreasureType() == TreasureType::translucentPotion && m_usePotion == false)
	{
		m_translucentInst->Play(false);
		m_translucentBgmInst->Play(true);
		m_portionTime = 60 * PlayerStatus::translucentTime;
		m_statusFlg |= (1 << TreasureType::translucentPotion);
		m_usePotion = true;
	}

	PotionEffectManager* potionEffectMan = m_gameScene->GetPotionEffectManager();
	//�X�s�[�h
	if (m_statusFlg & 1 << TreasureType::speedPotion)
	{
		m_movePow = PlayerStatus::speedMovePow;
		m_walkInst->SetPitch(0.5f);
		m_camAngle++;
		if (m_camAngle >= 90)
		{
			m_camAngle = 90;
		}
		if (m_moveFlg == true)
		{
			for (int i = 0; i < 2; i++)
			{
				potionEffectMan->Add(new PotionEffect(m_mat, TreasureType::speedPotion));
			}
		}

		if (m_portionTime <= 0)
		{
			m_walkInst->SetPitch(0);
			m_movePow = PlayerStatus::movePow;
			m_statusFlg &= ~(1 << TreasureType::speedPotion);
		}
		--m_portionTime;
	}
	else if (m_camAngle > 70)
	{
		m_camAngle--;
		if (m_camAngle <= 70)
		{
			m_camAngle = 70;
		}
	}

	//��
	if (m_statusFlg & 1 << TreasureType::recoveryPotion)
	{
		if (m_portionTime == 30)
		{
			m_recoveryInst->Play(false);

			//�񕜎��̃L���L���̐�
			int effectNum = 5;
			for (int i = 0; i < effectNum; i++)
			{
				potionEffectMan->Add(new PotionEffect(m_mat, TreasureType::recoveryPotion));
			}
		}
		if (m_portionTime <= 0)
		{
			m_statusFlg &= ~(1 << TreasureType::recoveryPotion);
		}
		--m_portionTime;
	}

	//������
	if (m_statusFlg & 1 << TreasureType::translucentPotion)
	{
		//�������X�^�[�g
		if (m_portionTime >= 250)
		{
			m_dissolvePow += 0.02f;
			if (m_dissolvePow > 1)
			{
				m_dissolvePow = 1;
			}
		}
		//�������I��
		if (m_portionTime < 50)
		{
			m_dissolvePow -= 0.02f;
			if (m_dissolvePow < 0)
			{
				m_dissolvePow = 0;
			}
		}

		SHADER.m_standardShader.SetDissolvePower(m_dissolvePow);

		m_transparency = 0.3f;
		if (m_portionTime <= 0)
		{
			m_translucentBgmInst->Stop();
			m_transparency = 1;
			m_statusFlg &= ~(1 << TreasureType::translucentPotion);
		}
		--m_portionTime;
	}
	potionEffectMan->Update(m_mat);
}

const void Player::Animation()
{
	if (m_moveFlg == true)
	{
		//�葫��������]�X�s�[�h
		if (m_walkFlg == false)
		{
			if (m_statusFlg & 1 << TreasureType::speedPotion)
			{
				m_limbsAng += 6;
			}
			else
			{
				m_limbsAng += 4;
			}

			if (m_limbsAng >= 60)
			{
				//����
				m_walkInst->Play(false);

				m_walkFlg = true;
			}
		}
		if (m_walkFlg == true)
		{
			if (m_statusFlg & 1 << TreasureType::speedPotion)
			{
				m_limbsAng -= 6;
			}
			if (!(m_statusFlg & 1 << TreasureType::speedPotion))
			{
				m_limbsAng -= 4;
			}

			if (m_limbsAng <= -60)
			{
				m_walkFlg = false;
			}
		}
		//�}���g�̓���
		m_cloakAng += 2;
		if (m_cloakAng >= 60)
		{
			m_cloakAng = 60;
		}
	}

	if (m_moveFlg == false)
	{
		if (m_limbsAng < 0)
		{
			m_limbsAng += 5;
		}
		if (m_limbsAng > 0)
		{
			m_limbsAng -= 5;
		}
		//�}���g�̓���
		m_cloakAng -= 2;
		if (m_cloakAng < 0)
		{
			m_cloakAng = 0;
		}
	}
}

const void Player::MoveKey()
{
	Math::Matrix rotMat;
	rotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));

	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, m_movePow), rotMat);
		m_moveFlg = true;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, -m_movePow), rotMat);
		m_moveFlg = true;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_vec = DirectX::XMVector3TransformNormal(Math::Vector3(m_movePow, 0, 0), rotMat);
		m_moveFlg = true;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_vec = DirectX::XMVector3TransformNormal(Math::Vector3(-m_movePow, 0, 0), rotMat);
		m_moveFlg = true;
	}
}

const void Player::NowLook()
{
	m_vec.Normalize();
	m_vec *= m_movePow;

	m_pos = m_mat.Translation();

	m_pos += m_vec;
	//���̃L�����������Ă������
	Math::Matrix nowRotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_ang));
	Math::Vector3 nowVec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), nowRotMat);

	//������������
	Math::Vector3 toVec = m_vec;
	toVec.Normalize();

	Math::Vector3 dot = DirectX::XMVector3Dot(nowVec, toVec);

	if (dot.x > 1)
	{
		dot.x = 1.0f;
	}
	if (dot.x < -1)
	{
		dot.x = -1.0f;
	}

	//�p�x���Ƃ�
	float ang = DirectX::XMConvertToDegrees(acos(dot.x));

	//�����ł��ړ������i�p�x�j���ς������
	if (ang >= 0.1f)
	{
		if (ang > 5)
		{
			ang = 5.0f;
		}

		//�O��(�ǂ����ɉ�]���邩)
		Math::Vector3 cross = DirectX::XMVector3Cross(toVec, nowVec);
		cross.Normalize();

		if (cross.y >= 0)
		{
			m_ang -= ang;
		}
		else
		{
			m_ang += ang;
		}
	}
}

