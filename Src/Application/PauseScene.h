#pragma once

class PauseScene
{
public:
	PauseScene();
	~PauseScene();

	void Update();
	void Draw();

	bool GetPauseFlg();

private:

	KdTexture* m_backTex;
	KdTexture* m_titleBackTex;
	KdTexture* m_sensitivityTex;

	Math::Matrix m_backMat;
	Math::Matrix m_titleBackMat;
	Math::Matrix m_sensitivityMat;

	bool m_pauseFlg;

};