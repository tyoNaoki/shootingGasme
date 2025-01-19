#pragma once
#include "ItemBase.h"

// �񕜃A�C�e���N���X
class HealItem : public ItemBase {
public:
    // �R���X�g���N�^
    HealItem();

    // �f�X�g���N�^
    ~HealItem() = default;

    // �������֐�
    void Init(CharacterType ct, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    // ���Z�b�g�֐�
    void Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    // �X�V�֐�
    void Update(float deltaTime) override;

    // �񕜐ݒ�֐�
    void SetHeal(const int level, std::string dropOwnerTypeName);

    // �������ς݂��m�F����֐�
    bool IsInitialize() override;

    // �A�C�e���{�[�i�X�擾�֐�
    void GetItemBonus() const override;

private:
    float mHeal = 0.0f; // �񕜗�
};


