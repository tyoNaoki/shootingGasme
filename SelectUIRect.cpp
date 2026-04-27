#include "SelectUIRect.h"
#include "UIText.h"

SelectUIRect::~SelectUIRect()
{
}

void SelectUIRect::Update(float deltaTime)
{
	//選択時の点滅処理
	if(mIsSelect &&mIsFlash){
		currentTime-=deltaTime;
	}else{
		currentTime = flashTime;
	}
	
	if(!mText){return;}
	//テキスト更新
	mText->Update(deltaTime);
}

void SelectUIRect::Draw(float deltaTime)
{
	//枠描画
	DrawBox(mRect.mLeftTop.x, mRect.mLeftTop.y, mRect.RightBottom().x, mRect.RightBottom().y,mRect.mColor, mRect.mFillFlag);
	//テキストの色決定
	if (mText) {
		//選択時
		if (mIsSelect) {
			mText->mColor = mSelectColor;
		}
		else {//非選択時
			mText->mColor = mDefaultColor;
		}
		//テキスト描画
		mText->Draw(deltaTime);
	}
}

void SelectUIRect::SetText(std::shared_ptr<UIText>text)
{
	//テキスト設定
	mText = text;
	//描画中心位置計算
	Vector2D<int>center(mRect.mLeftTop.x+mRect.mSize.x/2,mRect.mLeftTop.y+mRect.mSize.y/2);
	//テキストの縦、横幅取得
	int textWidth = mText->GetTextWidth();
	int fontHeight = GetFontSize();
	//描画中心位置微調整
	int centerX = (center.x - textWidth+5);
	int centerY = center.y - fontHeight;
	//描画座標設定
	mText->SetLocalPosition(Vector2D<float>(centerX,centerY));
	//初期の色設定
	mDefaultColor = mText->mColor;
}

