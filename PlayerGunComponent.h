#pragma once
#include "WeaponComponent.h"

// �v���C���[�̏e�������R���|�[�l���g�N���X
class PlayerGunComponent : public WeaponComponent {
public:
    // �R���X�g���N�^�F�L�����N�^�[�̏��L�҂�������
    PlayerGunComponent(std::shared_ptr<CharacterBase> owner);

    // �������֐�
    void Init() override;

    // �X�V�֐�
    void Update(const float deltaTime) override;

    // �X�e�[�^�X�ݒ�֐�
    void SetStatus(std::shared_ptr<WeaponStatus> status) override;

private:
    float mAttack = 0.0f; // �U����
    float mShotRate = 0.2f; // ���ˑ��x
    bool mIsPenetration = false; // �ђʂ̗L��
    int mPenetrationMaxCount = 0; // �ő�ђʉ�
    float mCurrentTime = 0.0f; // ���݂̎���

    const std::string mBulletName = "PlayerBullet"; // �e�̖��O
    const std::string mFireSound = "FireBulletSound"; // ���ˉ�

    float mLockOnRange = 400.0f; // ���b�N�I���͈�
    float mBulletSpeed = 1000.0f; // �e�̑��x
};


