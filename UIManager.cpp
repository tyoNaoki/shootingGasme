#include "UIManager.h"
#include "DxLib.h"

void UIManager::AddCanvas(std::string name,std::shared_ptr<Canvas> canvas)
{
	mCanvasList[name] = canvas;
}

std::shared_ptr<Canvas> UIManager::GetCanvas(std::string name)
{
	auto it = mCanvasList.find(name);
	//�L�����o�X�̎擾
	if(it != mCanvasList.end()){
		return it->second;
	}

	return nullptr;
}

void UIManager::Update(float deltaTime)
{
	//�X�V
	for(auto &u :mCanvasList){
		u.second->Update(deltaTime);
	}
}

void UIManager::Draw(float deltaTime)
{
	//�`��
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
	//�Ώۂ̖��O�̃L�����o�X���폜
	auto itr = mCanvasList.find(name);
	if (itr != mCanvasList.end()) {
		mCanvasList.erase(itr);
	}else{
		return false;
	}
}

void UIManager::SetVisibiltyToCanvas(std::string name, bool isVisibilty)
{
	//�L�����o�X�̕\���A��\���؂�ւ�
	auto itr = mCanvasList.find(name);
	if(itr != mCanvasList.end()){
		itr->second->SetVisibilty(isVisibilty);
	}
}

UIManager::~UIManager()
{
	ClearCanvas();
}
