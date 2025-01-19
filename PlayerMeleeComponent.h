#pragma once
#include "Component.h"
#include "CollisionManager.h"
#include "StatusManager.h"
#include "Collision.h"
#include "Vector2D.h"

class PlayerCharacter;

// �v���C���[�̋ߐڍU���𐧌䂷��R���|�[�l���g�N���X
class PlayerMeleeComponent : public Component {
public:
    // �R���X�g���N�^�F�L�����N�^�[�̏��L�҂�������
    PlayerMeleeComponent(std::shared_ptr<CharacterBase> owner);
    // �f�X�g���N�^
    ~PlayerMeleeComponent();

    // �������֐�
    void Init() override;
    // �X�V�֐�
    void Update(const float deltaTime) override;

    // �X�e�[�^�X�ݒ�֐�
    void SetStatus(std::shared_ptr<WeaponStatus> status) override;

    // �ߐڍU���̕`��֐�
    void DrawMelee(std::shared_ptr<CharacterBase> owner, Collision::RotatedRect& rotRect);

private:
    float attack = 0.0f; // �U����
    float meleeSpeed = 0.5f; // �ߐڍU���̑��x
    float currentTime = 0.0f; // ���݂̎���

    const float drawTime = 0.2f; // �`�掞��
    float meleeRange = 200.0f; // �ߐڍU���͈̔�
    float meleeWidthRange = 50.0f; // �ߐڍU���̕��͈�
    float meleeHeightRange = 150.0f; // �ߐڍU���̍����͈�
    float meleeShock = 1000.0f; // �ߐڍU���̏Ռ���

    std::string meleeSound = "swordAttack"; // �ߐڍU���̉�
    const std::string mActionName = "Melee"; // �A�N�V������

    bool mIsWidth = false; // ���͈͂���V�ŋ����ς݂��t���O
};


