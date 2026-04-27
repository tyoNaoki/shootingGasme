#pragma once
#include "ItemBase.h"

// ドロップの経験値アイテムクラス
class LevelUpItem : public ItemBase {
public:
    //アニメーション初期化、描画位置、大きさ微調整
    LevelUpItem();

    // デストラクタ
    ~LevelUpItem() = default;

    // 初期化関数
    void Init(CharacterType ct, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    // リセット関数
    void Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    // 更新関数
    void Update(float deltaTime) override;

    // 経験値設定関数
    void SetExp(const int level, std::string dropOwnerTypeName);

    // 初期化済みか確認する関数
    bool IsInitialize() override;

    // 経験値量を取得
    void GetItemBonus() const override;

private:
    float mExp = 0.0f; // 経験値
};


