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
	//�����L�[�̌^�A�l�̌^
};

#define TEXTUREMANAGER TextureManager::GetInstance()