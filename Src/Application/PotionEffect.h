#pragma once

class PotionEffect
{
public:
	PotionEffect(Math::Matrix _mat,int _porChoice);
	~PotionEffect();
	void Update(Math::Matrix _mat);
	void Draw();

	bool GetAliveFlg() { return m_aliveFlg; }

private:
	KdTexture m_tex;
	KdBuffer m_vb;

	Math::Matrix m_mat;
	Math::Matrix m_playerMat;
	
	Math::Vector3 m_pos;
	Math::Vector3 m_randomMove;

	int m_randomRot;
	int m_porChoice;

	bool m_aliveFlg;

	float m_scale;
	float m_gravity;

	ID3D11BlendState* m_blendAlpha;
	ID3D11BlendState* m_blendAdd;
};
