#pragma once

class Potion :public Treasure
{
public:

	Potion(GameScene* _gameScene, Math::Matrix _mat);

	~Potion();

	void Update();

	void Draw();

	void AnimeProc();

private:
	KdModel* m_model;

	GameScene* m_gameScene;

	int m_portionCollar;
	bool m_playerSpeedUp;
	float m_speedUpTime;

	float m_playerDis;

	float m_animeFrame;
	float m_ang;

	Math::Matrix m_startMat;
	Math::Matrix m_endMat;
};
