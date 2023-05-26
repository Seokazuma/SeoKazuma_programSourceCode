#pragma once

class Treasure
{
public:
	Treasure();
	~Treasure();

	virtual void Update();
	virtual void Draw();

	virtual int GetTreasureType()
	{
		return m_treasureType;
	}

	bool GetAliveFlg() { return m_aliveFlg; }

protected:

	Math::Matrix m_mat;

	bool m_aliveFlg;

	int m_treasureType;
};