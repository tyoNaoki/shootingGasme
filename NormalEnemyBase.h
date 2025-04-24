#pragma once
#include "CharacterBase.h"

//雑魚敵の基本クラス
class NormalEnemyBase : public CharacterBase {
public:
<<<<<<< HEAD
    
    NormalEnemyBase();
=======
    // デフォルトコンストラクタを削除
    NormalEnemyBase() = delete;

    // コリジョンを矩形で初期化
    NormalEnemyBase(Collision::Rect rect);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
    // デストラクタ
    ~NormalEnemyBase();

    // 初期化関数
    virtual void Init(CharacterType ct, std::string name, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    // リセット関数
    void Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    // 更新関数
    virtual void Update(const float deltaTime) override;
    // 初期化済みか確認する関数
    bool IsInitialize() override;
    // 移動速度取得関数
    float GetMoveSpeed() const override;

    // void TakeDamage(float damage, Vector2D<float> shock) override;

private:
    float mMoveSpeed = 0.0f; // 移動速度
    bool mInitialize = false; // 初期化フラグ
    std::vector<std::weak_ptr<CharacterBase>> mCharaToIgnores; //衝突判定で無視するキャラクターリスト
};


