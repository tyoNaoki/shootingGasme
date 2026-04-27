#pragma once
#include "UIBase.h"
#include <vector>

class TitlePanel :
    public UIBase
{
public:
	TitlePanel(int width, int height);
	~TitlePanel();
	//çXêVÅAï`âÊ
	virtual void Update(float deltaTime) override;
	virtual void Draw(float deltaTime) override;

private:
	TitlePanel() = default;
	void AddUIElement(std::shared_ptr<UIBase>ui);

	std::vector<std::shared_ptr<UIBase>>mUIList;
};

