#include"Application/Headers.h"

Enemy::Enemy(GameScene* _gameScene, const Math::Matrix _mat)
{
	m_gameScene = _gameScene;
	m_model = MODELMANAGER.GetModel("Data/Enemy/EnemyBody.gltf");
	m_rightArmModel = MODELMANAGER.GetModel("Data/Enemy/EnemyRightArm.gltf");
	m_leftArmModel = MODELMANAGER.GetModel("Data/Enemy/EnemyLeftArm.gltf");
	m_rightLegModel = MODELMANAGER.GetModel("Data/Enemy/EnemyRightLeg.gltf");
	m_leftLegModel = MODELMANAGER.GetModel("Data/Enemy/EnemyLeftLeg.gltf");

	m_mat = _mat;
	m_spotMat = _mat;

	m_rightArmMat = DirectX::XMMatrixTranslation(m_mat._41, m_mat._42 + 2.5f, m_mat._43);
	m_leftArmMat = DirectX::XMMatrixTranslation(m_mat._41, m_mat._42 + 2.5f, m_mat._43);
	m_rightLegMat = DirectX::XMMatrixTranslation(m_mat._41, m_mat._42 + 2.5f, m_mat._43);
	m_leftLegMat = DirectX::XMMatrixTranslation(m_mat._41, m_mat._42 + 2.5f, m_mat._43);


	m_currentState = new EnemyWalk(this,_gameScene);

	m_rotAngY = 0;
	m_movePow = 0;

	m_attackFlg = false;
	m_damageFlg = false;
	m_limbsFlg = false;
	m_limbsAng = 0;
	m_rightArmAng = 0;

	//3Dサウンド
	m_walkSound = std::make_shared<KdSoundEffect>();
	m_walkSound->Load("Data/Enemy/SE/walkEnemy.WAV");
	m_walkInst = m_walkSound->CreateInstance(true);
	//3Dサウンドのパラメータリセット
	m_walkInst->SetCurveDistanceScaler(1.0f);
	m_walkInst->SetPitch(0.3f);
	m_walkInst->SetVolume(1.0f);

	m_hitNum = 0;


}

Enemy::~Enemy()
{
	if (m_currentState != nullptr)
	{
		delete m_currentState;
	}
}

void Enemy::Update()
{
	//ステートをセット
	if (m_currentState != nullptr)
	{
		BaseEnemy* nextState;
		nextState = m_currentState->Action(m_gameScene,this);

		if (nextState != nullptr)
		{
			delete m_currentState;

			m_currentState = nextState;
		}
	}

	//当たり判定
	CollisionDetection();

	//手足のモーション
	Motion();

	const Player& player = m_gameScene->GetPlayer();

	//位置
	AUDIO.GetListener().SetPosition(player.GetPos());
	//向き
	AUDIO.GetListener().SetOrientation(
		SHADER.m_cb7_Camera.Work().mV.Backward(),
		SHADER.m_cb7_Camera.Work().mV.Up()
	);
	m_walkInst->Apply3D();

}

void Enemy::Draw()
{

	DirectX::BoundingSphere sphere;
	sphere.Radius = 15.0f;
	sphere.Center = m_mat.Translation();
	if (m_gameScene->GetViewArea().Intersects(sphere))
	{

		SHADER.m_standardShader.SetWorldMatrix(m_mat);
		SHADER.m_standardShader.DrawModel(m_model);

		SHADER.m_standardShader.SetWorldMatrix(m_rightArmMat);
		SHADER.m_standardShader.DrawModel(m_rightArmModel);

		SHADER.m_standardShader.SetWorldMatrix(m_leftArmMat);
		SHADER.m_standardShader.DrawModel(m_leftArmModel);

		SHADER.m_standardShader.SetWorldMatrix(m_rightLegMat);
		SHADER.m_standardShader.DrawModel(m_rightLegModel);

		SHADER.m_standardShader.SetWorldMatrix(m_leftLegMat);
		SHADER.m_standardShader.DrawModel(m_leftLegModel);
	}
}
const Math::Matrix Enemy::Rotate()const
{
	Math::Matrix rotMat;

	rotMat = DirectX::XMMatrixRotationRollPitchYaw(
		DirectX::XMConvertToRadians(0),
		DirectX::XMConvertToRadians(m_rotAngY),
		DirectX::XMConvertToRadians(0));

	return rotMat;
}

const void Enemy::CollisionDetection()
{
	//壁当たり判定_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	const Wall& wall = m_gameScene->GetWall();

	bool upXHit, downXHit, upZHit, downZHit;
	float size = 2, wallSize;
	Math::Matrix wallXMat, wallZMat;
	wall.HitWall(m_mat.Translation(), size, upXHit, downXHit, upZHit, downZHit, wallXMat, wallZMat, wallSize);

	if (upXHit == true)
	{
		m_mat._41 = (wallXMat._41 - wallSize) - size;
	}
	if (downXHit == true)
	{
		m_mat._41 = (wallXMat._41 + wallSize) + size;
	}
	if (upZHit == true)
	{
		m_mat._43 = (wallZMat._43 - wallSize) - size;
	}
	if (downZHit == true)
	{
		m_mat._43 = (wallZMat._43 + wallSize) + size;
	}
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	Math::Matrix transMat;
	transMat = DirectX::XMMatrixTranslation(0, 0, m_movePow);

	m_mat = Rotate() * transMat * m_mat;

	//敵同士の当たり判定
	if (m_hitFlg == true)
	{
		for (int i = 0; i < m_hitNum + 1; i++)
		{
			Math::Matrix hitMat = DirectX::XMMatrixTranslation(m_hitVec[i].x, 0, m_hitVec[i].z);
			m_mat._41 += hitMat._41;
			m_mat._43 += hitMat._43;
		}
	}
}

const void Enemy::Motion()
{
	const Player& player = m_gameScene->GetPlayer();
	Math::Vector3 vec = player.GetPos() - m_mat.Translation();

	float dis = vec.Length();
	//手足の回転
	if (m_movePow > 0)
	{

		if (m_limbsFlg == 0)
		{
			m_limbsAng += 4;
			if (m_limbsAng >= 60)
			{
				m_limbsFlg = 1;
			}
		}
		if (m_limbsFlg == 1)
		{
			m_limbsAng -= 4;
			if (m_limbsAng <= -60)
			{
				if (dis < 35)
				{
					m_walkInst->Play3D(m_mat.Translation(), false);
				}
				m_limbsFlg = 0;
			}
		}
		if (m_attackFlg == false)
		{
			m_rightArmAng = m_limbsAng;
		}
	}
	if (m_movePow <= 0)
	{
		if (m_limbsAng > 0)
		{
			m_limbsAng -= 4;
		}
		if (m_limbsAng < 0)
		{
			m_limbsAng += 4;
		}
	}
	m_damageFlg = false;
	//攻撃開始
	if (m_attackFlg == true)
	{
		if (m_limbsFlg == 0)
		{
			m_rightArmAng -= 10;
			if (m_rightArmAng <= -180)
			{
				//振り下ろしきったらダメージ与える
				m_damageFlg = true;

				const Player& player = m_gameScene->GetPlayer();
				m_targetMat = player.GetMat();
				//敵から見たプレイヤーへの方向ベクトル
				m_vec = m_targetMat.Translation() - m_mat.Translation();
				m_vec.Normalize();

				m_rightArmAng = -180;
				m_limbsFlg = 1;
			}
		}
		if (m_limbsFlg == 1)
		{
			m_rightArmAng += 30;
			if (m_rightArmAng >= 60)
			{
				m_rightArmAng = 60;
				m_limbsFlg = 0;
			}
		}
	}

	Math::Matrix transArmMat;
	Math::Matrix rotRArmMat;
	transArmMat = DirectX::XMMatrixTranslation(0, 2.5f, 0);
	rotRArmMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_rightArmAng));
	m_rightArmMat = rotRArmMat * transArmMat * m_mat;

	Math::Matrix rotLArmMat;
	rotLArmMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-90));
	m_leftArmMat = rotLArmMat * transArmMat * m_mat;

	Math::Matrix transLegMat;
	Math::Matrix rotRLegMat;
	transLegMat = DirectX::XMMatrixTranslation(0, 1, 0);
	rotRLegMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-m_limbsAng));
	m_rightLegMat = rotRLegMat * transLegMat * m_mat;

	Math::Matrix rotLLegMat;
	rotLLegMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_limbsAng));
	m_leftLegMat = rotLLegMat * transLegMat * m_mat;
}

const float Enemy::RotationAngle(const Math::Vector3& _nowVec, Math::Vector3& _destinationVec)const
{
	_destinationVec.Normalize();

	Math::Vector3 dot = DirectX::XMVector3Dot(_nowVec, _destinationVec);
	if (dot.x > 1)
	{
		dot.x = 1.0f;
	}
	if (dot.x < -1)
	{
		dot.x = -1.0f;
	}

	float ang{};
	ang = 10;

	//角度を求める
	if (DirectX::XMConvertToDegrees(acos(dot.x)) < ang)
	{
		ang = DirectX::XMConvertToDegrees(acos(dot.x));
	}

	Math::Vector3 cross;
	cross = DirectX::XMVector3Cross(_nowVec, _destinationVec);
	if (cross.y < 0)
	{
		ang = -ang;
	}
	return ang;
}

