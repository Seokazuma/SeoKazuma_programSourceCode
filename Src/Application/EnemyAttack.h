#pragma once

class Enemy;

class EnemyAttack :public BaseEnemy
{
public:
	EnemyAttack(Enemy* _enemy);

	BaseEnemy* Action(GameScene* _gameScene,Enemy* _enemy);


private:

	Math::Matrix m_targetMat;

	Enemy* m_enemy;
};
