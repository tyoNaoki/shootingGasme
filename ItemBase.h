#pragma once
#include "Actor.h"

class PlayerCharacter;

// アイテム基底クラス
class ItemBase : public Actor {
public:
    // コンストラクタ
    ItemBase();

    // デストラクタ
    ~ItemBase() = default;

    // 初期化関数
    virtual void Init(CharacterType ct, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible);

    // リセット関数
    virtual void Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible);

    // 初期化済みか確認する関数
    virtual bool IsInitialize() override = 0;

    // 更新関数
    virtual void Update(const float deltaTime) override;

    // 描画関数
    void Draw(const float deltaTime) override;

    // 描画オフセット適用関数
    void ApplyDrawOffset(Vector2D<float> offset);

    // 追跡移動関数
    void ChaseMovement(float deltaTime, std::shared_ptr<Actor> target);

    // 移動速度取得関数
    virtual float GetMoveSpeed() const;

    // アイテムボーナス取得関数
    virtual void GetItemBonus() const = 0;

    // 取得済みか確認する関数
    bool IsTaked();

    // 生存確認関数
    bool IsLife();

protected:
    // ライフタイム計算関数
    void calcLifeTime(float deltaTime);

    bool mIsTaked = false; // 取得済みフラグ
    bool mIsInitialize = false; // 初期化フラグ

    float mCurrentLifeTime = 0.0f; // 現在のライフタイム
    float mMaxLifeTime = 10.0f; // 最大ライフタイム

private:
    float mMoveSpeed = 500.0f; // 移動速度
    float mChaseDistance = 100000.0f; // 追跡距離
    bool mIsChase = false; // 追跡フラグ
};


