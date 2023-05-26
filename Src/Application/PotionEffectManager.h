#pragma once
class PotionEffect;

class PotionEffectManager
{
public:
	PotionEffectManager(GameScene* _gameScene);
	~PotionEffectManager();
	void Update(Math::Matrix _mat);
	void Draw();

	const void Add(PotionEffect* _smo);

private:

	std::list<PotionEffect*>m_List;

	GameScene* m_gameScene;

	Math::Matrix m_mat;
};