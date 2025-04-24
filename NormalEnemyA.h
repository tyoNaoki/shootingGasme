#pragma once
#include "NormalEnemyBase.h"

// 通常敵Aのクラス
class NormalEnemyA : public NormalEnemyBase {
public:
<<<<<<< HEAD
    NormalEnemyA();
=======
    // デフォルトコンストラクタを削除
    NormalEnemyA() = delete;
    // 矩形でコリジョンを初期化,終了処理
    NormalEnemyA(Collision::Rect rect);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
    ~NormalEnemyA();

    // 更新関数
    void Update(const float deltaTime) override;
};


