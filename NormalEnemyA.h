#pragma once
#include "NormalEnemyBase.h"

// �ʏ�GA�̃N���X
class NormalEnemyA : public NormalEnemyBase {
public:
    // �f�t�H���g�R���X�g���N�^���폜
    NormalEnemyA() = delete;
    // ��`�ŃR���W������������,�I������
    NormalEnemyA(Collision::Rect rect);
    ~NormalEnemyA();

    // �X�V�֐�
    void Update(const float deltaTime) override;
};


