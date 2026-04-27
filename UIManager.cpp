#include "UIManager.h"
#include "DxLib.h"

void UIManager::AddCanvas(std::string name,std::shared_ptr<Canvas> canvas)
{
	mCanvasList[name] = canvas;
}

std::shared_ptr<Canvas> UIManager::GetCanvas(std::string name)
{
	auto it = mCanvasList.find(name);
	//キャンバスの取得
	if(it != mCanvasList.end()){
		return it->second;
	}

	return nullptr;
}

void UIManager::Update(float deltaTime)
{
	//更新
	for(auto &u :mCanvasList){
		u.second->Update(deltaTime);
	}
}

void UIManager::Draw(float deltaTime)
{
	//描画
	for(auto &u: mCanvasList){
		u.second->Draw(deltaTime);
	}
}

void UIManager::ClearCanvas()
{
	mCanvasList.clear();
}

bool UIManager::RemoveCanvas(std::string name)
{
	//対象の名前のキャンバスを削除
	auto itr = mCanvasList.find(name);
	if (itr != mCanvasList.end()) {
		mCanvasList.erase(itr);
	}else{
		return false;
	}
}

void UIManager::SetVisibiltyToCanvas(std::string name, bool isVisibilty)
{
	//キャンバスの表示、非表示切り替え
	auto itr = mCanvasList.find(name);
	if(itr != mCanvasList.end()){
		itr->second->SetVisibilty(isVisibilty);
	}
}

UIManager::~UIManager()
{
	ClearCanvas();
}
