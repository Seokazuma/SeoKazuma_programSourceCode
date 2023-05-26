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

	//�G�����x�N�g��
	vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), mat);
	_enemy->SetVec(vec);

	//�v���C���[���W
	const Player& player = _gameScene->GetPlayer();
	m_targetMat = player.GetMat();

	//�v���C���[����
	Math::Vector3 targetVec;
	targetVec = m_targetMat.Translation() - mat.Translation();

	//�v���C���[�܂ł̋���
	float dis{};
	dis = targetVec.Length();

	const Wall& wall = _gameScene->GetWall();

	Math::Matrix nearMat = Math::Matrix::Identity;
	//�v���C���[�������ɂȂ�����
	if (dis > 15 || player.GetTranslucentFlg() == true)
	{
		//�߂��̌����n�_�ɍs��
		nearMat=wall.NearDestination(mat);
		m_enemy->SetSpotMat(nearMat);

		return new EnemyWalk(m_enemy,_gameScene);
	}
	if (dis <= 3)
	{
		return new EnemyAttack(m_enemy);
	}
	//�v���C���[�����։�]
	_enemy->SetRotAngY(m_enemy->RotationAngle(vec, targetVec));

	SHADER.AddSpotLighit(mat.Translation() + Math::Vector3(0, 1, 0), vec, 20, DirectX::XMConvertToRadians(30), { 3,3,1 });

	return nullptr;

}