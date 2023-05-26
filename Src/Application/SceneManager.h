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

	BaseScene* m_currentScene = nullptr;//現在実行中シーン
	BaseScene* m_nextScene = nullptr;//次に実行したいシーン

	//State(ステート)ぱたーん
	//デザインパターンの一つ
	//ある状態を表すクラスを個々に作り
	//現在の状態を表すクラス

};

#define SCENEMANAGER SceneManager::GetInstance()