#include"Headers.h"


KdModel* ModelManager::GetModel(const std::string _fileName)
{
	KdModel* tmpModel;

	if (m_modelList.find(_fileName) == m_modelList.end())
	{
		//_fileNameという検索キーで探したが
		//最後まで見つからなかった場合
		//== これまで一度もその画像が読み込まれていない
		tmpModel = new KdModel;
		tmpModel->Load(_fileName);
		m_modelList[_fileName] = tmpModel;
		//m_modelListの_fileNameという項目 (検索キー)に
		//その画像を保存しているアドレスを記録する
	}
	else
	{
		tmpModel = m_modelList[_fileName];
	}
	return tmpModel;
}

ModelManager::~ModelManager()
{
	AllRelease();
}

void ModelManager::AllRelease()
{
	for (auto itr = m_modelList.begin(); itr != m_modelList.end(); itr++)
	{
		// itr->first : 検索キー (今回は画像ファイル名)
		// itr->second : 値 (今回は画像データが保存されているアドレス)
		(itr->second)->Release();
		delete (itr->second);
		// (itr->second) == もともとm_modelと表現していたもの
	}
}