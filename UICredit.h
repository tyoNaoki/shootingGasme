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
    //�N���W�b�g�̃e�L�X�g
    std::vector<std::string> mTexts;
    //�g�g��
    std::vector<UI::Rect> mRects;
    int mFontHandle;
};

