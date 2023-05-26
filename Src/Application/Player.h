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

	//�|�[�V�������g�������H
	const bool GetUsePotionFlg()const
	{
		return m_usePotion;
	}

	//�������̃|�[�V�����g���Ă邩�H
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

	//�|�[�V�����g�p
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

	//�_���[�W��
	std::shared_ptr<KdSoundEffect>m_damageSound;
	std::shared_ptr<KdSoundInstance>m_damageInst;

	//�񕜉�
	std::shared_ptr<KdSoundEffect>m_recoverySound;
	std::shared_ptr<KdSoundInstance>m_recoveryInst;

	//���މ�
	std::shared_ptr<KdSoundEffect>m_toDrinkSound;
	std::shared_ptr<KdSoundInstance>m_toDrinkkInst;

	//���鉹
	std::shared_ptr<KdSoundEffect>m_speedUpSound;
	std::shared_ptr<KdSoundInstance>m_speedUpInst;

	//��������BGM
	std::shared_ptr<KdSoundEffect>m_translucentBgm;
	std::shared_ptr<KdSoundInstance>m_translucentBgmInst;
	
	//��������
	std::shared_ptr<KdSoundEffect>m_translucentSound;
	std::shared_ptr<KdSoundInstance>m_translucentInst;
	
	//����
	std::shared_ptr<KdSoundEffect>m_walkSound;
	std::shared_ptr<KdSoundInstance>m_walkInst;

	//bool m_Flg;

	GameScene* m_gameScene;

	//�|�[�V�����g�p���̊Ǘ��p
	uint8_t m_statusFlg;

	//�����Ă��邩�ۂ�
	bool m_moveFlg;

	int m_hp;
	int m_initialHp;

	//�������̎葫�������t���O
	bool m_walkFlg;
	float m_ang;

	float m_portionTime;
	float m_movePow;

	bool m_usePotion;

	//�葫�̉�]�p�x
	float m_limbsAng;
	float m_cloakAng;

	//�ǂƂ̋���
	float m_wallDis = 1000;

	//�m�b�N�o�b�N
	float m_knockBack;
	float m_knockUp;

	//�f�B�]���u����
	float m_dissolvePow;

	float m_camAngY;

	bool m_damageFlg;

	float m_transparency;

	int m_camAngle;


};
