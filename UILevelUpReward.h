#pragma once
#include "UIBase.h"
#include "StatusManager.h"

class UIText;


class UILevelUpReward :
    public UIBase
{
public:
    UILevelUpReward(UI::Rect rect, unsigned int selectColor, bool isFlash,int nameFontHandle,int descriptionFontHandle);

    ~UILevelUpReward();
    //更新、描画
    void Update(float deltaTime)override;
    void Draw(float deltaTime)override;
    //報酬の効果を設定
    void SetReward(std::shared_ptr<LevelUpBonus>reward);
    //描画位置設定
    void SetLocalPosition(Vector2D<float>localPosition)override;
    //報酬をプレイヤーに付与する
    void ApplyReward();

    bool mIsSelect = false;

private:
    //選択するときの枠組み
    UI::Rect mRect;
    //報酬の効果
    std::shared_ptr<LevelUpBonus>mReward;
    //色やテキストなどのハンドル
    unsigned int mSelectColor;
    unsigned int mDefaultColor;
    int mNameFontHandle;
    int mDesFontHandle;
    //文字の点滅
    bool mIsFlash;
    float flashTime = 0.2f;
    float currentTime = 0.0f;
};

