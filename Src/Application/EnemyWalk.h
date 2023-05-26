#pragma once

class Enemy;

class EnemyWalk :public BaseEnemy
{
public:
	EnemyWalk(Enemy* _enemy, GameScene* _gameScene);

	BaseEnemy* Action(GameScene* _gameScene,Enemy* _enemy);

	const bool LookPlayer(GameScene* _gameScene,const Math::Matrix& _mat,const Math::Vector3& _nowVec);
	const Math::Matrix NextSpot(GameScene* _gameScene, const Math::Matrix& _mat);

private:
	//float m_targetAng;//–Ú•WŠp“x
	//int m_lastTime;//s“®Œp‘±ŠúŠÔ
	int m_spotFlg;
	Math::Matrix m_spotMat1;
	Math::Matrix m_spotMat2;
	Math::Matrix m_nextspotMat;

	Math::Vector3 m_currentspot;

	std::shared_ptr<KdSoundEffect>m_whistleSound;
	std::shared_ptr<KdSoundInstance>m_whistleInst;

	Enemy* m_enemy;

	int m_mapX;
	int m_mapZ;

	int m_direction;

	float m_stopTime;
	bool m_moveFlg;
	bool m_discoverFlg;

	float m_jump;
};