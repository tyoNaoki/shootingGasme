#pragma once
#include "ItemBase.h"

// �h���b�v�̌o���l�A�C�e���N���X
class LevelUpItem : public ItemBase {
public:
    //�A�j���[�V�����������A�`��ʒu�A�傫��������
    LevelUpItem();

    // �f�X�g���N�^
    ~LevelUpItem() = default;

    // �������֐�
    void Init(CharacterType ct, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    // ���Z�b�g�֐�
    void Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    // �X�V�֐�
    void Update(float deltaTime) override;

    // �o���l�ݒ�֐�
    void SetExp(const int level, std::string dropOwnerTypeName);

    // �������ς݂��m�F����֐�
    bool IsInitialize() override;

    // �o���l�ʂ��擾
    void GetItemBonus() const override;

private:
    float mExp = 0.0f; // �o���l
};


