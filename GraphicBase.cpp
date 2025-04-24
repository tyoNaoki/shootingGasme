#include "GraphicBase.h"
#include "DxLib.h"

GraphicBase::GraphicBase(int actionAllNum, std::vector<int>& handle,const std::unordered_map<std::string, ActionInfo> actions)
{
	mActionAllNum  = actionAllNum;
	graph = handle;
	mActions = actions;
}

GraphicBase::GraphicBase()
{
	mActionAllNum = 0;
	dummy = true;
}

GraphicBase::~GraphicBase()
{
	for(auto x : graph){
		DeleteGraph(x);//�摜����
	}
}

int GraphicBase::GetGHandle(int frame)
{
	//�A�N�V�����̕`�文���z���̓G���[
	if(graph.size()<= frame){
		return -1;
	}

	return graph[frame];
}

int GraphicBase::GetStartFrame(std::string actionName)
{
	//�ŏ��̉摜�t���[���擾
	return mActions[actionName].startFrame;
}

int GraphicBase::GetInterval(std::string actionName)
{
	//�`��C���^�[�o���擾
	return mActions[actionName].interval;
}

int GraphicBase::GetActionAllNum()const
{
	//�S�ẴA�N�V�����̕`�捇�v��
	return mActionAllNum;
}

Vector2D<int> GraphicBase::GetSize(std::string actionName)
{
	//�`�文��
	return mActions[actionName].size;
}

float GraphicBase::GetPlaySpeed(std::string actionName)
{
	//�`�摬�x�擾
	return mActions[actionName].playSpeed;
}

bool GraphicBase::IsLoop(std::string actionName)
{
	//���[�v���邩
	return mActions[actionName].IsLoop;
}

bool GraphicBase::IsLoadGraphic(std::string actionName)
{
	//�A�N�V�����摜��������[�h����Ă��Ȃ�
	if(mActions.size() == 0){
		return false;
	}

	auto it = mActions.find(actionName);
	return it!=mActions.end();
}
