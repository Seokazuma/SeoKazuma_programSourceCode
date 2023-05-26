#include"Headers.h"

void SceneManager::DoScene()
{
	//現在のシーンと異なるシーンが登録されていたら
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
		//現在のシーンを実行
		m_currentScene->Frame();
	}
}

void SceneManager::ChangeScene(BaseScene* _nextScene)
{
	//移行したい次のシーンを登場
	m_nextScene = _nextScene;
}

SceneManager::~SceneManager()
{
	if (m_currentScene != nullptr)
	{
		delete m_currentScene;
	}
}
