#include"Headers.h"

PauseScene::PauseScene()
{
	m_backTex = TEXTUREMANAGER.GetTexture("Data/Pause/Back.png");
	m_titleBackTex = TEXTUREMANAGER.GetTexture("Data/Pause/TitleBack.png");
	m_sensitivityTex = TEXTUREMANAGER.GetTexture("Data/Pause/Sensitivity.png");

	m_backMat= DirectX::XMMatrixTranslation(0, -100, 0);
	m_titleBackMat= DirectX::XMMatrixTranslation(0, 0, 0);
	m_sensitivityMat= DirectX::XMMatrixTranslation(0, 100, 0);

	m_pauseFlg = true;

}

PauseScene::~PauseScene()
{
}

void PauseScene::Update()
{
	m_pauseFlg = true;

	//カーソル
	POINT currentPt;
	GetCursorPos(&currentPt);
	ScreenToClient(APP.m_window.GetWndHandle(), &currentPt);

	RECT rectClient;
	GetClientRect(APP.m_window.GetWndHandle(), &rectClient);

	currentPt.x *= ((float)ScrW / (float)rectClient.right);
	currentPt.y *= ((float)ScrH / (float)rectClient.bottom);

	currentPt.x -= ScrW / 2;
	currentPt.y -= ScrH / 2;
	currentPt.y *= -1;

	//ゲームシーンに戻る
	if ((100 > currentPt.x) && (currentPt.x > -100) && (-50 > currentPt.y) && (currentPt.y > -150))
	{
		m_backMat = DirectX::XMMatrixScaling(1.2f, 1.2f, 0) * DirectX::XMMatrixTranslation(0, -100, 0);

		if (GetAsyncKeyState(MK_LBUTTON) & 0x8000)
		{
			if (SYSTEM.GetLbuttonFlg() == false)//直前までキーを押していなかった
			{
				SYSTEM.SetLbuttonFlg(true);

				m_pauseFlg = false;
				ShowCursor(FALSE);

			}
		}
		else
		{
			SYSTEM.SetLbuttonFlg(false);
		}
	}
	else
	{
		m_backMat = DirectX::XMMatrixScaling(1.0f, 1.0f, 0) * DirectX::XMMatrixTranslation(0, -100, 0);
	}

	//タイトルシーンに戻る
	if ((200 > currentPt.x) && (currentPt.x > -200) && (50 > currentPt.y) && (currentPt.y > -50))
	{
		m_titleBackMat = DirectX::XMMatrixScaling(1.2f, 1.2f, 0) * DirectX::XMMatrixTranslation(0, 0, 0);

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
	else
	{
		m_titleBackMat = DirectX::XMMatrixScaling(1.0f, 1.0f, 0) * DirectX::XMMatrixTranslation(0, 0, 0);
	}


}

void PauseScene::Draw()
{

	SHADER.m_spriteShader.SetMatrix(m_backMat);
	SHADER.m_spriteShader.DrawTex(m_backTex, Math::Rectangle(0, 0, 140, 100), 1.0f);

	SHADER.m_spriteShader.SetMatrix(m_titleBackMat);
	SHADER.m_spriteShader.DrawTex(m_titleBackTex, Math::Rectangle(0, 0, 430, 100), 1.0f);

}

bool PauseScene::GetPauseFlg()
{

	return m_pauseFlg;
}
