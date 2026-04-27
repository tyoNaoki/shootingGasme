#include "UIText.h"

UIText::~UIText()
{
	//テキスト消去
	DeleteFontToHandle(mFontHandle);
}

void UIText::Update(float deltaTime)
{
	
}

void UIText::Draw(float deltaTime)
{
	//テキスト表示するか
	if(!IsVisible()){return;}
	//テキストの描画
	DrawStringToHandle(mLocalPosition.x,mLocalPosition.y,mText.c_str(),mColor, mFontHandle);
}

void UIText::SetText(const std::string& text)
{
	mText = text;
}

int UIText::GetTextWidth()
{
	//テキストの幅取得
	return GetDrawNStringWidth(mText.c_str(),strlen(mText.c_str()));
}

