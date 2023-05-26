#include"Headers.h"

PotionEffect::PotionEffect(Math::Matrix _mat, int _porChoice)
{
	m_porChoice = _porChoice;

	Vertex v[4];

	v[0].pos = { -1,1,0 };
	v[1].pos = { 1,1,0 };
	v[2].pos = { -1,-1,0 };
	v[3].pos = { 1,-1,0 };

	v[0].color = { 1,1,1,0.5 };
	v[1].color = { 1,1,1,0.5 };
	v[2].color = { 1,1,1,0.5 };
	v[3].color = { 1,1,1,0.5 };

	m_randomRot = rand() % 360;

	if (_porChoice == TreasureType::speedPotion)
	{
		m_tex.Load("Data/Player/dashSmoke.png");
		m_randomMove.y = (float)((rand() % 10) / 100.0f);
		m_randomMove.x = (float)((rand() % 5 - 2) / 100.0f);
		m_randomMove.z = (float)((rand() % 5 - 2) / 100.0f);
	}
	m_gravity = 0;
	if (_porChoice == TreasureType::recoveryPotion)
	{
		m_tex.Load("Data/Player/recoveryEffect.png");
		m_randomMove.x = (float)((rand() % 5 - 2)/100.0f);
		m_randomMove.z = (float)((rand() % 5 - 2)/100.0f);
		m_gravity = 0.3f;
	}

	v[0].uv = { 0,0 };
	v[1].uv = { 1,0 };
	v[2].uv = { 0,1 };
	v[3].uv = { 1,1 };

	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = v;
	m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DEFAULT, &srd);
	m_playerMat = DirectX::XMMatrixTranslation(_mat._41,_mat._42+0.3f,_mat._43);

	m_aliveFlg = true;

	m_scale = 0.5f;

	m_pos = {};

	m_blendAlpha = D3D.CreateBlendState(KdBlendMode::Alpha);
	m_blendAdd = D3D.CreateBlendState(KdBlendMode::Add);
}

PotionEffect::~PotionEffect()
{
	m_tex.Release();
	m_vb.Release();

	m_blendAlpha->Release();
	m_blendAdd->Release();
}

void PotionEffect::Update(Math::Matrix _mat)
{
	//カメラの行列を取得
	Math::Matrix tmpMat = SHADER.m_cb7_Camera.Work().mV;

	//カメラの座標不要
	tmpMat._41 = 0;
	tmpMat._42 = 0;
	tmpMat._43 = 0;

	//逆行列に変換
	tmpMat =tmpMat.CreateRotationZ((float)m_randomRot) * tmpMat.Invert();

	if (m_porChoice == TreasureType::speedPotion)
	{
		m_scale += 0.05f;

		Math::Matrix scaleMat = DirectX::XMMatrixScaling(m_scale, m_scale, 1);

		m_playerMat._41 += m_randomMove.x;
		m_playerMat._42 += m_randomMove.y;
		m_playerMat._43 += m_randomMove.z;

		m_mat = scaleMat * tmpMat * m_playerMat;

		if (m_scale > 2)
		{
			m_aliveFlg = false;
		}
	}
	if (m_porChoice == TreasureType::recoveryPotion)
	{
		Math::Vector3 playerPos = _mat.Translation();

		m_pos.x += m_randomMove.x;
		m_pos.z += m_randomMove.z;

		m_pos.y += m_gravity;

		Math::Matrix transMat = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

		m_gravity -= 0.01f;
		if (_mat._42 < -1)
		{
			m_aliveFlg = false;
		}
		m_mat =tmpMat* transMat * DirectX::XMMatrixTranslation(playerPos.x,playerPos.y,playerPos.z);
	}
}

void PotionEffect::Draw()
{
	D3D.GetDevContext()->OMSetDepthStencilState(
		D3D.CreateDepthStencilState(true, false), 0);

	//加算半透明に変更
	D3D.GetDevContext()->OMSetBlendState(
		m_blendAdd, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);

	SHADER.m_effectShader.SetToDevice();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	if (m_porChoice == TreasureType::speedPotion)
	{
		D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_vb.GetAddress(), &stride, &offset);

		D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		D3D.GetDevContext()->PSSetShaderResources(0, 1, m_tex.GetSRViewAddress());

		SHADER.m_effectShader.SetWorldMatrix(m_mat);
		SHADER.m_effectShader.WriteToCB();

		D3D.GetDevContext()->Draw(4, 0);
	}
	if (m_porChoice == TreasureType::recoveryPotion)
	{
		D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_vb.GetAddress(), &stride, &offset);

		D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		D3D.GetDevContext()->PSSetShaderResources(0, 1, m_tex.GetSRViewAddress());

		SHADER.m_effectShader.SetWorldMatrix(m_mat);
		SHADER.m_effectShader.WriteToCB();

		D3D.GetDevContext()->Draw(4, 0);
	}

	//通常半透明に変更
	D3D.GetDevContext()->OMSetBlendState(
		m_blendAlpha, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);

	D3D.GetDevContext()->OMSetDepthStencilState(
		D3D.CreateDepthStencilState(true, true), 0);

}
