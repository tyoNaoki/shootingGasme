#pragma once
#include "CharacterBase.h"

//�G���G�̊�{�N���X
class NormalEnemyBase : public CharacterBase {
public:
<<<<<<< HEAD
    
    NormalEnemyBase();
=======
    // �f�t�H���g�R���X�g���N�^���폜
    NormalEnemyBase() = delete;

    // �R���W��������`�ŏ�����
    NormalEnemyBase(Collision::Rect rect);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
    // �f�X�g���N�^
    ~NormalEnemyBase();

    // �������֐�
    virtual void Init(CharacterType ct, std::string name, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    // ���Z�b�g�֐�
    void Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    // �X�V�֐�
    virtual void Update(const float deltaTime) override;
    // �������ς݂��m�F����֐�
    bool IsInitialize() override;
    // �ړ����x�擾�֐�
    float GetMoveSpeed() const override;

    // void TakeDamage(float damage, Vector2D<float> shock) override;

private:
    float mMoveSpeed = 0.0f; // �ړ����x
    bool mInitialize = false; // �������t���O
    std::vector<std::weak_ptr<CharacterBase>> mCharaToIgnores; //�Փ˔���Ŗ�������L�����N�^�[���X�g
};


