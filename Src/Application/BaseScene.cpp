#include "Headers.h"

BaseScene::~BaseScene()
{
}

void BaseScene::Frame()
{
	//�X�V����
	Update();

	SHADER.m_cb8_Light.Write();
	SHADER.m_cb8_Light.Work().PL_Num = 0;
	SHADER.m_cb8_Light.Work().SL_Num = 0;

	// �o�b�N�o�b�t�@�N���A
	D3D.GetDevContext()->ClearRenderTargetView(D3D.GetBackBuffer()->GetRTView(), Math::Color(0.2f, 0.2f, 0.2f, 1.0f));//RGBA��0-1�͈̔͂�
	// Z�o�b�t�@�N���A
	D3D.GetDevContext()->ClearDepthStencilView(D3D.GetZBuffer()->GetDSView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//�J�����E���E�ݒ�
	SetCamera();

	DynamicDraw();
	//�`�揈��
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