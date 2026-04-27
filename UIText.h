#pragma once
#include "UIBase.h"
#include <string>

class UIText :
    public UIBase
{
public:
    UIText(const std::string& text,int fontHandle,unsigned int color):mText(text),mFontHandle(fontHandle),mColor(color),mDefaultColor(color){;}

    ~UIText();
    //更新、描画
    virtual void Update(float deltaTime)override;
    virtual void Draw(float deltaTime)override;

    //テキスト設定
    void SetText(const std::string& text);
    //テキスト幅取得
    int GetTextWidth();

    unsigned int mColor = -1;

private:
    std::string mText;
    int mFontHandle;
    //初期色
    unsigned int mDefaultColor;
};

