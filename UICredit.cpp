#include "UICredit.h"

UICredit::UICredit(Vector2D<float> localPosition)
{
    //枠座標、大きさ設定
    auto rectPosi = Vector2D<float>(localPosition.x - 30, localPosition.y - 30);
    auto size = Vector2D<int>(1500,650);
    mRects.push_back(UI::Rect(rectPosi, size, UI::gray, true));

    //テキスト設定
    mFontHandle = CreateFontToHandle("ＭＳゴシック", 30, 2);
    mColor = UI::black;
    mTexts.push_back("クレジット");
    mTexts.push_back("主人公 by Sventhole (https://sventhole.itch.io/hero-knight)");
    mTexts.push_back("雑魚敵、武器のエフェクト関係 by Luiz Melo (https://luizmelo.itch.io/monsters-creatures-fantasy)");
    mTexts.push_back("Boss、武器のエフェクト関係 by Kronovi (https://darkpixel-kronovi.itch.io/)");
    mTexts.push_back("ステージ背景チップ : Tiny Texture Packs (https://screamingbrainstudios.itch.io/tiny-texture-pack)");
    mTexts.push_back("Clashing Blades BGM: by Seventh Beast");
    mTexts.push_back("このリソースはCasper Gaming利用規約の対象です: https://www.caspergaming.com/terms-of-use/");
    mTexts.push_back("Free Laser Weapon Sound Library: by @danielsoundsgood");
    mTexts.push_back("https://danielsoundsgood.itch.io/laser-weapon-sound-library");
    mTexts.push_back("SPACEで閉じる");

    //座標設定
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
    //枠描画
    for (auto& x : mRects) {
        DrawBox(x.mLeftTop.x, x.mLeftTop.y, x.RightBottom().x, x.RightBottom().y, x.mColor, x.mFillFlag);
    }
    //テキスト描画
    const float textSpace = 30;
    const float textSize = 30;
    for (int i = 0; i < mTexts.size(); i++) {
        auto text = mTexts[i];
        if (text == "") { continue; }//空行はスキップ
        float drawPosiY = mLocalPosition.y + i * textSpace + i * textSize;
        DrawStringToHandle(mLocalPosition.x, drawPosiY, text.c_str(), mColor, mFontHandle);
    }
}
