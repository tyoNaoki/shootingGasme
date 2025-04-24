#include "UIControllDescription.h"


UIControllDescription::UIControllDescription(Vector2D<float> localPosition)
{
    //枠座標、大きさ設定
    auto rectPosi = Vector2D<float>(localPosition.x - 30, localPosition.y - 30);
    auto size = Vector2D<int>(600,500);
    mRects.push_back(UI::Rect(rectPosi, size, UI::gray, true));

    //テキスト設定
    mFontHandle = CreateFontToHandle("ＭＳゴシック", 40, 2);
    mColor = UI::black;
    mTexts.push_back("操作説明");
    mTexts.push_back("");
    mTexts.push_back("WASD / 矢印キー　: 移動");
    mTexts.push_back("SPACE : 決定");
    mTexts.push_back("ESC / P :  ポーズ画面にする");
    mTexts.push_back("");
    mTexts.push_back("SPACE で 操作説明を閉じる");

    //座標設定
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
    //枠描画
    for(auto &x:mRects){
        DrawBox(x.mLeftTop.x, x.mLeftTop.y, x.RightBottom().x, x.RightBottom().y, x.mColor, x.mFillFlag);
    }
    //テキスト描画
    const float textSpace = 30;
    const float textSize = 30;
    for(int i = 0;i < mTexts.size();i++){
        auto text = mTexts[i];
        if(text == ""){continue;}//空行はスキップ
        float drawPosiY = mLocalPosition.y + i * textSpace + i*textSize;
        DrawStringToHandle(mLocalPosition.x, drawPosiY, text.c_str(), mColor, mFontHandle);
    }
}

