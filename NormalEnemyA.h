#pragma once
#include "NormalEnemyBase.h"

// 通常敵Aのクラス
class NormalEnemyA : public NormalEnemyBase {
public:
    NormalEnemyA();
    ~NormalEnemyA();

    // 更新関数
    void Update(const float deltaTime) override;
};


