#pragma once

class Wall
{
public:
	Wall(GameScene* _gameScene);
	~Wall();

	void Update();
	void Draw();

	//�}�b�v�\���p
	void MapTexDraw();

	//�O�㍶�E�ɕǂ����邩����
	const void HitWall(Math::Vector3& _pos, float& _size, bool& _upXHit, bool& _downXHit, bool& _upZHit, bool& _downZHit,Math::Matrix& _wallXMat,Math::Matrix& _wallZMat,float& _wallsize)const;
	//�ǂƂ̓����蔻��
	const void CollisionDetection(Math::Matrix _mat, Math::Vector3& _pos, float& _size, float& _wallsize, Math::Vector3& _dis, float& _sizeSum,bool& _hit,Math::Matrix& _wallMat)const;
	//���ׂ������W���}�b�v��̉����𒲂ׂ�
	const int ChecMapGrid(const int& _x, const int& _z)const;
	//��ԋ߂������n�_��T��
	const Math::Matrix NearDestination(const Math::Matrix& _mat)const;
	//�ǃ��C����
	const bool CheckWall(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec) const;
	//�}�b�v��̌����n�_�̐��𒲂ׂ�
	const int GetIntersectionNum()const;
	//�G���瑗���Ă���_spawnNum�œG�̃X�|�[���n�_�����߂�
	const Math::Vector3 GetIntersectionPos(const int& _intersectionNum)const;

private:


	KdModel* m_model[4];
	KdModel* m_invisibleModel;

	//�\������}�b�v�̃e�N�X�`��
	KdTexture* m_mapWallTex;
	KdTexture* m_mapSpaceTex;
	KdTexture* m_mapPlayerTex;

	//�����n�_�ɂ�����̋���
	float m_lanternPow;
	bool m_lanternFlg;

	//�X�^�[�g�n�_�̃}�b�v���
	int m_startMap[7][4] = { {0,0,0,0},
							{0,2,2,2},
							{0,2,2,2},
							{3,2,2,2},
							{0,2,2,2},
							{0,2,2,2},
							{0,0,0,0} };

	//�Q�[�����x��
	int m_gameLevel;

	//���C���}�b�v
	int m_map[27][26];

	//�}�b�v���W
	Math::Matrix m_startMat[7][4];
	Math::Matrix m_mat[27][26];
	Math::Matrix m_invisibleMat;

	GameScene* m_gameScene;
};
