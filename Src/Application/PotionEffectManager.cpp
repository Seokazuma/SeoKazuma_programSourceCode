#include"Headers.h"

PotionEffectManager::PotionEffectManager(GameScene* _gameScene)
{
	m_gameScene = _gameScene;
	m_List.clear();
}

PotionEffectManager::~PotionEffectManager()
{
	auto it = m_List.begin();

	for (auto& tre : m_List)
	{
		delete tre;
	}

	m_List.clear();
}

void PotionEffectManager::Update(Math::Matrix _mat)
{

	for (auto& smo : m_List)
	{
		smo->Update(_mat);

	}
	auto it = m_List.begin();

	while (it != m_List.end())
	{
		if ((*it)->GetAliveFlg() == false)
		{
			//ŽÀ‘Ì‰»‚µ‚½ƒf[ƒ^Á‚·
			delete (*it);
			//˜gÁ‚·
			it = m_List.erase(it);

		}
		else
		{
			++it;
		}
	}

}

void PotionEffectManager::Draw()
{
	for (auto& smo : m_List)
	{
		smo->Draw();
	}
}

const void PotionEffectManager::Add(PotionEffect* _smo)
{
	m_List.push_back(_smo);
}
