#include"Application/Headers.h"
#include "EnemyAttack.h"

EnemyAttack::EnemyAttack(Enemy* _enemy)
{
	m_enemy = _enemy;
}


BaseEnemy* EnemyAttack::Action(GameScene* _gameScene,Enemy* _enemy)
{
	Math::Matrix mat = _enemy->GetMat();
	Math::Vector3 vec = _enemy->GetVec();

	_enemy->SetMovePow(0);

	_enemy->SetAttackFlg(true);
	//敵方向ベクトル
	vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), mat);
	_enemy->SetVec(vec);

	const Player& player = _gameScene->GetPlayer();
	m_targetMat = player.GetMat();
	//敵から見たプレイヤーへの方向ベクトル
	Math::Vector3 targetVec;
	targetVec = m_targetMat.Translation() - mat.Translation();

	float dis{};
	dis = targetVec.Length();

	//歩きながら剣を振るとき
	if (dis > 3)
	{
		_enemy->SetMovePow(EnemyStatus::attackMovePow);
	}
	//剣が届かなかったら
	if (dis > 5)
	{
		return new EnemyTrack(m_enemy);
	}

	const Wall& wall = _gameScene->GetWall();
	Math::Matrix nearMat = Math::Matrix::Identity;

	//プレイヤーが透明になったら
	if (player.GetTranslucentFlg() == true)
	{
		//近くの交差地点に行く
		nearMat=wall.NearDestination(mat);
		m_enemy->SetSpotMat(nearMat);

		return new EnemyWalk(m_enemy,_gameScene);
	}

	//プレイヤー方向へ回転
	_enemy->SetRotAngY(m_enemy->RotationAngle(vec, targetVec));

	SHADER.AddSpotLighit(mat.Translation() + Math::Vector3(0, 1, 0), vec, 20, DirectX::XMConvertToRadians(30), { 2,2,1 });

	return nullptr;
}
