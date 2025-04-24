#pragma once
#include "UIBase.h"

class UIBar :
    public UIBase
{
public:
    UIBar(float width,float height,float value,float maxValue,int barColor,int rectColor) :mWidth(width),mHeight(height),mValue(value),mMaxValue(maxValue),mBarColor(barColor),mRectColor(rectColor){}

    virtual void Update(float deltaTime) override{};

    virtual void Draw(float deltaTime) override;
    //�o�[�̊����v�Z�̂��߂̐��l�ݒ�
    void SetValue(float value);
    //�o�[�̊����v�Z�̍ő吔�l�ݒ�
    void SetMaxValue(float maxVal);
    //�F�ݒ�
    void SetColor(int barColor,int rectColor);

protected:
    //�o�[�̉��c��
    int mWidth = 0;
    int mHeight = 0;
private:
    //�o�[�̍ő吔�A���ݐ��l
    float mMaxValue = 0;
    float mValue = 0;
    //�F
    int mBarColor = 0;
    int mRectColor = 0;
};

