#pragma once
#include "Actor.h"

class PlayerCharacter;

// �A�C�e�����N���X
class ItemBase : public Actor {
public:
    // �R���X�g���N�^
    ItemBase();

    // �f�X�g���N�^
    ~ItemBase() = default;

    // �������֐�
    virtual void Init(CharacterType ct, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible);

    // ���Z�b�g�֐�
    virtual void Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible);

    // �������ς݂��m�F����֐�
    virtual bool IsInitialize() override = 0;

    // �X�V�֐�
    virtual void Update(const float deltaTime) override;

    // �`��֐�
    void Draw(const float deltaTime) override;

    // �`��I�t�Z�b�g�K�p�֐�
    void ApplyDrawOffset(Vector2D<float> offset);

    // �ǐՈړ��֐�
    void ChaseMovement(float deltaTime, std::shared_ptr<Actor> target);

    // �ړ����x�擾�֐�
    virtual float GetMoveSpeed() const;

    // �A�C�e���{�[�i�X�擾�֐�
    virtual void GetItemBonus() const = 0;

    // �擾�ς݂��m�F����֐�
    bool IsTaked();

    // �����m�F�֐�
    bool IsLife();

protected:
    // ���C�t�^�C���v�Z�֐�
    void calcLifeTime(float deltaTime);

    bool mIsTaked = false; // �擾�ς݃t���O
    bool mIsInitialize = false; // �������t���O

    float mCurrentLifeTime = 0.0f; // ���݂̃��C�t�^�C��
    float mMaxLifeTime = 10.0f; // �ő僉�C�t�^�C��

private:
    float mMoveSpeed = 500.0f; // �ړ����x
    float mChaseDistance = 100000.0f; // �ǐՋ���
    bool mIsChase = false; // �ǐՃt���O
};


