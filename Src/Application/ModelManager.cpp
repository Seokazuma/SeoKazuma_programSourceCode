#include"Headers.h"


KdModel* ModelManager::GetModel(const std::string _fileName)
{
	KdModel* tmpModel;

	if (m_modelList.find(_fileName) == m_modelList.end())
	{
		//_fileName�Ƃ��������L�[�ŒT������
		//�Ō�܂Ō�����Ȃ������ꍇ
		//== ����܂ň�x�����̉摜���ǂݍ��܂�Ă��Ȃ�
		tmpModel = new KdModel;
		tmpModel->Load(_fileName);
		m_modelList[_fileName] = tmpModel;
		//m_modelList��_fileName�Ƃ������� (�����L�[)��
		//���̉摜��ۑ����Ă���A�h���X���L�^����
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
		// itr->first : �����L�[ (����͉摜�t�@�C����)
		// itr->second : �l (����͉摜�f�[�^���ۑ�����Ă���A�h���X)
		(itr->second)->Release();
		delete (itr->second);
		// (itr->second) == ���Ƃ���m_model�ƕ\�����Ă�������
	}
}