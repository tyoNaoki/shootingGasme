#pragma once
#include "CharacterBase.h"

class PlayerCharacter : public CharacterBase {
public:
    // デフォルトコンストラクタ
    PlayerCharacter();

    // デフォルトデストラクタ
    ~PlayerCharacter() = default;

    // 初期化関数
    void Init(CharacterType ct, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;

    // リセット関数
    void Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;

    // 更新関数
    void Update(const float deltaTime) override;
    // 初期化済みか確認する関数
    bool IsInitialize() override;

    // ダメージ処理関数
    void TakeDamage(float damage, Vector2D<float> direction = Vector2D<float>(), float strength = 0.0f, float knockBackTime = -1.0f) override;

    // レベルアップ開始関数
    void StartLevelUp(int levelUpCount);
    // レベルアップ処理関数
    void LevelUp();
    // 武器レベルアップ関数
    void WeaponLevelUp(std::string componentName);

    // 経験値リセット関数
    void ResetExp();
    // 経験値取得関数
    void TakeExp(float exp);
    // 回復処理関数
    void TakeHeal(float heal);

    // 移動速度取得関数
    float GetMoveSpeed() const override;
    // 現在の経験値取得関数
    float GetCurrentExp() const;

    // 初期装備設定関数
    void AddPlayerComponents(std::shared_ptr<PlayerCharacter> self);

    // プレイヤーコンポーネントリセット関数
    void ResetPlayerComponents();

    // 装備追加関数(装備を使用可能にする)
    void EnablePlayerComponent(const std::string componentName);
    
private:
    float mCurrentExp = 0; // 現在の経験値
    float mAllExp = 0; // 全経験値

    float mMoveSpeed = 0.0f; // 移動速度

    bool mInitialize = false; // 初期化フラグ

    std::vector<int> mStartEnableComponentList; // 初期有効コンポーネントリスト
};


