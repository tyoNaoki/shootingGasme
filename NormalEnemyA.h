#pragma once
#include "NormalEnemyBase.h"

// �ʏ�GA�̃N���X
class NormalEnemyA : public NormalEnemyBase {
public:
<<<<<<< HEAD
    NormalEnemyA();
=======
    // �f�t�H���g�R���X�g���N�^���폜
    NormalEnemyA() = delete;
    // ��`�ŃR���W������������,�I������
    NormalEnemyA(Collision::Rect rect);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
    ~NormalEnemyA();

    // �X�V�֐�
    void Update(const float deltaTime) override;
};


