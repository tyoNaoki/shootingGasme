#include "UILevelUpReward.h"
#include "LevelUpBonus.h"

UILevelUpReward::UILevelUpReward(UI::Rect rect, unsigned int selectColor, bool isFlash,int nameFontHandle,int descriptionFontHandle) :mRect(rect), mSelectColor(selectColor),mNameFontHandle(nameFontHandle),mDesFontHandle(descriptionFontHandle), mIsFlash(isFlash)
{
	mDefaultColor = rect.mColor;
}

UILevelUpReward::~UILevelUpReward()
{
	//�����n���h���̏���
	mReward = nullptr;
	DeleteFontToHandle(mNameFontHandle);
	DeleteFontToHandle(mDesFontHandle);
}

void UILevelUpReward::Update(float deltaTime)
{
	if(!mReward){return;}
	//�����̑I�����̓_��
	if (mIsSelect && mIsFlash) {
		currentTime -= deltaTime;
	}
	else {
		currentTime = flashTime;
	}
}

void UILevelUpReward::Draw(float deltaTime)
{
	//�g��`��
	DrawBox(mRect.mLeftTop.x, mRect.mLeftTop.y, mRect.RightBottom().x, mRect.RightBottom().y, mIsSelect?mSelectColor:mDefaultColor, mRect.mFillFlag);

	if(!mReward){return;}
	//��V�̖��O�A�������擾
	auto name = mReward->GetName();
	auto des = mReward->GetDescription();

	int textPadding = 10; // �e�L�X�g�Ƙg�̊Ԃ̗]��
	int nameX = mRect.mLeftTop.x + textPadding;
	int nameY = mRect.mLeftTop.y + textPadding;

	int color;
	//�I�����́A���틭���͐ԁA�v���C���[�̂g�o�Ȃǂ̋����͗΂ŕ`��
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
	}//��I�����A��
	else {
		color = UI::white;
	}
	//�����`��
	DrawStringToHandle(nameX,nameY, name.c_str(), color,mNameFontHandle);

	// ������Y���W�v�Z
	int fontHeight = GetFontSize();
	int desY = nameY + 10 + fontHeight + textPadding;
	
	// �����̕`�� 
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
