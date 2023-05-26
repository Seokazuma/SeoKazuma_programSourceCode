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

	//�S�[������
	const bool GoalFlg()const;

	//�o���̈ē�Tex
	const void ExitTexDraw();


private:
	//�v���C���[
	Player* m_player;
	//�|�[�V�����p�G�t�F�N�g
	PotionEffectManager* m_potionEffectMan;
	//�G
	EnemyManager* m_enemyManager;
	//�ǃ}�b�v
	Wall* m_wall;
	//�X�R�A�\���E�v�Z
	Score* m_score;
	//����i�_�C�A�E�|�[�V�����j
	TreasureManager* m_treMan;
	//�|�[�Y�V�[��
	PauseScene* m_pause;

	//�Q�[���J�n���ƏI�����̃e�N�X�`��
	KdTexture* m_backTex;

	//�}�b�v���J�����̃{�^���\��
	KdTexture* m_mapOpenKeyTex;

	//�o���̃{�^����\��
	KdTexture m_exitTex;
	Math::Matrix m_exitMat;

	//�f�B�U�����p
	KdTexture m_ditherTex;

	KdBuffer m_vb;

	POINT m_basePt;

	Math::Vector3 m_camPos;
	float m_camAngY;
	float m_camAngX;

	//�Q�[���I���^�C�}�[
	int m_timeLimit;

	//�Q�[�����x���p
	int m_gameLevel;
	
	//�Q�[���V�[���̏I���E�V�[���؂�ւ��Ǘ��p
	uint8_t m_gameFlg;

	enum GameFlg
	{
		m_goalFlg,
		m_pauseFlg,
		m_mapOpenFlg
	};

	//�Q�[���J�n���ƏI�����̈Ó]�̋���
	float m_backTransparency;

	//�h�A�J��
	std::shared_ptr<KdSoundEffect>m_doorSound;
	std::shared_ptr<KdSoundInstance>m_doorSoundInst;

	DirectX::BoundingFrustum m_viewArea;

	//���ʃJ�����O
	ID3D11RasterizerState* cullBack;
	ID3D11RasterizerState* cullOff;
};
