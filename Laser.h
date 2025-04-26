#pragma once
#include "Actor.h"
#include <unordered_map>

class CharacterBase;

// ���[�U�[�N���X
class Laser : public Actor {
public:
    // �R���X�g���N�^
    Laser();

    // �f�X�g���N�^
    ~Laser();

    // �������֐�
    void Init(CharacterType ct, std::shared_ptr<CharacterBase> owner, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible);

    // ���Z�b�g�֐�
    void Reset(std::shared_ptr<CharacterBase> owner, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible);

    // �������ς݂��m�F����֐�
    bool IsInitialize() override;

    // �X�V�֐�
    void Update(const float deltaTime) override;

    // �`��֐�
    void Draw(const float deltaTime) override;

    // �X�e�[�^�X�ݒ�֐�
    void SetStatus(float width, float speed, float attack, float knockBackStrength, float knockBackTime);

    // �q�b�g�������m�F����֐�
    bool IsHit();

private:
    void AddLaserKnockBack(std::shared_ptr<CharacterBase>target,float knockBackStrength,float knockBackTime);

    bool mIsHit = false; // �q�b�g�t���O
    bool mIsInitialize = false; // �������t���O

    int mCurrent_startHandle = -1; // ���݂̊J�n�n���h��
    int mCurrent_laserHandle = -1; // ���݂̃��[�U�[�n���h��

    const std::string mAnimStartName = "LaserStart"; // �Ə��t�߂̃A�j���[�V������
    const std::string mAnimLaserName = "Laser"; // ���[�U�[�A�j���[�V������

    float mCurrentTime = 0.0f; // ���݂̎���
    float mInvincibilityTime = 0.2f; // ��e���̖��G����(0.2�b���ƂɃ_���[�W)

    float mSpeed = 0.0f; // ���x
    float mAttack = 0.0f; // �U����
    float mKnockBackStrength = 0.0f; // �m�b�N�o�b�N���x
    float mknockBackTime = 0.0f; // �m�b�N�o�b�N����

    std::weak_ptr<CharacterBase> mOwner; //���˂�����

    Collision::RotatedRect mRotRect; // ��]��`
    float mLaserWidth = 0.0f; // ���[�U�[�̕�
    float mLaserLength = 100.0f; // ���[�U�[�̒���

    Vector2D<float> mLaserStartPosition; // ���[�U�[�̊J�n�ʒu
};


