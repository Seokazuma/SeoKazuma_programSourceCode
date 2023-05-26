#pragma once

class GameScene;
class Player;

class EnemyManager
{
public:
	EnemyManager(GameScene* _gameScene);
	~EnemyManager();
	void Update();
	void Draw();

	int GetDamager()const
	{
		return m_damage;
	}
	Math::Vector3 GetNowVec()const
	{
		return m_vec;
	}
	const Math::Vector3 GetPlayerHitVec()const
	{
		return m_hitPlayer;
	}
	const bool GetPlayerHitFlg()const
	{
		return m_hitPlayerFlg;
	}

private:

	std::vector<Enemy*> m_enemy;

	GameScene* m_gameScene;

	Math::Vector3 m_vec;

	Math::Vector3 m_hitPlayer;

	float m_enemySize;

	int m_damage;

	bool m_hitPlayerFlg;
};