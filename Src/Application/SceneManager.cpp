#include"Headers.h"

void SceneManager::DoScene()
{
	//���݂̃V�[���ƈقȂ�V�[�����o�^����Ă�����
	if (m_nextScene != m_currentScene)
	{
		if (m_currentScene != nullptr)
		{
			delete m_currentScene;
		}

		m_currentScene = m_nextScene;

	}

	if (m_currentScene != nullptr)
	{
		//���݂̃V�[�������s
		m_currentScene->Frame();
	}
}

void SceneManager::ChangeScene(BaseScene* _nextScene)
{
	//�ڍs���������̃V�[����o��
	m_nextScene = _nextScene;
}

SceneManager::~SceneManager()
{
	if (m_currentScene != nullptr)
	{
		delete m_currentScene;
	}
}
