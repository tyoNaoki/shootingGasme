#include "UILevelUpReward.h"
#include "LevelUpBonus.h"

UILevelUpReward::UILevelUpReward(UI::Rect rect, unsigned int selectColor, bool isFlash,int nameFontHandle,int descriptionFontHandle) :mRect(rect), mSelectColor(selectColor),mNameFontHandle(nameFontHandle),mDesFontHandle(descriptionFontHandle), mIsFlash(isFlash)
{
	mDefaultColor = rect.mColor;
}

UILevelUpReward::~UILevelUpReward()
{
	//文字ハンドルの消去
	mReward = nullptr;
	DeleteFontToHandle(mNameFontHandle);
	DeleteFontToHandle(mDesFontHandle);
}

void UILevelUpReward::Update(float deltaTime)
{
	if(!mReward){return;}
	//文字の選択時の点滅
	if (mIsSelect && mIsFlash) {
		currentTime -= deltaTime;
	}
	else {
		currentTime = flashTime;
	}
}

void UILevelUpReward::Draw(float deltaTime)
{
	//枠を描画
	DrawBox(mRect.mLeftTop.x, mRect.mLeftTop.y, mRect.RightBottom().x, mRect.RightBottom().y, mIsSelect?mSelectColor:mDefaultColor, mRect.mFillFlag);

	if(!mReward){return;}
	//報酬の名前、説明を取得
	auto name = mReward->GetName();
	auto des = mReward->GetDescription();

	int textPadding = 10; // テキストと枠の間の余白
	int nameX = mRect.mLeftTop.x + textPadding;
	int nameY = mRect.mLeftTop.y + textPadding;

	int color;
	//選択時は、武器強化は赤、プレイヤーのＨＰなどの強化は緑で描画
	if (mIsSelect) {
		switch (mReward->GetType())
		{
		case RewardType::WEAPON_UP:
			color = UI::red;
			break;
		case RewardType::STATUS_UP:
			color = UI::green;
			break;
		default:
			color = UI::white;
			break;
		}
	}//非選択時、白
	else {
		color = UI::white;
	}
	//文字描画
	DrawStringToHandle(nameX,nameY, name.c_str(), color,mNameFontHandle);

	// 説明のY座標計算
	int fontHeight = GetFontSize();
	int desY = nameY + 10 + fontHeight + textPadding;
	
	// 説明の描画 
	DrawStringToHandle(nameX, desY, des.c_str(), color,mDesFontHandle);
}

void UILevelUpReward::SetReward(std::shared_ptr<LevelUpBonus>reward)
{
	mReward = reward;
}

void UILevelUpReward::SetLocalPosition(Vector2D<float> localPosition)
{
	mRect.mLeftTop = localPosition;
}

void UILevelUpReward::ApplyReward()
{
	mReward->Apply();
	mReward->UpdateCurrentPlayer();
}
