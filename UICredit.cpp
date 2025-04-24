#include "UICredit.h"

UICredit::UICredit(Vector2D<float> localPosition)
{
    //�g���W�A�傫���ݒ�
    auto rectPosi = Vector2D<float>(localPosition.x - 30, localPosition.y - 30);
    auto size = Vector2D<int>(1500,650);
    mRects.push_back(UI::Rect(rectPosi, size, UI::gray, true));

    //�e�L�X�g�ݒ�
    mFontHandle = CreateFontToHandle("�l�r�S�V�b�N", 30, 2);
    mColor = UI::black;
    mTexts.push_back("�N���W�b�g");
    mTexts.push_back("��l�� by Sventhole (https://sventhole.itch.io/hero-knight)");
    mTexts.push_back("�G���G�A����̃G�t�F�N�g�֌W by Luiz Melo (https://luizmelo.itch.io/monsters-creatures-fantasy)");
    mTexts.push_back("Boss�A����̃G�t�F�N�g�֌W by Kronovi (https://darkpixel-kronovi.itch.io/)");
    mTexts.push_back("�X�e�[�W�w�i�`�b�v : Tiny Texture Packs (https://screamingbrainstudios.itch.io/tiny-texture-pack)");
    mTexts.push_back("Clashing Blades BGM: by Seventh Beast");
    mTexts.push_back("���̃��\�[�X��Casper Gaming���p�K��̑Ώۂł�: https://www.caspergaming.com/terms-of-use/");
    mTexts.push_back("Free Laser Weapon Sound Library: by @danielsoundsgood");
    mTexts.push_back("https://danielsoundsgood.itch.io/laser-weapon-sound-library");
    mTexts.push_back("SPACE�ŕ���");

    //���W�ݒ�
    SetLocalPosition(localPosition);
}

UICredit::~UICredit()
{
    mTexts.clear();
    DeleteFontToHandle(mFontHandle);
}

void UICredit::Draw(float deltaTime)
{
    if (!IsVisible()) { return; }
    //�g�`��
    for (auto& x : mRects) {
        DrawBox(x.mLeftTop.x, x.mLeftTop.y, x.RightBottom().x, x.RightBottom().y, x.mColor, x.mFillFlag);
    }
    //�e�L�X�g�`��
    const float textSpace = 30;
    const float textSize = 30;
    for (int i = 0; i < mTexts.size(); i++) {
        auto text = mTexts[i];
        if (text == "") { continue; }//��s�̓X�L�b�v
        float drawPosiY = mLocalPosition.y + i * textSpace + i * textSize;
        DrawStringToHandle(mLocalPosition.x, drawPosiY, text.c_str(), mColor, mFontHandle);
    }
}
