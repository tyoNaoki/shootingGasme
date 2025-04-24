#include "UIControllDescription.h"


UIControllDescription::UIControllDescription(Vector2D<float> localPosition)
{
    //�g���W�A�傫���ݒ�
    auto rectPosi = Vector2D<float>(localPosition.x - 30, localPosition.y - 30);
    auto size = Vector2D<int>(600,500);
    mRects.push_back(UI::Rect(rectPosi, size, UI::gray, true));

    //�e�L�X�g�ݒ�
    mFontHandle = CreateFontToHandle("�l�r�S�V�b�N", 40, 2);
    mColor = UI::black;
    mTexts.push_back("�������");
    mTexts.push_back("");
    mTexts.push_back("WASD / ���L�[�@: �ړ�");
    mTexts.push_back("SPACE : ����");
    mTexts.push_back("ESC / P :  �|�[�Y��ʂɂ���");
    mTexts.push_back("");
    mTexts.push_back("SPACE �� ������������");

    //���W�ݒ�
    SetLocalPosition(localPosition);
}

UIControllDescription::~UIControllDescription()
{
    mTexts.clear();
    DeleteFontToHandle(mFontHandle);
}

void UIControllDescription::Draw(float deltaTime)
{
    if (!IsVisible()) { return; }
    //�g�`��
    for(auto &x:mRects){
        DrawBox(x.mLeftTop.x, x.mLeftTop.y, x.RightBottom().x, x.RightBottom().y, x.mColor, x.mFillFlag);
    }
    //�e�L�X�g�`��
    const float textSpace = 30;
    const float textSize = 30;
    for(int i = 0;i < mTexts.size();i++){
        auto text = mTexts[i];
        if(text == ""){continue;}//��s�̓X�L�b�v
        float drawPosiY = mLocalPosition.y + i * textSpace + i*textSize;
        DrawStringToHandle(mLocalPosition.x, drawPosiY, text.c_str(), mColor, mFontHandle);
    }
}

