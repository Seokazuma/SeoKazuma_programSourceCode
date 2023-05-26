#pragma once

class BaseScene;

class SceneManager
{
public:
	~SceneManager();

	void DoScene();
	void ChangeScene(BaseScene* _nextScene);

	static SceneManager& GetInstance()
	{
		static SceneManager sceneManager;

		return sceneManager;
	}



private:
	SceneManager()
	{
	}

	BaseScene* m_currentScene = nullptr;//���ݎ��s���V�[��
	BaseScene* m_nextScene = nullptr;//���Ɏ��s�������V�[��

	//State(�X�e�[�g)�ς��[��
	//�f�U�C���p�^�[���̈��
	//�����Ԃ�\���N���X���X�ɍ��
	//���݂̏�Ԃ�\���N���X

};

#define SCENEMANAGER SceneManager::GetInstance()