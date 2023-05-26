#pragma once

class Diamond:public Treasure
{
public:

	Diamond(GameScene* _gameScene,Math::Matrix _mat);

	~Diamond();

	void Update();

	void Draw();

	void AnimeProc();


private:
	KdModel* m_model;

	GameScene* m_gameScene;

	float m_playerDis;
	float m_animeFrame;
	float m_ang;

	Math::Matrix m_startMat;
	Math::Matrix m_endMat;
};
