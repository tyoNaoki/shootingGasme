#pragma once
#include "WeaponComponent.h"

// ���e�𓊂���v���C���[�̃R���|�[�l���g�N���X
class PlayerThrowBombComponent : public WeaponComponent {
public:
    // �R���X�g���N�^�F�L�����N�^�[�̏��L�҂�������
    PlayerThrowBombComponent(std::shared_ptr<CharacterBase> owner);
    // �������֐�
    void Init() override;

    // �X�V�֐�
    void Update(const float deltaTime) override;

    // �X�e�[�^�X�ݒ�֐�
    void SetStatus(std::shared_ptr<WeaponStatus> status) override;

    //�{���𓊂���ʒu�𗐐��Ōv�Z����֐�
    Vector2D<float> CalcRandomSlowPosition(std::shared_ptr<CharacterBase> owner, float radius, std::shared_ptr<Map> map);

private:
    float mAttack = 0.0f; // �U����
    float mThrowRate = 0.0f; // �������x
    float mExplosionRange = 0.0f; // �����͈�
    float mExplosionTime = 0.0f; // ��������
    float mTimeToExplode = 0.0f; // �����܂ł̎���
    float mKnockBackStrength = 0.0f; // �m�b�N�o�b�N���x
    float mThrowRange = 0.0f; // ��������
    bool mHasContinuousDamage = false; // �����_���[�W�̗L��

    const std::string mBombName = "PlayerBomb"; // ���e�̖��O
    const std::string mSlowSound = "FireBulletSound"; // �X���[�T�E���h

    const Vector2D<float> mDefaultBombSize = Vector2D<float>(6.1, 6.1); // �f�t�H���g�̔��e�T�C�Y

    const float mThrowRangeMin = 200.0f; // �ŏ���������

    float mCurrentTime = 0.0f; // ���݂̎���
    float mDefaultExplosionRange; // �f�t�H���g�̔����͈�
    float mDefaultExplosionTime; // �f�t�H���g�̔�������
    const float defaultPlaySpeed = 1.0f; // �f�t�H���g�̃v���C�X�s�[�h
};


