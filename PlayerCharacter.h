#pragma once
#include "CharacterBase.h"

class PlayerCharacter : public CharacterBase {
public:
    // �f�t�H���g�R���X�g���N�^
    PlayerCharacter();

    // �f�t�H���g�f�X�g���N�^
    ~PlayerCharacter() = default;

    // �������֐�
    void Init(CharacterType ct, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;

    // ���Z�b�g�֐�
    void Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;

    // �X�V�֐�
    void Update(const float deltaTime) override;
    // �������ς݂��m�F����֐�
    bool IsInitialize() override;

    // �_���[�W�����֐�
    void TakeDamage(float damage, Vector2D<float> direction = Vector2D<float>(), float strength = 0.0f, float knockBackTime = -1.0f) override;

    // ���x���A�b�v�J�n�֐�
    void StartLevelUp(int levelUpCount);
    // ���x���A�b�v�����֐�
    void LevelUp();
    // ���탌�x���A�b�v�֐�
    void WeaponLevelUp(std::string componentName);

    // �o���l���Z�b�g�֐�
    void ResetExp();
    // �o���l�擾�֐�
    void TakeExp(float exp);
    // �񕜏����֐�
    void TakeHeal(float heal);

    // �ړ����x�擾�֐�
    float GetMoveSpeed() const override;
    // ���݂̌o���l�擾�֐�
    float GetCurrentExp() const;

    // ���������ݒ�֐�
    void AddPlayerComponents(std::shared_ptr<PlayerCharacter> self);

    // �v���C���[�R���|�[�l���g���Z�b�g�֐�
    void ResetPlayerComponents();

    // �����ǉ��֐�(�������g�p�\�ɂ���)
    void EnablePlayerComponent(const std::string componentName);
    
private:
    float mCurrentExp = 0; // ���݂̌o���l
    float mAllExp = 0; // �S�o���l

    float mMoveSpeed = 0.0f; // �ړ����x

    bool mInitialize = false; // �������t���O

    std::vector<int> mStartEnableComponentList; // �����L���R���|�[�l���g���X�g
};


