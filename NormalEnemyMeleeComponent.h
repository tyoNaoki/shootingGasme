#pragma once
<<<<<<< HEAD
#include "WeaponComponent.h"
#include <functional>

// �ʏ�G�̋ߐڍU���𐧌䂷��R���|�[�l���g�N���X
class NormalEnemyMeleeComponent : public WeaponComponent {
=======
#include "Component.h"
#include <functional>

// �ʏ�G�̋ߐڍU���𐧌䂷��R���|�[�l���g�N���X
class NormalEnemyMeleeComponent : public Component {
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
public:
    //�L�����N�^�[�̏��L�҂�������
    NormalEnemyMeleeComponent(std::shared_ptr<CharacterBase> owner);

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

    const std::string mActionName = "Spore"; // �A�N�V������
};


