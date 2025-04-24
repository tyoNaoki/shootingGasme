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
		DeleteGraph(x);//画像消去
	}
}

int GraphicBase::GetGHandle(int frame)
{
	//アクションの描画枚数越えはエラー
	if(graph.size()<= frame){
		return -1;
	}

	return graph[frame];
}

int GraphicBase::GetStartFrame(std::string actionName)
{
	//最初の画像フレーム取得
	return mActions[actionName].startFrame;
}

int GraphicBase::GetInterval(std::string actionName)
{
	//描画インターバル取得
	return mActions[actionName].interval;
}

int GraphicBase::GetActionAllNum()const
{
	//全てのアクションの描画合計数
	return mActionAllNum;
}

Vector2D<int> GraphicBase::GetSize(std::string actionName)
{
	//描画枚数
	return mActions[actionName].size;
}

float GraphicBase::GetPlaySpeed(std::string actionName)
{
	//描画速度取得
	return mActions[actionName].playSpeed;
}

bool GraphicBase::IsLoop(std::string actionName)
{
	//ループするか
	return mActions[actionName].IsLoop;
}

bool GraphicBase::IsLoadGraphic(std::string actionName)
{
	//アクション画像が一つもロードされていない
	if(mActions.size() == 0){
		return false;
	}

	auto it = mActions.find(actionName);
	return it!=mActions.end();
}
