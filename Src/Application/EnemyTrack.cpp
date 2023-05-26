#include"Application/Headers.h"
#include "EnemyTrack.h"

EnemyTrack::EnemyTrack(Enemy* _enemy)
{
	m_enemy = _enemy;
}


BaseEnemy* EnemyTrack::Action(GameScene* _gameScene,Enemy* _enemy)
{

	Math::Matrix mat = _enemy->GetMat();
	Math::Vector3 vec = _enemy->GetVec();

	_enemy->SetMovePow(EnemyStatus::trackMovePow);

	_enemy->SetAttackFlg(false);

	//敵方向ベクトル
	vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), mat);
	_enemy->SetVec(vec);

	//プレイヤー座標
	const Player& player = _gameScene->GetPlayer();
	m_targetMat = player.GetMat();

	//プレイヤー方向
	Math::Vector3 targetVec;
	targetVec = m_targetMat.Translation() - mat.Translation();

	//プレイヤーまでの距離
	float dis{};
	dis = targetVec.Length();

	const Wall& wall = _gameScene->GetWall();

	Math::Matrix nearMat = Math::Matrix::Identity;
	//プレイヤーが透明になったら
	if (dis > 15 || player.GetTranslucentFlg() == true)
	{
		//近くの交差地点に行く
		nearMat=wall.NearDestination(mat);
		m_enemy->SetSpotMat(nearMat);

		return new EnemyWalk(m_enemy,_gameScene);
	}
	if (dis <= 3)
	{
		return new EnemyAttack(m_enemy);
	}
	//プレイヤー方向へ回転
	_enemy->SetRotAngY(m_enemy->RotationAngle(vec, targetVec));

	SHADER.AddSpotLighit(mat.Translation() + Math::Vector3(0, 1, 0), vec, 20, DirectX::XMConvertToRadians(30), { 3,3,1 });

	return nullptr;

}