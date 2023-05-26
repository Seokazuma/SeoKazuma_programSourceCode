#pragma once
class TextureManager
{
public:
	KdTexture* GetTexture(const std::string _fileName);

	static TextureManager& GetInstance()
	{
		static TextureManager TexMan;

		return TexMan;
	}

	void AllRelease();

	~TextureManager();

private:
	TextureManager()
	{

	}

	std::map<std::string, KdTexture*>m_texureList;
	//検索キーの型、値の型
};

#define TEXTUREMANAGER TextureManager::GetInstance()