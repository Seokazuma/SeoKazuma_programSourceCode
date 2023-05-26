#pragma once

class Player;
class PotionEffectManager;
class EnemyWalk;
class EnemyManager;
class Wall;
class TreasureManager;
class Score;
class Treasure;
class PotionManager;
class Inventory;
class PauseScene;

class GameScene :public BaseScene
{
public:

	GameScene(int _gameLevel);

	void SetCamera()override;

	void Update()override;

	void DynamicDraw()override;

	void Draw()override;

	~GameScene();

	const TreasureManager& GetTreMan()
	{
		return *m_treMan;
	}

	const Wall& GetWall()
	{
		return *m_wall;
	}

	const Player& GetPlayer()
	{
		return *m_player;
	}

	const EnemyManager& GetEnemyManager()
	{
		return *m_enemyManager;
	}

	const Score& GetScore()
	{
		return *m_score;
	}

	PotionEffectManager* GetPotionEffectManager()
	{
		return m_potionEffectMan;
	}

	const DirectX::BoundingFrustum& GetViewArea()
	{
		return m_viewArea;
	}

	const Math::Vector3& GetCamPos()
	{
		return m_camPos;
	}

	const int GetTimeLimit()
	{
		return m_timeLimit;
	}

	const void SetGameLevel(int _level)
	{
		m_gameLevel = _level;
	}
	const int GetGameLevel()
	{
		return m_gameLevel;
	}

	//ゴール条件
	const bool GoalFlg()const;

	//出口の案内Tex
	const void ExitTexDraw();


private:
	//プレイヤー
	Player* m_player;
	//ポーション用エフェクト
	PotionEffectManager* m_potionEffectMan;
	//敵
	EnemyManager* m_enemyManager;
	//壁マップ
	Wall* m_wall;
	//スコア表示・計算
	Score* m_score;
	//お宝（ダイア・ポーション）
	TreasureManager* m_treMan;
	//ポーズシーン
	PauseScene* m_pause;

	//ゲーム開始時と終了時のテクスチャ
	KdTexture* m_backTex;

	//マップを開く時のボタン表示
	KdTexture* m_mapOpenKeyTex;

	//出口のボタンを表示
	KdTexture m_exitTex;
	Math::Matrix m_exitMat;

	//ディザ抜き用
	KdTexture m_ditherTex;

	KdBuffer m_vb;

	POINT m_basePt;

	Math::Vector3 m_camPos;
	float m_camAngY;
	float m_camAngX;

	//ゲーム終了タイマー
	int m_timeLimit;

	//ゲームレベル用
	int m_gameLevel;
	
	//ゲームシーンの終了・シーン切り替え管理用
	uint8_t m_gameFlg;

	enum GameFlg
	{
		m_goalFlg,
		m_pauseFlg,
		m_mapOpenFlg
	};

	//ゲーム開始時と終了時の暗転の強さ
	float m_backTransparency;

	//ドア開閉音
	std::shared_ptr<KdSoundEffect>m_doorSound;
	std::shared_ptr<KdSoundInstance>m_doorSoundInst;

	DirectX::BoundingFrustum m_viewArea;

	//裏面カリング
	ID3D11RasterizerState* cullBack;
	ID3D11RasterizerState* cullOff;
};
