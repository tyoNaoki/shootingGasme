#pragma once
#include "WeaponComponent.h"

//�G���G�̃v���C���[�Փˎ��̃_���[�W��^����R���|�[�l���g�N���X
class NormalEnemyAttackComponent : public WeaponComponent{
public:
    // �R���X�g���N�^�F�L�����N�^�[�̏��L�҂�������
    NormalEnemyAttackComponent(std::shared_ptr<CharacterBase> owner);

    // �������֐�
    void Init() override;

    // �X�V�֐�
    void Update(const float deltaTime) override;

    // �X�e�[�^�X�ݒ�֐�
    void SetStatus(std::shared_ptr<WeaponStatus> status) override;

private:
    // �U����
    float mAttack = 0.0f;

    // �U���̊Ԋu
    const float mAttackTime = 3.0f;

    // ���݂̎���
    float currentTime = 0.0f;
};


