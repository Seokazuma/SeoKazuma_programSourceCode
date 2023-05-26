#pragma once

class TitleScene :public BaseScene
{
public:
	TitleScene();

	void Update()override;

	void Draw()override;

	void SetCamera()override;

	~TitleScene();

	//キーフレームアニメーション関数
	void AnimeProc();

private:
	KdModel* m_model;
	Math::Matrix m_mat;
	KdModel* m_ceilingModel;
	Math::Matrix m_ceilingMat;
	KdModel* m_diamondModel;
	Math::Matrix m_diamondMat;
	KdTexture* m_TitleTex;
	Math::Matrix m_TitleMat;
	KdTexture* m_EnterTex;
	Math::Matrix m_EnterMat;
	KdTexture* m_backTex;

	KdTexture* m_Level1Tex;
	Math::Matrix m_Level1Mat;
	KdTexture* m_Level2Tex;
	Math::Matrix m_Level2Mat;
	KdTexture* m_Level3Tex;
	Math::Matrix m_Level3Mat;

	Math::Matrix m_startMat;
	Math::Matrix m_endMat;

	float m_animeFrame;
	float m_ang;
	float m_backSpeed;
	bool m_backFlg;
	uint8_t m_checkFlg;
	bool m_startClickFlg;
	bool m_levelFlg;

	int m_gameLevel;

	DirectX::SpriteFont* spriteFont;

	std::shared_ptr<KdSoundEffect>m_soundBgm;
	std::shared_ptr<KdSoundInstance>m_soundBgmInst;

	std::shared_ptr<KdSoundEffect>m_soundClick;
	std::shared_ptr<KdSoundInstance>m_soundClickInst;

	std::shared_ptr<KdSoundEffect>m_soundSelection;
	std::shared_ptr<KdSoundInstance>m_soundSelectionInst;

	std::shared_ptr<KdSoundEffect>m_doorSound;
	std::shared_ptr<KdSoundInstance>m_doorSoundInst;

	ID3D11RasterizerState* cullBack;
	ID3D11RasterizerState* cullOff;
};