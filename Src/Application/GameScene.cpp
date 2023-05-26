#include"Headers.h"

void GameScene::Update()
{
	//ポーズシーンへ飛ぶ
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		if (SYSTEM.GetEscapeFlg() == false)
		{
			SYSTEM.SetEscapeFlg(true);
			m_gameFlg |= (1 << GameFlg::m_pauseFlg);
			ShowCursor(TRUE);
		}
	}
	else
	{
		SYSTEM.SetEscapeFlg(false);
	}

	//ポーズシーン
	if (m_gameFlg & (1<< GameFlg::m_pauseFlg))
	{
		m_pause->Update();
		if (m_pause->GetPauseFlg() == false)
		{
			m_gameFlg &= ~(1 << GameFlg::m_pauseFlg);
		}
	}
	else
	{
		//タイトルシーンへ飛ぶ
		--m_timeLimit;
		if (GoalFlg() || m_timeLimit <= 0 || m_player->GetHp() <= 0)
		{
			if (m_timeLimit <= 0)
			{
				m_timeLimit = 0;
			}
			m_gameFlg |= (1 << GameFlg::m_goalFlg);
		}
		if (m_gameFlg & (1 << GameFlg::m_goalFlg))
		{
			//暗転
			m_backTransparency += 0.01f;

			if (m_backTransparency >= 1)
			{
				ShowCursor(TRUE);
				SCENEMANAGER.ChangeScene(new ScoreScene(this));
			}
		}
		//ゲームシーン（本編）
		else
		{
			//暗転
			if (m_backTransparency > 0.01f)
			{
				m_backTransparency -= 0.01f;
			}
			if (m_backTransparency <= 0)
			{
				m_backTransparency = 0;
			}

			m_player->Update(m_camAngY);
			m_enemyManager->Update();
			m_score->Update();
			m_treMan->Update();
			m_wall->Update();

			//カーソル
			POINT currentPt;
			GetCursorPos(&currentPt);

			m_camAngY += (currentPt.x - m_basePt.x) / 8.0f;
			m_camAngX += (currentPt.y - m_basePt.y) / 8.0f;

			if (m_camAngX < -20)
			{
				m_camAngX = -20;
			}
			if (m_camAngX > 45)
			{
				m_camAngX = 45;
			}

			SetCursorPos(m_basePt.x, m_basePt.y);

			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				m_camAngX += 2.0f;
				if (m_camAngX > 40)
				{
					m_camAngX = 40;
				}
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				m_camAngX -= 2.0f;
				if (m_camAngX < -80)
				{
					m_camAngX = -80;
				}
			}
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				m_camAngY += 2.0f;
			}
			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				m_camAngY -= 2.0f;
			}
		}
	}

	//マップ開く
	m_gameFlg &= ~(1 << GameFlg::m_mapOpenFlg);
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_gameFlg |= (1 << GameFlg::m_mapOpenFlg);
	}

	//視錐台カリング用
	m_viewArea.Origin = m_player->GetPos();

	Math::Matrix playerRotMat;
	playerRotMat = DirectX::XMMatrixRotationRollPitchYaw
	(DirectX::XMConvertToRadians(m_camAngX),
		DirectX::XMConvertToRadians(m_camAngY), 0);

	Math::Quaternion playerQuaternion;
	playerQuaternion = DirectX::XMQuaternionRotationMatrix(playerRotMat);

	m_viewArea.Orientation = playerQuaternion;

}

void GameScene::DynamicDraw()
{
	//裏面カリングオン
	D3D.GetDevContext()->RSSetState(cullBack);

	SHADER.m_standardShader.SetToDevice();

	//カメラが近づいたらディザ抜き
	SHADER.m_standardShader.SetDitherEnable(true);

	m_treMan->Draw();
	m_wall->Draw();

	//ディザ抜きオフ
	SHADER.m_standardShader.SetDitherEnable(false);

	m_enemyManager->Draw();
	m_player->Draw();

	//エッジ用
	SHADER.m_spriteShader.SetEdgeParam(2.5f, 0.0001f, 10.0f, true);

	//裏面カリングオフ
	D3D.GetDevContext()->RSSetState(cullOff);

	//エッジオフ
	D3D.GetBackBuffer()->SetRenderTarget(false);
}

void GameScene::Draw()
{

	SHADER.m_spriteShader.Begin();
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);

	SHADER.m_spriteShader.DrawTex(D3D.GetZBuffer().get(), { 0,0,1280,720 }, 1.0f);
	SHADER.m_spriteShader.End();

	D3D.GetBackBuffer()->SetRenderTarget(true);

	SHADER.m_spriteShader.SetEdgeParam(0, 0, 0, false);

	m_potionEffectMan->Draw();
	m_score->Draw();
	m_player->HpBarDraw();

	ExitTexDraw();

	if (!(m_gameFlg & (1 << GameFlg::m_pauseFlg)))
	{
		//Mapテクスチャ
		if (m_gameFlg & (1 << GameFlg::m_mapOpenFlg))
		{
			m_wall->MapTexDraw();
		}
	}

	//MapOpenKeyテクスチャ
	SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(500, -200, 0));
	SHADER.m_spriteShader.DrawTex(m_mapOpenKeyTex, Math::Rectangle(0, 0, 200, 55));

	//暗転テクスチャ
	SHADER.m_spriteShader.SetMatrix(DirectX::XMMatrixTranslation(0, 0, 0));
	SHADER.m_spriteShader.DrawTex(m_backTex, Math::Rectangle(0, 0, 1280, 720), m_backTransparency);

	if (m_gameFlg&(1<<GameFlg::m_pauseFlg))
	{
		m_pause->Draw();
	}

}

void GameScene::SetCamera()
{
	//カメラ
	Math::Matrix rotMatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_camAngY));
	Math::Matrix rotMatX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_camAngX));
	Math::Matrix rotMat = rotMatX * rotMatY;

	Math::Vector3 camPos = { 0, 5, -8 };
	m_camPos = DirectX::XMVector3TransformNormal(camPos, rotMat);

	m_camPos += m_player->GetPos();
	Math::Vector3 camLook = m_player->GetPos() + Math::Vector3(0, 4, 0);
	Math::Vector3 camHead = { 0,1,0 };

	SHADER.m_cb7_Camera.Work().mV = DirectX::XMMatrixLookAtLH(
		m_camPos,
		camLook,
		camHead);

	SHADER.m_cb7_Camera.Work().CamPos = SHADER.m_cb7_Camera.Work().mV.Invert().Translation();

	//射影行列
	SHADER.m_cb7_Camera.Work().mP = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians((float)m_player->GetCamAngle()), (float)ScrW / (float)ScrH, 0.01f, 1000.0f);

	//ビュー行列・射影行列をGPUに転送
	SHADER.m_cb7_Camera.Write();

}

GameScene::GameScene(int _gameLevel)
{
	//ゲームレベルゲット
	SetGameLevel(_gameLevel);

	srand(timeGetTime());

	// 平行ライト設定
	SHADER.m_cb8_Light.Work().DL_Dir = { 0.5f,-1.0f,0};
	SHADER.m_cb8_Light.Work().DL_Dir.Normalize();
	SHADER.m_cb8_Light.Work().DL_Color = { 0.0f,0.0f,0.0f };

	//アンビエントライトの設定（環境光）
	SHADER.m_cb8_Light.Work().AmbientLight = { 0.3f,0.3f,0.3f };

	m_backTex = TEXTUREMANAGER.GetTexture("Data/Gamescene/black.png");
	m_mapOpenKeyTex = TEXTUREMANAGER.GetTexture("Data/Gamescene/MapOpenKey.png");

	m_backTransparency = 1.0f;

	m_player = new Player(this);
	m_potionEffectMan = new PotionEffectManager(this);
	m_wall = new Wall(this);
	m_enemyManager = new EnemyManager(this);
	m_treMan = new TreasureManager(this);
	m_score = new Score(this);
	m_pause = new PauseScene();

	m_camAngY = 0;
	m_camAngX = 0;
	m_camPos = {};

	//マウス設定
	m_basePt.x = ScrW / 2;
	m_basePt.y = ScrH / 2;
	ClientToScreen(APP.m_window.GetWndHandle(), &m_basePt);
	SetCursorPos(m_basePt.x, m_basePt.y);

	m_gameFlg = 0;

	m_timeLimit = (TimeLimit::minutes * 60 * 60) + (TimeLimit::seconds * 60);

	//視錐台カリング用
	DirectX::BoundingFrustum::CreateFromMatrix(m_viewArea,
		DirectX::XMMatrixPerspectiveFovLH
		(DirectX::XMConvertToRadians((float)m_player->GetCamAngle()),
			(float)ScrW / (float)ScrH, 0.01f, 1000.0f));

	//裏面カリング
	cullBack = D3D.CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, true, false);
	cullOff = D3D.CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, true, false);

	//出口サウンド
	m_doorSound = std::make_shared<KdSoundEffect>();
	m_doorSound->Load("Data/Gamescene/doorOpen.WAV");
	m_doorSoundInst = m_doorSound->CreateInstance(false);
	m_doorSoundInst->SetVolume(0.3f);//音のボリューム

	//出口マウス用_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
	m_exitTex.Load("Data/GameScene/Exit.png");
	Vertex v[4];

	v[0].pos = { -0.5f, 0.5f,0 };
	v[1].pos = { 0.5f,0.5f,0 };
	v[2].pos = { -0.5f, -0.5f,0 };
	v[3].pos = { 0.5f,-0.5f,0 };

	v[0].color = { 1,1,1,1 };
	v[1].color = { 1,1,1,1 };
	v[2].color = { 1,1,1,1 };
	v[3].color = { 1,1,1,1 };

	v[0].uv = { 0,0 };
	v[1].uv = { 1,0 };
	v[2].uv = { 0,1 };
	v[3].uv = { 1,1 };

	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = v;
	m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DEFAULT, &srd);
	//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

	m_ditherTex.Load("Data/Gamescene/dither.png");
	D3D.GetDevContext()->PSSetShaderResources(5, 1, m_ditherTex.GetSRViewAddress());
}

GameScene::~GameScene()
{

	delete m_player;
	delete m_potionEffectMan;
	delete m_enemyManager;
	delete m_score;
	delete m_treMan;
	delete m_wall;
	delete m_pause;

	m_exitTex.Release();
	cullBack->Release();
	cullOff->Release();
}

const bool GameScene::GoalFlg() const
{
	const Math::Vector3& playerPos = m_player->GetPos();
	Math::Vector3 goalPos = { 90, 0, -30 };
	Math::Vector3 vec = goalPos - playerPos;
	float dis = vec.Length();

	if (dis < 4&& GetAsyncKeyState(MK_RBUTTON) & 0x8000)
	{
		if (SYSTEM.GetRbuttonFlg() == false)
		{
			SYSTEM.SetRbuttonFlg(true);

			m_doorSoundInst->Play(false);
			return true;
		}
	}
	else
	{
		SYSTEM.SetRbuttonFlg(false);
		return false;
	}
	return false;

}

const void GameScene::ExitTexDraw()
{
	const Math::Vector3& playerPos = m_player->GetPos();
	Math::Vector3 goalPos = { 90, 0, -30 };
	Math::Vector3 vec = goalPos - playerPos;
	float dis = vec.Length();

	if (dis < 4)
	{
		//カメラの行列を取得
		Math::Matrix tmpMat = SHADER.m_cb7_Camera.Work().mV;

		tmpMat._41 = 0;
		tmpMat._42 = 0;
		tmpMat._43 = 0;

		tmpMat = tmpMat.Invert();

		m_exitMat = tmpMat * DirectX::XMMatrixTranslation(90, 5, -34);

		SHADER.m_effectShader.SetToDevice();

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_vb.GetAddress(), &stride, &offset);

		D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		D3D.GetDevContext()->PSSetShaderResources(0, 1, m_exitTex.GetSRViewAddress());

		SHADER.m_effectShader.SetWorldMatrix(m_exitMat);
		SHADER.m_effectShader.WriteToCB();

		D3D.GetDevContext()->Draw(4, 0);
	}
}

bool ModelIntersects(const KdModel& _model, const Math::Matrix& _mat, const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec)
{
	float nearestDist = FLT_MAX;	//FLT_MAX==float型の最大値

	Math::Vector3 hitVpos[3];//レイが当たったポリゴンの3頂点座標
	Math::Matrix hitMat;//レイが当たったポリゴンの行列

	for (auto&& node : _model.GetAllNodes())
	{
		//メッシュが含まれていないノードなら飛ばす
		if (node.Mesh == nullptr)continue;

		//そのノードのワールド行列を取得
		Math::Matrix worldMat = node.LocalTransform * _mat;

		//逆行列化
		Math::Matrix inv = worldMat.Invert();

		//逆行列を使ってローカル座標に変換
		Math::Vector3 localPos = DirectX::XMVector3Transform(_rayPos, inv);
		Math::Vector3 localDir = DirectX::XMVector3TransformNormal(_rayVec, inv);

		//拡大行列を使っているメッシュ用にレイの長さを計っておく
		float rayLen = localDir.Length();

		//方向は必ず長さ１にする
		localDir.Normalize();

		//メッシュ内の全ポリゴンを回す
		for (auto&& face : node.Mesh->GetFaces())
		{
			float dist;

			Math::Vector3 vPos[3];//今回レイ判定するポリゴンの3頂点座標
			vPos[0] = node.Mesh->GetVertexPositions()[face.Idx[0]];
			vPos[1] = node.Mesh->GetVertexPositions()[face.Idx[1]];
			vPos[2] = node.Mesh->GetVertexPositions()[face.Idx[2]];

			bool hit = DirectX::TriangleTests::Intersects(
				localPos,
				localDir,
				vPos[0],//１頂点目の座標
				vPos[1],//２頂点目の座標
				vPos[2],//３頂点目の座標
				dist	//レイが当たったところまでの距離(参照型)
			);

			if (hit == false)continue;

			//拡大分を反映
			dist /= rayLen;

			//もっとも近い距離を残す
			if (dist < nearestDist)
			{
				nearestDist = dist;

				//レイが当たったポリゴンの頂点座標を保存
				hitVpos[0] = vPos[0];
				hitVpos[1] = vPos[1];
				hitVpos[2] = vPos[2];

				//レイが当たったポリゴンの行列を保存
				hitMat = worldMat;
			}
		}
	}
	if (nearestDist == FLT_MAX)
	{
		return false;
	}
	else {
		_dis = nearestDist;

		//ポリゴンの縁（辺）を表すベクトルを求める
		Math::Vector3 vec1, vec2;
		vec1 = hitVpos[1] - hitVpos[0];
		vec2 = hitVpos[2] - hitVpos[0];
		//ポリゴン2辺に対する外角（＝そのポリゴンの法線
		_normalVec = DirectX::XMVector3Cross(vec1, vec2);
		//法線を現在のモデルの向きに合わせて回転
		_normalVec = DirectX::XMVector3TransformNormal(_normalVec, hitMat);
		//長さ１
		_normalVec.Normalize();

		return true;
	}
}