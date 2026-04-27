#pragma once
#include "UIBase.h"

class UIBar :
    public UIBase
{
public:
    UIBar(float width,float height,float value,float maxValue,int barColor,int rectColor) :mWidth(width),mHeight(height),mValue(value),mMaxValue(maxValue),mBarColor(barColor),mRectColor(rectColor){}

    virtual void Update(float deltaTime) override{};

    virtual void Draw(float deltaTime) override;
    //バーの割合計算のための数値設定
    void SetValue(float value);
    //バーの割合計算の最大数値設定
    void SetMaxValue(float maxVal);
    //色設定
    void SetColor(int barColor,int rectColor);

protected:
    //バーの横縦幅
    int mWidth = 0;
    int mHeight = 0;
private:
    //バーの最大数、現在数値
    float mMaxValue = 0;
    float mValue = 0;
    //色
    int mBarColor = 0;
    int mRectColor = 0;
};

