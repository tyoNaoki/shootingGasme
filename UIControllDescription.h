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
    //操作説明テキスト
    std::vector<std::string> mTexts;
    //枠組み
    std::vector<UI::Rect> mRects;
    int mFontHandle;
};

