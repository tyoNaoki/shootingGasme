#include "UIBar.h"

void UIBar::Draw(float deltaTime)
{
	if (!IsVisible()) { return; }
	//�����v�Z
	float ratio = mValue / mMaxValue;
	//�g�`��
	DrawBox(mLocalPosition.x, mLocalPosition.y, mLocalPosition.x + static_cast<int>(mWidth * ratio), mLocalPosition.y + mHeight, mBarColor, TRUE);
	DrawBox(mLocalPosition.x, mLocalPosition.y, mLocalPosition.x + mWidth, mLocalPosition.y + mHeight,mRectColor, FALSE);
}

void UIBar::SetValue(float value)
{
	mValue = value;
}

void UIBar::SetMaxValue(float maxVal)
{
	mMaxValue = maxVal;
}

void UIBar::SetColor(int barColor,int rectColor)
{
	//�F�ݒ�
	mBarColor = barColor;
	mRectColor = rectColor;
}

