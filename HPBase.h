#pragma once
#include "UIBase.h"
#include "CharacterBase.h"

// HPバーの基底クラス
class HPBase : public UIBase {
public:
    // キャラクターの所有者を初期化
    HPBase(std::shared_ptr<CharacterBase> owner) : mOwner(owner) {}

    // 更新関数
    void Update(float deltaTime) override;

    // 描画関数
    void Draw(float deltaTime) override;

    // HP設定関数
    void SetHP(float hp);

    // 最大HP設定関数
    void SetMaxHP(float maxHp);

    // サイズ設定関数
    void SetSize(int width, int height);

    // 所有者取得関数
    std::shared_ptr<CharacterBase> GetOwner();

private:
    std::weak_ptr<CharacterBase> mOwner; // キャラクターの所有者

    float mMaxHP = 0; // 最大HP
    float mHP = 0; // 現在のHP
    int mWidth = 60; // 幅
    int mHeight = 15; // 高さ
};


