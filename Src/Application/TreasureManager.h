#pragma once

class Potion;
class Diamond;

class TreasureManager
{
public:
	TreasureManager(GameScene* _gameScene);
	~TreasureManager();

	void Update();

	void Draw();

	void Add(Treasure* _tre);

	const int GetTreasureType()const
	{
		return m_treasureType;
	}

	const Math::Matrix GetTreasureMat(int _num)
	{
		return m_treasureMat[_num];
	}

protected:

	std::list<Treasure*>m_List;

	Potion* m_portion;
	Diamond* m_diamond;
	GameScene* m_gameScene;

	std::shared_ptr<KdSoundEffect>m_treasureSound;
	std::shared_ptr<KdSoundInstance>m_treasureInst;

	int m_treasureType;

	Math::Matrix m_treasureMat[100];
};