#pragma once
<<<<<<< HEAD
#include "WeaponComponent.h"
=======
#include "Component.h"
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
#include <chrono>
#include <thread>
#include <random>

//�G���G�̏e�������R���|�[�l���g�N���X
<<<<<<< HEAD
class NormalEnemyGunComponent : public WeaponComponent {
=======
class NormalEnemyGunComponent : public Component {
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
public:
    // �L�����N�^�[�̏��L�҂�������
    NormalEnemyGunComponent(std::shared_ptr<CharacterBase> owner);

    // �������֐�
    void Init() override;
    // �X�V�֐�
    void Update(const float deltaTime) override;

    // �X�e�[�^�X�ݒ�֐�
    void SetStatus(std::shared_ptr<WeaponStatus> status) override;

private:
    // �e�𔭎˂���֐�
    void FireBullet(std::shared_ptr<Actor> owner, float targetRot);

    float mAttack = 0.0f; // �U����
    float mShotRate = 0.2f; // ���ˑ��x
    float mBurstRate = 1.0f; // �o�[�X�g���x
    int mBurstCount = 0; // ��x�̃o�[�X�g�Ŕ��˂��鐔
    int mMaxBurstCount = 3; // �ő�o�[�X�g��
    float mCurrentTime = 0.0f; // ���݂̎���

    const std::string mBulletName = "EnemyBullet"; // �e�̖��O
    float mLockOnRange = 400.0f; // ���b�N�I���͈�
    float mBulletSpeed = 1000.0f; // �e�̑��x

    // �����_��������ƕ��z
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution{ 0.1f, 2.0f };
};


