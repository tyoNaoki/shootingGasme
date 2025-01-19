#pragma once
#include "UIBase.h"
class UICredit :
    public UIBase
{
public:
    UICredit(Vector2D<float>localPosition);

    ~UICredit();

    virtual void Update(float deltaTime)override {};

    virtual void Draw(float deltaTime)override;

    unsigned int mColor = -1;

private:
    //クレジットのテキスト
    std::vector<std::string> mTexts;
    //枠組み
    std::vector<UI::Rect> mRects;
    int mFontHandle;
};

