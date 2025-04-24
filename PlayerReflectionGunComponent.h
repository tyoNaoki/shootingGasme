#pragma once
<<<<<<< HEAD
#include "WeaponComponent.h"

// ���ˏe�������v���C���[�̃R���|�[�l���g�N���X
class PlayerReflectionGunComponent : public WeaponComponent {
=======
#include "Component.h"

// ���ˏe�������v���C���[�̃R���|�[�l���g�N���X
class PlayerReflectionGunComponent : public Component {
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
public:
    // �R���X�g���N�^�F�L�����N�^�[�̏��L�҂�������
    PlayerReflectionGunComponent(std::shared_ptr<CharacterBase> owner);

    // �������֐�
    void Init() override;

    // �X�V�֐�
    void Update(const float deltaTime) override;

    // �X�e�[�^�X�ݒ�֐�
    void SetStatus(std::shared_ptr<WeaponStatus> status) override;

private:
    float mAttack = 0.0f; // �U����
    float mShotRate = 0.2f; // ���ˑ��x
    int mReflectionCount = 0; // ���ˉ�
    float mBulletLifeTime = 0.0f; // �e�̎���
    float mCurrentTime = 0.0f; // ���݂̎���

    const std::string mBulletName = "PlayerReflectionBullet"; // �e�̖��O
    const std::string mFireSound = "MagicBulletSound"; // ���ˉ�

    float mLockOnRange = 400.0f; // ���b�N�I���͈�
    float mBulletSpeed = 1000.0f; // �e�̑��x
};


