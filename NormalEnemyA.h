#pragma once
#include "NormalEnemyBase.h"

// �ʏ�GA�̃N���X
class NormalEnemyA : public NormalEnemyBase {
public:
    NormalEnemyA();
    ~NormalEnemyA();

    // �X�V�֐�
    void Update(const float deltaTime) override;
};


