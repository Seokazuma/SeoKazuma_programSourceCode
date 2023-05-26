#pragma once

class ModelManager
{
public:
	KdModel* GetModel(const std::string _fileName);

	static ModelManager& GetInstance()
	{
		static ModelManager modelMan;

		return modelMan;
	}
	~ModelManager();
	void AllRelease();

private:
	ModelManager()
	{

	}
	std::map<std::string, KdModel*>m_modelList;
	//検索キーの型、値の型

};

#define MODELMANAGER ModelManager::GetInstance()