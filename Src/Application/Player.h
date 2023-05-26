#pragma once

class GameScene;
class DashSmokeManager;

class Player
{
public:

	Player(GameScene* _gameScene);

	~Player();

	void Update(float _camAngY);

	void Draw();

	void HpBarDraw();

	const Math::Vector3 GetPos()const
	{
		return m_mat.Translation();
	}
	const Math::Matrix GetMat()const
	{
		return m_mat;
	}

	const int GetHp()const
	{
		return m_hp;
	}

	//ポーションを使ったか？
	const bool GetUsePotionFlg()const
	{
		return m_usePotion;
	}

	//透明化のポーション使ってるか？
	const bool GetTranslucentFlg()const
	{
		if (m_statusFlg & 1 << TreasureType::translucentPotion)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	const int GetCamAngle()const
	{
		return m_camAngle;
	}

	//ポーション使用
	const void UsePotion();

	const void Animation();

	const void MoveKey();

	const void NowLook();

private:

	KdModel* m_model;
	KdModel* m_rightArmModel;
	KdModel* m_leftArmModel;
	KdModel* m_rightLegModel;
	KdModel* m_leftLegModel;
	KdModel* m_cloakModel;

	KdTexture m_dissolveTex;

	KdTexture* m_hpGreenTex;
	KdTexture* m_hpRedTex;
	KdTexture* m_hpBarTex;

	Math::Matrix m_mat;
	Math::Matrix m_rightArmMat;
	Math::Matrix m_leftArmMat;
	Math::Matrix m_rightLegMat;
	Math::Matrix m_leftLegMat;
	Math::Matrix m_cloakMat;

	Math::Vector3 m_pos;
	Math::Vector3 m_hitVec;
	Math::Vector3 m_vec;

	//ダメージ音
	std::shared_ptr<KdSoundEffect>m_damageSound;
	std::shared_ptr<KdSoundInstance>m_damageInst;

	//回復音
	std::shared_ptr<KdSoundEffect>m_recoverySound;
	std::shared_ptr<KdSoundInstance>m_recoveryInst;

	//飲む音
	std::shared_ptr<KdSoundEffect>m_toDrinkSound;
	std::shared_ptr<KdSoundInstance>m_toDrinkkInst;

	//走る音
	std::shared_ptr<KdSoundEffect>m_speedUpSound;
	std::shared_ptr<KdSoundInstance>m_speedUpInst;

	//透明化中BGM
	std::shared_ptr<KdSoundEffect>m_translucentBgm;
	std::shared_ptr<KdSoundInstance>m_translucentBgmInst;
	
	//透明化音
	std::shared_ptr<KdSoundEffect>m_translucentSound;
	std::shared_ptr<KdSoundInstance>m_translucentInst;
	
	//足音
	std::shared_ptr<KdSoundEffect>m_walkSound;
	std::shared_ptr<KdSoundInstance>m_walkInst;

	//bool m_Flg;

	GameScene* m_gameScene;

	//ポーション使用時の管理用
	uint8_t m_statusFlg;

	//歩いているか否か
	bool m_moveFlg;

	int m_hp;
	int m_initialHp;

	//歩く時の手足動かすフラグ
	bool m_walkFlg;
	float m_ang;

	float m_portionTime;
	float m_movePow;

	bool m_usePotion;

	//手足の回転角度
	float m_limbsAng;
	float m_cloakAng;

	//壁との距離
	float m_wallDis = 1000;

	//ノックバック
	float m_knockBack;
	float m_knockUp;

	//ディゾルブ強さ
	float m_dissolvePow;

	float m_camAngY;

	bool m_damageFlg;

	float m_transparency;

	int m_camAngle;


};
