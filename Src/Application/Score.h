#pragma once

class GameScene;
class Player;

class Score
{
public:
	Score(GameScene* _gameScene);

	void Update();
	void Draw();

	const void SetScor(const int _score)
	{
		m_score = _score;
	}

	const int& GetDiamondScore()const
	{
		return m_score;
	}

	const int Numcalculation(const int& _num, const int& _place)const;


private:

	KdTexture* m_numTex[10];
	KdTexture* m_hpTex;
	KdTexture* m_scoreTex;
	KdTexture* m_crossTex;
	KdTexture* m_colonTex;

	GameScene* m_gameScene;

	Math::Matrix m_mat[4][3];

	int m_num[4][3]{};

	int m_minutes;
	int m_seconds;


	int m_diamondNum;
	int m_score;
	int m_oldScore;
	int m_hp;
	int m_oldHp;

	int m_portion;

	uint8_t m_scalingFlg;
	float m_scoreScaling;
	float m_hpScaling;

	enum Place
	{
		thousand,
		hundred,
		ten,
		one
	};
};
