#pragma once
#include "ItemBase.h"

// 回復アイテムクラス
class HealItem : public ItemBase {
public:
    // コンストラクタ
    HealItem();

    // デストラクタ
    ~HealItem() = default;

    // 初期化関数
    void Init(CharacterType ct, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    // リセット関数
    void Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    // 更新関数
    void Update(float deltaTime) override;

    // 回復設定関数
    void SetHeal(const int level, std::string dropOwnerTypeName);

    // 初期化済みか確認する関数
    bool IsInitialize() override;

    // アイテムボーナス取得関数
    void GetItemBonus() const override;

private:
    float mHeal = 0.0f; // 回復量
};


