#pragma once
#include<memory>
#include<vector>
#include "Vector2D.h"
#include <unordered_map>

//�摜�f�[�^���
struct GraphicInfo {
	std::vector<const char*> mFileNames;
	int mAllNum;
	int mXnum;
	int mYnum;
	int mXSize;
	int mYSize;
};

struct ActionInfo {
	int startFrame;
	int interval;
	Vector2D<int> size;
	float playSpeed = 1.0f;
	bool IsLoop = true;
};

class GraphicBase
{
public:
	//GraphicBase(int* handle,const GraphicInfo&);
	GraphicBase(int actionAllNum,std::vector<int>& handle,const std::unordered_map<std::string,ActionInfo>actions);

	//��摜�p
	GraphicBase();

	~GraphicBase();

	int GetGHandle(int frame); //�摜�擾
	int GetStartFrame(std::string actionName);
	int GetInterval(std::string actionName); //�ő�t���[���擾
	int GetActionAllNum()const; //�A�N�V�����̎�ނ̐��擾
	Vector2D<int>GetSize(std::string actionName);//�P�A�N�V�����̕`�文���擾
	float GetPlaySpeed(std::string actionName);//�`��X�s�[�h�擾

	bool isDummy()const{return dummy;}//�_�~�[�摜�t���O�擾
	bool IsLoop(std::string actionName);//���[�v���邩

	bool IsLoadGraphic(std::string actionName); //�ǂݍ��߂Ă��邩�ǂ���

private:
	std::vector<int>graph;//�摜

	int mInterval; //�ő�t���[��
	int mNumColumns;//�A�N�V�����̎�ނ̐�
	int mActionAllNum;//�S�A�N�V�����`�捇�v��

	//������Ȃ��ꍇ�A�������́A�T�C�Y0�̏ꍇ�ADummy�ɍ����ւ�
	std::unordered_map<std::string,ActionInfo>mActions;

	bool dummy = false;//�_�~�[�t���O
};

