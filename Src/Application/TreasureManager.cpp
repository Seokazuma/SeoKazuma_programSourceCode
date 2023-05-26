#include"Headers.h"

TreasureManager::TreasureManager(GameScene* _gameScene)
{
	m_List.clear();
	m_treasureType = 0;

	m_treasureSound = std::make_shared<KdSoundEffect>();
	m_treasureSound->Load("Data/Item/treasureGet.WAV");
	m_treasureInst = m_treasureSound->CreateInstance(false);
	m_treasureInst->SetVolume(0.1f);

	m_gameScene = _gameScene;

	//ダイア・ポーション座標
	int treNum = 0;
	Math::Vector3 existTrePos[100];
	while (treNum < 100)
	{
		for (int x = 0; x < 27; ++x)
		{
			for (int z = 0; z < 26; ++z)
			{
				int i = rand() % 26;
				if (i == 0)
				{
					Math::Vector3 trePos = { 10.0f * x , 0, 10.0f * z };
					bool treFlg = true;
					for (int j = 0; j < treNum; ++j)
					{
						if (trePos == existTrePos[j])
						{
							treFlg = false;
							break;
						}
					}
					const Wall& wall = m_gameScene->GetWall();
					int map = wall.ChecMapGrid(x, z);
					if (map != 0&& treFlg==true)
					{
						Math::Matrix treMat;
						int choiceTre = rand() % 2;
						if (choiceTre == 0)
						{
							Math::Matrix treMat = DirectX::XMMatrixTranslation(trePos.x, 0, trePos.z);
							m_diamond = new Diamond(m_gameScene, treMat);
							Add(m_diamond);
							existTrePos[treNum] = trePos;
						}
						if (choiceTre == 1)
						{
							Math::Matrix treMat = DirectX::XMMatrixTranslation(trePos.x, 0, trePos.z);
							m_portion = new Potion(m_gameScene, treMat);
							Add(m_portion);
							existTrePos[treNum] = trePos;

						}
						++treNum;
						if (treNum >= 100)break;
					}
				}
			}
			if (treNum >= 100)break;
		}
	}

}

TreasureManager::~TreasureManager()
{
	auto it = m_List.begin();

	for (auto& tre : m_List)
	{
		delete tre;
	}

	m_List.clear();
}

void TreasureManager::Update()
{
	for (auto& tre : m_List)
	{
		tre->Update();

	}

	auto it = m_List.begin();

	m_treasureType = 0;

	while (it != m_List.end())
	{
		if ((*it)->GetTreasureType() == TreasureType::speedPotion)
		{
			m_treasureType = TreasureType::speedPotion;
		}
		else if ((*it)->GetTreasureType() == TreasureType::recoveryPotion)
		{
			m_treasureType = TreasureType::recoveryPotion;
		}
		else if ((*it)->GetTreasureType() == TreasureType::translucentPotion)
		{
			m_treasureType = TreasureType::translucentPotion;
		}
		else if ((*it)->GetTreasureType() == TreasureType::diamond)
		{
			m_treasureType = TreasureType::diamond;
		}

		if ((*it)->GetAliveFlg() == false)
		{
			//Get音
			m_treasureInst->Play(false);
			//実体化したデータ消す
			delete (*it);
			//枠消す
			it = m_List.erase(it);

		}
		else
		{
			++it;
		}
	}
}

void TreasureManager::Draw()
{

	SHADER.m_standardShader.SetLimLightEnable(true);
	SHADER.m_standardShader.SetLimLightX(2);
	SHADER.m_standardShader.SetLimLightColor(Math::Vector3(1, 2, 2));
	for (auto& tre : m_List)
	{
		tre->Draw();
	}
	SHADER.m_standardShader.SetLimLightEnable(false);

}

void TreasureManager::Add(Treasure* _tre)
{
	m_List.push_back(_tre);
}
