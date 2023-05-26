#include"Headers.h"
#include "ScoreScene.h"

ScoreScene::ScoreScene(GameScene* _gameScene)
{
	m_gameScene = _gameScene;

	m_backTex = TEXTUREMANAGER.GetTexture("Data/Score_scene/back.png");
	m_dollarTex = TEXTUREMANAGER.GetTexture("Data/Score_scene/dollar.png");
	m_scoreTex = TEXTUREMANAGER.GetTexture("Data/Score_scene/score.png");
	m_minusTex = TEXTUREMANAGER.GetTexture("Data/Number/minus.png");

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

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			m_mat[x][y] = Math::Matrix::Identity;
			m_num[x][y] = 0;
		}
	}

	//ダイアモンドスコア
	const Score& gameScore = m_gameScene->GetScore();
	m_diamond = gameScore.GetDiamondScore();

	//ダメージマイナス
	const Player& player = m_gameScene->GetPlayer();
	m_damage = (PlayerStatus::Hp - player.GetHp()) * 50;

	//タイム計算
	int timeLimit = m_gameScene->GetTimeLimit();
	m_minutes = timeLimit / 60 / 60;
	m_seconds = (timeLimit / 60) - (m_minutes * 60);

	//ロストタイムマイナス計算
	m_lostTime = ((int)((TimeLimit::minutes * 60) + (TimeLimit::seconds)) - ((m_minutes * 60) + m_seconds)) * 5;

	//最終スコア計算
	m_score = (m_diamond) - (m_damage + m_lostTime);

	//もしカンストした場合
	if (m_score >= 9999)
	{
		m_score = 9999;
	}

	m_minusScore = false;
	//スコアがマイナス
	if (m_score < 0)
	{
		m_score *= -1;
		m_minusScore = true;
	}

	//数をケタごとに分ける
	for(int type = 0; type < 6; type++)
	{
		for (int place = 0; place < 4; place++)
		{
			if (type == diamond)
			{
				m_num[type][place] = gameScore.Numcalculation(m_diamond, place);
			}
			if(type == damage)
			{
				m_num[type][place] = gameScore.Numcalculation(m_damage, place);
			}
			if (type == minutes)
			{
				m_num[type][place] = gameScore.Numcalculation(m_minutes, place);
			}
			if (type == seconds)
			{
				m_num[type][place] = gameScore.Numcalculation(m_seconds, place);
			}
			if (type == lostTime)
			{
				m_num[type][place] = gameScore.Numcalculation(m_lostTime, place);
			}
			if (type == score)
			{
				m_num[type][place] = gameScore.Numcalculation(m_score, place);
			}
		}
	}
}

void ScoreScene::Update()
{
	//シーン切り替え
	if (GetAsyncKeyState(MK_LBUTTON) & 0x8000)
	{
		if (SYSTEM.GetLbuttonFlg() == false)//直前までキーを押していなかった
		{
			SYSTEM.SetLbuttonFlg(true);

			SCENEMANAGER.ChangeScene(new TitleScene);
		}
	}
	else
	{
		SYSTEM.SetLbuttonFlg(false);
	}
}

void ScoreScene::Draw()
{
	SHADER.m_spriteShader.Begin();
	SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(0, 0, 0));
	SHADER.m_spriteShader.DrawTex(m_backTex, Math::Rectangle(0, 0, 1280, 720), 1);

	SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(150, -150, 0));
	SHADER.m_spriteShader.DrawTex(m_dollarTex, Math::Rectangle(0, 0, 53, 78), 1);

	//線テクスチャ（-----------）
	for (int x = 0; x < 14; x++)
	{
		SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(x * 50.0f - 450, -100, 0));
		SHADER.m_spriteShader.DrawTex(m_minusTex, Math::Rectangle(0, 0, 60, 60), 1);
	}
	
	//スコアのマイナス座標
	if (m_minusScore == true)
	{
		SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(-100.0f + (float)MinusTexPos(m_score), -160, 0));
		SHADER.m_spriteShader.DrawTex(m_minusTex, Math::Rectangle(0, 0, 60, 60), 1);
	}

	//ダメージのマイナス座標
	if (m_damage > 0)
	{
		SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(-100.0f + (float)MinusTexPos(m_damage), 95, 0));
		SHADER.m_spriteShader.DrawTex(m_minusTex, Math::Rectangle(0, 0, 60, 60), 1);
	}

	//タイムロスのマイナス座標
	if (m_lostTime > 0)
	{
		SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(-100.0f + (float)MinusTexPos(m_lostTime), -5, 0));
		SHADER.m_spriteShader.DrawTex(m_minusTex, Math::Rectangle(0, 0, 60, 60), 1);
	}

	//数を表示
	for (int type = 0; type < 6; type++)
	{
		for (int place = 0; place < 4; place++)
		{
			int num = m_num[type][place];

			//いらない０を消す
			if (type != minutes)
			{
				if (type != seconds)
				{
					if (place == thousand && m_num[type][thousand] == 0)continue;
					if (place == hundred && m_num[type][thousand] == 0 && m_num[type][hundred] == 0)continue;
					if (place == ten && m_num[type][thousand] == 0 && m_num[type][hundred] == 0 && m_num[type][ten] == 0)continue;
				}
			}
			
			if (type == diamond)
			{
				SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation((float)place * 50.0f - 50.0f, 200.0f, 0));
				SHADER.m_spriteShader.DrawTex(m_numTex[num], Math::Rectangle(0, 0, 60, 60), 1);
			}
			if (type == damage)
			{
				SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation((float)place * 50.0f - 50.0f, 100.0f, 0));
				SHADER.m_spriteShader.DrawTex(m_numTex[num], Math::Rectangle(0, 0, 60, 60), 1);
			}
			if (type == lostTime)
			{
				SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation((float)place * 50.0f - 50.0f, 0, 0));
				SHADER.m_spriteShader.DrawTex(m_numTex[num], Math::Rectangle(0, 0, 60, 60), 1);
			}
			if (type == minutes)
			{
				if (place == 0 || place == 1)continue;
				SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation((float)place * 50.0f - 500.0f, 0, 0));
				SHADER.m_spriteShader.DrawTex(m_numTex[num], Math::Rectangle(0, 0, 60, 60), 1);
			}
			if (type == seconds)
			{
				if (place == 0 || place == 1)continue;
				SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation((float)place * 50.0f - 350.0f, 0, 0));
				SHADER.m_spriteShader.DrawTex(m_numTex[num], Math::Rectangle(0, 0, 60, 60), 1);
			}
			if (type == score)
			{
				SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation((float)place * 50.0f - 50.0f, -150.0f, 0));
				SHADER.m_spriteShader.DrawTex(m_numTex[num], Math::Rectangle(0, 0, 60, 60), 1);
				SHADER.m_spriteShader.End();
			}
			
		}
	}
}

ScoreScene::~ScoreScene()
{
}

const int ScoreScene::MinusTexPos(const int _num)
{
	int x = 0;
	if (_num < 10) { x = 150; }
	else if (_num < 100) { x = 100; }
	else if (_num < 1000) { x = 50; }
	return x;
}

