#pragma once
#include "UIBase.h"
#include <string>

class UIText :
    public UIBase
{
public:
    UIText(const std::string& text,int fontHandle,unsigned int color):mText(text),mFontHandle(fontHandle),mColor(color),mDefaultColor(color){;}

    ~UIText();
    //�X�V�A�`��
    virtual void Update(float deltaTime)override;
    virtual void Draw(float deltaTime)override;

    //�e�L�X�g�ݒ�
    void SetText(const std::string& text);
    //�e�L�X�g���擾
    int GetTextWidth();

    unsigned int mColor = -1;

private:
    std::string mText;
    int mFontHandle;
    //�����F
    unsigned int mDefaultColor;
};

