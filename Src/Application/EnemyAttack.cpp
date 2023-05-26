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
	//�G�����x�N�g��
	vec = DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), mat);
	_enemy->SetVec(vec);

	const Player& player = _gameScene->GetPlayer();
	m_targetMat = player.GetMat();
	//�G���猩���v���C���[�ւ̕����x�N�g��
	Math::Vector3 targetVec;
	targetVec = m_targetMat.Translation() - mat.Translation();

	float dis{};
	dis = targetVec.Length();

	//�����Ȃ��猕��U��Ƃ�
	if (dis > 3)
	{
		_enemy->SetMovePow(EnemyStatus::attackMovePow);
	}
	//�����͂��Ȃ�������
	if (dis > 5)
	{
		return new EnemyTrack(m_enemy);
	}

	const Wall& wall = _gameScene->GetWall();
	Math::Matrix nearMat = Math::Matrix::Identity;

	//�v���C���[�������ɂȂ�����
	if (player.GetTranslucentFlg() == true)
	{
		//�߂��̌����n�_�ɍs��
		nearMat=wall.NearDestination(mat);
		m_enemy->SetSpotMat(nearMat);

		return new EnemyWalk(m_enemy,_gameScene);
	}

	//�v���C���[�����։�]
	_enemy->SetRotAngY(m_enemy->RotationAngle(vec, targetVec));

	SHADER.AddSpotLighit(mat.Translation() + Math::Vector3(0, 1, 0), vec, 20, DirectX::XMConvertToRadians(30), { 2,2,1 });

	return nullptr;
}
