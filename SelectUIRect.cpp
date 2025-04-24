#include "SelectUIRect.h"
#include "UIText.h"

SelectUIRect::~SelectUIRect()
{
}

void SelectUIRect::Update(float deltaTime)
{
	//�I�����̓_�ŏ���
	if(mIsSelect &&mIsFlash){
		currentTime-=deltaTime;
	}else{
		currentTime = flashTime;
	}
	
	if(!mText){return;}
	//�e�L�X�g�X�V
	mText->Update(deltaTime);
}

void SelectUIRect::Draw(float deltaTime)
{
	//�g�`��
	DrawBox(mRect.mLeftTop.x, mRect.mLeftTop.y, mRect.RightBottom().x, mRect.RightBottom().y,mRect.mColor, mRect.mFillFlag);
	//�e�L�X�g�̐F����
	if (mText) {
		//�I����
		if (mIsSelect) {
			mText->mColor = mSelectColor;
		}
		else {//��I����
			mText->mColor = mDefaultColor;
		}
		//�e�L�X�g�`��
		mText->Draw(deltaTime);
	}
}

void SelectUIRect::SetText(std::shared_ptr<UIText>text)
{
	//�e�L�X�g�ݒ�
	mText = text;
	//�`�撆�S�ʒu�v�Z
	Vector2D<int>center(mRect.mLeftTop.x+mRect.mSize.x/2,mRect.mLeftTop.y+mRect.mSize.y/2);
	//�e�L�X�g�̏c�A�����擾
	int textWidth = mText->GetTextWidth();
	int fontHeight = GetFontSize();
	//�`�撆�S�ʒu������
	int centerX = (center.x - textWidth+5);
	int centerY = center.y - fontHeight;
	//�`����W�ݒ�
	mText->SetLocalPosition(Vector2D<float>(centerX,centerY));
	//�����̐F�ݒ�
	mDefaultColor = mText->mColor;
}

