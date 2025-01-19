#pragma once
#include "UIBase.h"

class UIControllDescription :
    public UIBase
{
public:
    UIControllDescription(Vector2D<float>localPosition);

    ~UIControllDescription();

    virtual void Update(float deltaTime)override{};

    virtual void Draw(float deltaTime)override;

    unsigned int mColor = -1;

private:
    //��������e�L�X�g
    std::vector<std::string> mTexts;
    //�g�g��
    std::vector<UI::Rect> mRects;
    int mFontHandle;
};

