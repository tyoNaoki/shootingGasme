#pragma once
#include "NormalEnemyBase.h"

// 通常敵Aのクラス
class NormalEnemyA : public NormalEnemyBase {
public:
    // デフォルトコンストラクタを削除
    NormalEnemyA() = delete;
    // 矩形でコリジョンを初期化,終了処理
    NormalEnemyA(Collision::Rect rect);
    ~NormalEnemyA();

    // 更新関数
    void Update(const float deltaTime) override;
};


