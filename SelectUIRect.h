#pragma once
#include "UIBase.h"

class UIText;

// 選択可能なUI矩形クラス
class SelectUIRect : public UIBase {
public:
    // コンストラクタ：矩形、選択色、点滅の有無を初期化
    SelectUIRect(UI::Rect rect, unsigned int selectColor, bool isFlash)
        : mRect(rect), mDefaultColor(rect.mColor), mSelectColor(selectColor), mIsFlash(isFlash) {}

    // デストラクタ
    ~SelectUIRect();

    // 更新関数
    void Update(float deltaTime) override;

    // 描画関数
    void Draw(float deltaTime) override;

    // テキスト設定関数
    void SetText(std::shared_ptr<UIText> text);

    // 選択状態フラグ
    bool mIsSelect = false;

private:
    UI::Rect mRect; // 矩形データ
    std::shared_ptr<UIText> mText; // テキストオブジェクト
    unsigned int mSelectColor; // 選択時の色
    unsigned int mDefaultColor; // デフォルトの色
    bool mIsFlash; // 点滅の有無
    float flashTime = 0.2f; // 点滅間隔
    float currentTime = 0.0f; // 現在の時間
};


