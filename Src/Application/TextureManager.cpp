#include "Headers.h"

KdTexture* TextureManager::GetTexture(const std::string _fileName)
{
	KdTexture* tmpTex;

	if (m_texureList.find(_fileName)==m_texureList.end())
	{
		//_fileNameという検索キーで探したが
		//最後まで見つからなかった場合

		//これまで１度もその画像が読み込まれていないとき
		tmpTex = new KdTexture;
		tmpTex->Load(_fileName);
		m_texureList[_fileName] = tmpTex;
		//m_texureListの_fileNameという項目（検索キー）に
		//その画像を保存しているアドレスを記録する
	}
	else
	{
		tmpTex = m_texureList[_fileName];
	}

	return tmpTex;
}

void TextureManager::AllRelease()
{
	//std::map<std::string, KdTexture*>::iterator itr;

	//begin()やend()の戻り値であるイテレータ型は
	//autoで宣言することが一般的
	for (auto itr = m_texureList.begin(); itr != m_texureList.end(); itr++)
	{
		//itr->first : 検索キー（今回は画像ファイル名）
		//itr->second : 値（今回は画像データが保存されているアドレス）
		(itr->second)->Release();
		delete (itr->second);
		//delete (itr->second)== もともとm_texと表現していたもの
	}



}

TextureManager::~TextureManager()
{
	AllRelease();
}
