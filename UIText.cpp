#include "UIText.h"

UIText::~UIText()
{
	//�e�L�X�g����
	DeleteFontToHandle(mFontHandle);
}

void UIText::Update(float deltaTime)
{
	
}

void UIText::Draw(float deltaTime)
{
	//�e�L�X�g�\�����邩
	if(!IsVisible()){return;}
	//�e�L�X�g�̕`��
	DrawStringToHandle(mLocalPosition.x,mLocalPosition.y,mText.c_str(),mColor, mFontHandle);
}

void UIText::SetText(const std::string& text)
{
	mText = text;
}

int UIText::GetTextWidth()
{
	//�e�L�X�g�̕��擾
	return GetDrawNStringWidth(mText.c_str(),strlen(mText.c_str()));
}

