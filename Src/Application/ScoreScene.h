#pragma once

class GameScene;
class Player;
class Inventory;

class ScoreScene :public BaseScene
{
public:
	ScoreScene(GameScene* _gameScene);

	void Update();

	void Draw();

	~ScoreScene();

	//マイナスを数字の表示場所に合わせる
	const int MinusTexPos(const int _num);

private:

	//DirectX::SpriteFont* m_spriteFont;

	KdTexture* m_backTex;
	KdTexture* m_dollarTex;
	KdTexture* m_scoreTex;
	KdTexture* m_minusTex;

	KdTexture* m_numTex[10];

	GameScene* m_gameScene;

	Math::Matrix m_mat[6][4];
	//Math::Matrix m_scoreMat[4];
	//Math::Matrix m_damageMat[4];
	//Math::Matrix m_dollarMat[4];

	int m_diamond;
	int m_damage;
	int m_lostTime;
	int m_minutes;
	int m_seconds;
	int m_score;

	int m_num[6][4];

	bool m_minusScore;

	enum Type
	{
		diamond,
		damage,
		lostTime,
		minutes,
		seconds,
		score
	};

	enum Place
	{
		thousand,
		hundred,
		ten,
		one
	};
};