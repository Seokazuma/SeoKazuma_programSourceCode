#pragma once

class Enemy
{
public:
	Enemy(GameScene* _gameScene, const Math::Matrix _mat);
	~Enemy();
	void Update();
	void Draw();

	const Math::Matrix GetMat()const
	{
		return m_mat;
	}

	const void SetMat(const Math::Matrix& _mat)
	{
		m_mat = _mat;
	}

	const void SetRotAngY(const float& _rotAngY)
	{
		m_rotAngY = _rotAngY;
	}

	const float GetRotAngY()const
	{
		return m_rotAngY;
	}

	const void SetMovePow(const float& _movePow)
	{
		m_movePow = _movePow;
	}

	const float GetMovePow()const
	{
		return m_movePow;
	}

	const void SetAttackFlg(const bool& _attackFlg)
	{
		m_attackFlg = _attackFlg;
	}

	const void SetVec(const Math::Vector3& _vec)
	{
		m_vec = _vec;
	}

	const Math::Vector3 GetVec()const
	{
		return m_vec;
	}

	const Math::Matrix GetSpotMat()const
	{
		return m_spotMat;
	}
	void SetSpotMat(const Math::Matrix& _spotMat)
	{
		m_spotMat = _spotMat;
	}

	const bool GetDamageFlg()const
	{
		return m_damageFlg;
	}

	const Math::Matrix Rotate()const;
	//当たり判定
	const void CollisionDetection();
	//動きアニメーション
	const void Motion();
	//向いている方向
	const float RotationAngle(const Math::Vector3& _nowVec, Math::Vector3& _destinationVec)const;


	const void SetHitFlg(const bool _hitFlg)
	{
		m_hitFlg = _hitFlg;
	}

	const void SetHitVec(const Math::Vector3& _vec, const int _num)
	{
		m_hitVec[_num] = _vec;

		m_hitNum = _num;
	}

private:

	KdModel* m_model;
	KdModel* m_rightArmModel;
	KdModel* m_leftArmModel;
	KdModel* m_rightLegModel;
	KdModel* m_leftLegModel;

	Math::Matrix m_mat;
	Math::Matrix m_rightArmMat;
	Math::Matrix m_leftArmMat;
	Math::Matrix m_rightLegMat;
	Math::Matrix m_leftLegMat;

	Math::Matrix m_targetMat;

	Math::Matrix m_spotMat;

	Math::Vector3 m_hitVec[100];

	Math::Vector3 m_pos;

	Math::Vector3 m_vec;
	//歩く時の手足動かすフラグ
	bool m_limbsFlg;
	bool m_attackFlg;

	bool m_hitFlg;
	bool m_damageFlg;

	int m_hitNum;

	float m_limbsAng;
	float m_rightArmAng;

	GameScene* m_gameScene;

	float m_rotAngY;
	float m_movePow;

	BaseEnemy* m_currentState = nullptr;

	std::shared_ptr<KdSoundEffect>m_walkSound;
	std::shared_ptr<KdSoundInstance>m_walkInst;
};