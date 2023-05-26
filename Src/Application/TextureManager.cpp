#include "Headers.h"

KdTexture* TextureManager::GetTexture(const std::string _fileName)
{
	KdTexture* tmpTex;

	if (m_texureList.find(_fileName)==m_texureList.end())
	{
		//_fileName�Ƃ��������L�[�ŒT������
		//�Ō�܂Ō�����Ȃ������ꍇ

		//����܂łP�x�����̉摜���ǂݍ��܂�Ă��Ȃ��Ƃ�
		tmpTex = new KdTexture;
		tmpTex->Load(_fileName);
		m_texureList[_fileName] = tmpTex;
		//m_texureList��_fileName�Ƃ������ځi�����L�[�j��
		//���̉摜��ۑ����Ă���A�h���X���L�^����
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

	//begin()��end()�̖߂�l�ł���C�e���[�^�^��
	//auto�Ő錾���邱�Ƃ���ʓI
	for (auto itr = m_texureList.begin(); itr != m_texureList.end(); itr++)
	{
		//itr->first : �����L�[�i����͉摜�t�@�C�����j
		//itr->second : �l�i����͉摜�f�[�^���ۑ�����Ă���A�h���X�j
		(itr->second)->Release();
		delete (itr->second);
		//delete (itr->second)== ���Ƃ���m_tex�ƕ\�����Ă�������
	}



}

TextureManager::~TextureManager()
{
	AllRelease();
}
