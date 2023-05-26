#include"Headers.h"

Score::Score(GameScene* _gameScene)
{
	m_gameScene = _gameScene;

	m_numTex[0] = TEXTUREMANAGER.GetTexture("Data/Number/0.png");
	m_numTex[1] = TEXTUREMANAGER.GetTexture("Data/Number/1.png");
	m_numTex[2] = TEXTUREMANAGER.GetTexture("Data/Number/2.png");
	m_numTex[3] = TEXTUREMANAGER.GetTexture("Data/Number/3.png");
	m_numTex[4] = TEXTUREMANAGER.GetTexture("Data/Number/4.png");
	m_numTex[5] = TEXTUREMANAGER.GetTexture("Data/Number/5.png");
	m_numTex[6] = TEXTUREMANAGER.GetTexture("Data/Number/6.png");
	m_numTex[7] = TEXTUREMANAGER.GetTexture("Data/Number/7.png");
	m_numTex[8] = TEXTUREMANAGER.GetTexture("Data/Number/8.png");
	m_numTex[9] = TEXTUREMANAGER.GetTexture("Data/Number/9.png");
	m_scoreTex = TEXTUREMANAGER.GetTexture("Data/Score_scene/dollar.png");
	m_hpTex = TEXTUREMANAGER.GetTexture("Data/Score_scene/Hp.png");
	m_crossTex = TEXTUREMANAGER.GetTexture("Data/Number/cross.png");
	m_colonTex = TEXTUREMANAGER.GetTexture("Data/Number/colon.png");

	m_diamondNum = 0;
	m_score = 0;
	m_oldScore = 0;
	m_hp = 0;
	m_oldHp = 0;
	m_portion = 0;

	m_seconds = 0;
	m_minutes = 0;

	m_scalingFlg = 0;
	m_scoreScaling = 1.0f;
	m_hpScaling = 1.0f;
}

void Score::Update()
{


	const Player& player = m_gameScene->GetPlayer();
	m_hp = player.GetHp();
	const TreasureManager& treMan = m_gameScene->GetTreMan();
	if (treMan.GetTreasureType() == TreasureType::diamond)
	{
		m_diamondNum++;
	}
	m_score = m_diamondNum * 100;

	int timeLimit = m_gameScene->GetTimeLimit();
	m_minutes = timeLimit / 60 / 60;
	m_seconds = (timeLimit / 60) - (m_minutes * 60);

	enum
	{
		scoreFlg,
		hpFlg
	};

	//ÉXÉRÉAÇÃägèk
	if (m_oldScore < m_score)
	{
		if (m_scalingFlg & (1 << scoreFlg))
		{
			m_scalingFlg |= (1 << scoreFlg);
		}
		m_scoreScaling = 1.5f;
	}
	if (!(m_scalingFlg & (1 << scoreFlg)))
	{
		m_scoreScaling -= 0.02f;
		if (m_scoreScaling <= 1)
		{
			m_scalingFlg &= (1 << scoreFlg);
			m_scoreScaling = 1;
		}
	}
	m_oldScore = m_score;

	//HPÇÃägèk
	if ((m_oldHp < m_hp) || (m_oldHp > m_hp))
	{
		if (m_scalingFlg & (1 << hpFlg))
		{
			m_scalingFlg |= (1 << hpFlg);
		}
		m_hpScaling = 1.5f;
	}
	if (!(m_scalingFlg & (1 << hpFlg)))
	{
		m_hpScaling -= 0.02f;
		if (m_hpScaling <= 1)
		{
			m_scalingFlg &= (1 << hpFlg);
			m_hpScaling = 1;
		}
	}
	m_oldHp = m_hp;

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			if (y == 0)
			{
				m_num[x][y] = Numcalculation(m_score, x);
			}
			if (y == 1)
			{
				m_num[x][y] = Numcalculation(m_minutes, x);
			}
			if (y == 2)
			{
				m_num[x][y] = Numcalculation(m_seconds, x);
			}

			m_mat[x][0] = DirectX::XMMatrixScaling(m_scoreScaling, m_scoreScaling, 0) * DirectX::XMMatrixTranslation(x * 50.0f - 550, 300, 0);
			m_mat[x][1] = DirectX::XMMatrixTranslation(x * 50.0f + 300, 300, 0);
			m_mat[x][2] = DirectX::XMMatrixTranslation(x * 50.0f + 450, 300, 0);
		}
	}
}

void Score::Draw()
{
	SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(-350, 300, 0));
	SHADER.m_spriteShader.DrawTex(m_scoreTex, Math::Rectangle(0, 0, 53, 78), 1);

	SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(500, 300, 0));
	SHADER.m_spriteShader.DrawTex(m_colonTex, Math::Rectangle(0, 0, 60, 60), 1);

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			if ((x == 0 && y == 1) || (x == 1 && y == 1) ||
				(x == 0 && y == 2) || (x == 1 && y == 2))continue;

			int num = m_num[x][y];

			SHADER.m_spriteShader.SetMatrix(m_mat[x][y]);
			SHADER.m_spriteShader.DrawTex(m_numTex[num], Math::Rectangle(0, 0, 60, 60), 1);

		}
	}
}

const int Score::Numcalculation(const int& _num, const int& _place) const
{
	int tho = 0, hun = 0, ten = 0, one = 0;

	if (_place == Place::thousand)
	{
		tho = _num / 1000;
		return tho;
	}
	else if (_place == Place::hundred)
	{
		hun = (_num % 1000) / 100;
		return hun;
	}
	else if (_place == Place::ten)
	{
		ten = (_num % 100) / 10;
		return ten;
	}
	else
	{
		one = (_num % 10) / 1;
		return one;
	}
}
