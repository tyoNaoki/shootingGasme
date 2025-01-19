#pragma once
#include "UIText.h"
class UIResultText :
    public UIText
{
public:
    UIResultText(int fontHandle, unsigned int color);

    ~UIResultText();

    virtual void Update(float deltaTime)override;

    virtual void Draw(float deltaTime)override;

private:
    std::vector<std::string>results;
};

