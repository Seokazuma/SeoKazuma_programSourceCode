#pragma once

class Enemy;

class EnemyTrack :public BaseEnemy
{
public:
	EnemyTrack(Enemy* _enemy);

	BaseEnemy* Action(GameScene* _gameScene,Enemy* _enemy);

private:

	Math::Matrix m_targetMat;

	Enemy* m_enemy;
};