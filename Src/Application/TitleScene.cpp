#include"Headers.h"

TitleScene::TitleScene()
{

	// 平行ライト設定
	SHADER.m_cb8_Light.Work().DL_Dir = { -1,0,0 };
	SHADER.m_cb8_Light.Work().DL_Dir.Normalize();
	SHADER.m_cb8_Light.Work().DL_Color = { 1,1,1 };
	//アンビエントライトの設定（環境光）
	SHADER.m_cb8_Light.Work().AmbientLight = { 0.4f,0.4f,0.4f };

	m_model = MODELMANAGER.GetModel("Data/Title_scene/Title_scene.gltf");
	m_mat = DirectX::XMMatrixTranslation(-2.0f, -0.1f, -2.7f);

	m_diamondModel = MODELMANAGER.GetModel("Data/diamond/diamond.gltf");
	m_diamondMat = DirectX::XMMatrixTranslation(-0.7f, 0, 3.0f);

	m_ceilingModel = MODELMANAGER.GetModel("Data/Title_scene/Title_Ceiling.gltf");
	m_ceilingMat = DirectX::XMMatrixTranslation(m_mat._41, m_mat._42, m_mat._43);;

	m_TitleTex = TEXTUREMANAGER.GetTexture("Data/Title_scene/TitleLogo.png");
	m_TitleMat = DirectX::XMMatrixTranslation(0, 200, 0);

	m_EnterTex = TEXTUREMANAGER.GetTexture("Data/Title_scene/clickStart.png");
	m_EnterMat = DirectX::XMMatrixTranslation(0, -250, 0);

	m_backTex = TEXTUREMANAGER.GetTexture("Data/Gamescene/black.png");

	m_Level1Tex=TEXTUREMANAGER.GetTexture("Data/Title_scene/Level-1.png");
	m_Level1Mat = DirectX::XMMatrixTranslation(0, 100, 0);
	m_Level2Tex=TEXTUREMANAGER.GetTexture("Data/Title_scene/Level-2.png");
	m_Level2Mat = DirectX::XMMatrixTranslation(0, 0, 0);
	m_Level3Tex=TEXTUREMANAGER.GetTexture("Data/Title_scene/Level-3.png");
	m_Level3Mat = DirectX::XMMatrixTranslation(0, -100, 0);



	Math::Matrix transMat = DirectX::XMMatrixTranslation(-0.7f, 1, 3.0f);
	Math::Matrix rotMat = DirectX::XMMatrixRotationRollPitchYaw(
		0,
		DirectX::XMConvertToRadians(180),
		0);
	m_endMat = rotMat * transMat;
	m_startMat = m_diamondMat;
	m_ang = 0;
	m_animeFrame = 0;

	//BGM
	m_soundBgm = std::make_shared<KdSoundEffect>();
	m_soundBgm->Load("Data/Title_scene/TitleAudio.WAV");
	m_soundBgmInst = m_soundBgm->CreateInstance(false);
	m_soundBgmInst->SetVolume(0.07f);//音のボリューム
	m_soundBgmInst->Play(true);

	//クリック音
	m_soundClick = std::make_shared<KdSoundEffect>();
	m_soundClick->Load("Data/Title_scene/click.WAV");
	m_soundClickInst = m_soundClick->CreateInstance(false);
	m_soundClickInst->SetVolume(0.1f);//音のボリューム

	//選択音
	m_soundSelection = std::make_shared<KdSoundEffect>();
	m_soundSelection->Load("Data/Title_scene/selection.WAV");
	m_soundSelectionInst = m_soundSelection->CreateInstance(false);
	m_soundSelectionInst->SetVolume(0.1f);//音のボリューム

	//ドア開ける音
	m_doorSound = std::make_shared<KdSoundEffect>();
	m_doorSound->Load("Data/Gamescene/doorOpen.WAV");
	m_doorSoundInst = m_doorSound->CreateInstance(false);
	m_doorSoundInst->SetVolume(0.3f);//音のボリューム

	m_backFlg = false;
	m_checkFlg = 0;
	m_startClickFlg = true;
	m_levelFlg = false;

	m_backSpeed = 0;

	m_gameLevel = 3;
}

void TitleScene::Update()
{
	if(GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		if (SYSTEM.GetEscapeFlg() == false)
		{
			SYSTEM.SetEscapeFlg(true);
			APP.m_window.Release();
		}
	}
	else
	{
		SYSTEM.SetEscapeFlg(false);
	}

	//カーソル
	POINT currentPt;
	GetCursorPos(&currentPt);
	ScreenToClient(APP.m_window.GetWndHandle(), &currentPt);

	RECT rectClient;
	GetClientRect(APP.m_window.GetWndHandle(), &rectClient);

	currentPt.x *= ((LONG)ScrW / (LONG)rectClient.right);
	currentPt.y *= ((LONG)ScrH / (LONG)rectClient.bottom);

	currentPt.x -= ScrW / 2;
	currentPt.y -= ScrH / 2;
	currentPt.y *= -1;

	//暗転
	if (m_backFlg == true)
	{
		m_backSpeed += 0.01f;
		if (m_backSpeed >= 1)
		{
			ShowCursor(FALSE);
			SCENEMANAGER.ChangeScene(new GameScene(m_gameLevel));
		}
	}
	//スタートボタン
	if (m_startClickFlg == true)
	{
		if ((200 > currentPt.x) && (currentPt.x > -200) && (-200 > currentPt.y) && (currentPt.y > -300))
		{
			m_EnterMat = DirectX::XMMatrixScaling(1.2f, 1.2f, 0) * DirectX::XMMatrixTranslation(0, -250, 0);

			if (!(m_checkFlg & (1<<0)))
			{
				m_soundSelectionInst->Play(false);
				m_checkFlg |= (1<<0);
			}

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				if (SYSTEM.GetLbuttonFlg() == false)
				{
					SYSTEM.SetLbuttonFlg(true);
					m_levelFlg = true;
					m_startClickFlg = false;
					m_soundClickInst->Play(false);
				}
			}
			else
			{
				SYSTEM.SetLbuttonFlg(false);
			}
		}
		else
		{
			m_EnterMat = DirectX::XMMatrixTranslation(0, -250, 0);
			m_checkFlg &= ~(1 << 0);
		}
	}

	enum
	{
		level1Flg,
		level2Flg,
		level3Flg
	};
	uint8_t levelFlg = 0;

	if (m_levelFlg==true)
	{
		if ((150 > currentPt.x) && (currentPt.x > -150) && (150 > currentPt.y) && (currentPt.y > 50))
		{
			levelFlg |= (1 << level1Flg);
			m_Level1Mat = DirectX::XMMatrixScaling(1.2f, 1.2f, 0) * DirectX::XMMatrixTranslation(0, 100, 0);
			if (!(m_checkFlg & (1 << 1)))
			{
				m_soundSelectionInst->Play(false);
				m_checkFlg |= (1 << 1);
			}
		}
		else
		{
			levelFlg &= ~(1<<level1Flg);
			m_Level1Mat = DirectX::XMMatrixTranslation(0, 100, 0);
			m_checkFlg &= ~(1 << 1);
		}
		if ((150 > currentPt.x) && (currentPt.x > -150) && (50 > currentPt.y) && (currentPt.y > -50))
		{
			levelFlg |= (1 << level2Flg);
			m_Level2Mat = DirectX::XMMatrixScaling(1.2f, 1.2f, 0) * DirectX::XMMatrixTranslation(0, 0, 0);
			if (!(m_checkFlg & (1 << 2)))
			{
				m_soundSelectionInst->Play(false);
				m_checkFlg |= (1 << 2);
			}
		}
		else
		{
			levelFlg &= ~(1 << level2Flg);
			m_Level2Mat = DirectX::XMMatrixTranslation(0, 0, 0);
			m_checkFlg &= ~(1 << 2);
		}
		if ((150 > currentPt.x) && (currentPt.x > -150) && (-50 > currentPt.y) && (currentPt.y > -150))
		{
			levelFlg |= (1 << level3Flg);
			m_Level3Mat = DirectX::XMMatrixScaling(1.2f, 1.2f, 0) * DirectX::XMMatrixTranslation(0, -100, 0);
			if (!(m_checkFlg & (1 << 3)))
			{
				m_soundSelectionInst->Play(false);
				m_checkFlg |= (1 << 3);
			}
		}
		else
		{
			levelFlg &= ~(1 << level3Flg);
			m_Level3Mat = DirectX::XMMatrixTranslation(0, -100, 0);
			m_checkFlg &= ~(1 << 3);
		}

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				if (SYSTEM.GetLbuttonFlg() == false)
				{
					SYSTEM.SetLbuttonFlg(true);
					if (levelFlg & (1<<level1Flg))
					{
						m_backFlg = true;
						m_soundClickInst->Play(false);
						m_doorSoundInst->Play(false);
						m_gameLevel = 1;
						m_levelFlg = false;
					}
					if (levelFlg & (1 << level2Flg))
					{
						m_backFlg = true;
						m_soundClickInst->Play(false);
						m_doorSoundInst->Play(false);
						m_gameLevel = 2;
						m_levelFlg = false;
					}
					if (levelFlg & (1 << level3Flg))
					{
						m_backFlg = true;
						m_soundClickInst->Play(false);
						m_doorSoundInst->Play(false);
						m_gameLevel = 3;
						m_levelFlg = false;
					}

				}
			}
			else
			{
				SYSTEM.SetLbuttonFlg(false);
			}

		
	}



	m_animeFrame += 0.01f;
	if (m_animeFrame > 1)
	{
		m_animeFrame = 1.0f;
	}

	SHADER.AddPointLight({ 6,6,10 }, { -1.5, m_animeFrame + 2, 3 }, m_animeFrame * 10.0f);

	AnimeProc();

	m_animeFrame = (sin(DirectX::XMConvertToRadians(m_ang)) + 1) / 2;
	m_ang++;
	if (m_ang > 360)
	{
		m_ang -= 360;
		AnimeProc();
	}

}

void TitleScene::Draw()
{

	SHADER.m_standardShader.SetToDevice();

	SHADER.m_standardShader.SetDitherEnable(false);

	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);

	SHADER.m_standardShader.SetToDevice();
	SHADER.m_standardShader.SetWorldMatrix(m_ceilingMat);
	SHADER.m_standardShader.DrawModel(m_ceilingModel);

	SHADER.m_standardShader.SetToDevice();
	SHADER.m_standardShader.SetWorldMatrix(m_diamondMat);
	SHADER.m_standardShader.DrawModel(m_diamondModel);

	SHADER.m_spriteShader.Begin();

	SHADER.m_spriteShader.SetMatrix(m_TitleMat * DirectX::XMMatrixScaling(1.2f, 1.2f, 0));
	SHADER.m_spriteShader.DrawTex(m_TitleTex, Math::Rectangle(0, 0, 490, 130), 1.0f);

	if (m_startClickFlg == true)
	{
		SHADER.m_spriteShader.SetMatrix(m_EnterMat);
		SHADER.m_spriteShader.DrawTex(m_EnterTex, Math::Rectangle(0, 0, 417, 121), 1.0f);
	}

	if (m_levelFlg == true)
	{
		SHADER.m_spriteShader.SetMatrix(m_Level1Mat);
		SHADER.m_spriteShader.DrawTex(m_Level1Tex, Math::Rectangle(0, 0, 294, 111), 1.0f);

		SHADER.m_spriteShader.SetMatrix(m_Level2Mat);
		SHADER.m_spriteShader.DrawTex(m_Level2Tex, Math::Rectangle(0, 0, 294, 111), 1.0f);

		SHADER.m_spriteShader.SetMatrix(m_Level3Mat);
		SHADER.m_spriteShader.DrawTex(m_Level3Tex, Math::Rectangle(0, 0, 294, 111), 1.0f);
	}


	SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(0, 0, 0));
	SHADER.m_spriteShader.DrawTex(m_backTex, Math::Rectangle(0, 0, 1280, 720), m_backSpeed);

	SHADER.m_spriteShader.End();
}

void TitleScene::SetCamera()
{
	//カメラ

	Math::Matrix rotmat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(25));

	SHADER.m_cb7_Camera.Work().mV = DirectX::XMMatrixLookToLH(
		Math::Vector3{ 0,3,0 },
		DirectX::XMVector3TransformNormal(Math::Vector3(0, 0, 1), rotmat),
		Math::Vector3{ 0,1,0 });
	SHADER.m_cb7_Camera.Work().CamPos = SHADER.m_cb7_Camera.Work().mV.Invert().Translation();

	//射影行列
	SHADER.m_cb7_Camera.Work().mP = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(90), (float)ScrW / (float)ScrH, 0.01f, 1000.0f);

	//ビュー行列・射影行列をGPUに転送
	SHADER.m_cb7_Camera.Write();
}

TitleScene::~TitleScene()
{
	m_soundBgmInst->Stop();
}

void TitleScene::AnimeProc()
{
	//クォータニオン用意
	Math::Quaternion startQua;
	Math::Quaternion endQua;
	Math::Quaternion nowQua;

	//最初と最後の行列からクオータニオンを作成
	startQua = DirectX::XMQuaternionRotationMatrix(m_startMat);
	endQua = DirectX::XMQuaternionRotationMatrix(m_endMat);

	//中間行列を求める(Sleap = エス　ラープ
	nowQua = DirectX::XMQuaternionSlerp(startQua, endQua, m_animeFrame);
	m_diamondMat = DirectX::XMMatrixRotationQuaternion(nowQua);

	//中間座標を求める
	Math::Vector3 startPos;
	Math::Vector3 endPos;
	Math::Vector3 nowPos;

	//最初と最後の座標を求める
	startPos = m_startMat.Translation();
	endPos = m_endMat.Translation();

	//中間座標を求める（Lerpラープ）
	nowPos = DirectX::XMVectorLerp(startPos, endPos, m_animeFrame);

	//中間座標を行列にセット
	m_diamondMat.Translation(nowPos);
}