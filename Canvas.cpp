#include "Canvas.h"
#include "UIBase.h"

Canvas::~Canvas()
{
	elements.clear();
}

void Canvas::AddElement(std::shared_ptr<UIBase> element)
{
	elements.push_back(element);
}

std::vector<std::shared_ptr<UIBase>> Canvas::GetElements()
{
	return elements;
}

void Canvas::Draw(float deltaTime)
{
	if(!mIsVisible){return;}
	//•`‰æ
	for(auto &x:elements){
		x->Draw(deltaTime);
	}
}

void Canvas::Update(float deltaTime)
{
	if(!mIsVisible){return;}
	//XV
	for(auto &x:elements){
		x->Update(deltaTime);
	}
}

void Canvas::SetVisibilty(bool isVisible)
{
	mIsVisible = isVisible;
}
