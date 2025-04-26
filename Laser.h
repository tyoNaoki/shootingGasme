#pragma once
#include "Actor.h"
#include <unordered_map>

class CharacterBase;

// レーザークラス
class Laser : public Actor {
public:
    // コンストラクタ
    Laser();

    // デストラクタ
    ~Laser();

    // 初期化関数
    void Init(CharacterType ct, std::shared_ptr<CharacterBase> owner, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible);

    // リセット関数
    void Reset(std::shared_ptr<CharacterBase> owner, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible);

    // 初期化済みか確認する関数
    bool IsInitialize() override;

    // 更新関数
    void Update(const float deltaTime) override;

    // 描画関数
    void Draw(const float deltaTime) override;

    // ステータス設定関数
    void SetStatus(float width, float speed, float attack, float knockBackStrength, float knockBackTime);

    // ヒットしたか確認する関数
    bool IsHit();

private:
    void AddLaserKnockBack(std::shared_ptr<CharacterBase>target,float knockBackStrength,float knockBackTime);

    bool mIsHit = false; // ヒットフラグ
    bool mIsInitialize = false; // 初期化フラグ

    int mCurrent_startHandle = -1; // 現在の開始ハンドル
    int mCurrent_laserHandle = -1; // 現在のレーザーハンドル

    const std::string mAnimStartName = "LaserStart"; // 照準付近のアニメーション名
    const std::string mAnimLaserName = "Laser"; // レーザーアニメーション名

    float mCurrentTime = 0.0f; // 現在の時間
    float mInvincibilityTime = 0.2f; // 被弾時の無敵時間(0.2秒ごとにダメージ)

    float mSpeed = 0.0f; // 速度
    float mAttack = 0.0f; // 攻撃力
    float mKnockBackStrength = 0.0f; // ノックバック強度
    float mknockBackTime = 0.0f; // ノックバック時間

    std::weak_ptr<CharacterBase> mOwner; //発射した主

    Collision::RotatedRect mRotRect; // 回転矩形
    float mLaserWidth = 0.0f; // レーザーの幅
    float mLaserLength = 100.0f; // レーザーの長さ

    Vector2D<float> mLaserStartPosition; // レーザーの開始位置
};


