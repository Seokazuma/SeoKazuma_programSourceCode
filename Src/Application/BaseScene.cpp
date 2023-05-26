#include "Headers.h"

BaseScene::~BaseScene()
{
}

void BaseScene::Frame()
{
	//更新処理
	Update();

	SHADER.m_cb8_Light.Write();
	SHADER.m_cb8_Light.Work().PL_Num = 0;
	SHADER.m_cb8_Light.Work().SL_Num = 0;

	// バックバッファクリア
	D3D.GetDevContext()->ClearRenderTargetView(D3D.GetBackBuffer()->GetRTView(), Math::Color(0.2f, 0.2f, 0.2f, 1.0f));//RGBAを0-1の範囲で
	// Zバッファクリア
	D3D.GetDevContext()->ClearDepthStencilView(D3D.GetZBuffer()->GetDSView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//カメラ・視界設定
	SetCamera();

	DynamicDraw();
	//描画処理
	Draw();

}

void BaseScene::SetCamera()
{
}

void BaseScene::Update()
{
}

void BaseScene::DynamicDraw()
{
}
void BaseScene::Draw()
{
}