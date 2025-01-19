#pragma once
#include "UIBase.h"
#include "CharacterBase.h"

// HP�o�[�̊��N���X
class HPBase : public UIBase {
public:
    // �L�����N�^�[�̏��L�҂�������
    HPBase(std::shared_ptr<CharacterBase> owner) : mOwner(owner) {}

    // �X�V�֐�
    void Update(float deltaTime) override;

    // �`��֐�
    void Draw(float deltaTime) override;

    // HP�ݒ�֐�
    void SetHP(float hp);

    // �ő�HP�ݒ�֐�
    void SetMaxHP(float maxHp);

    // �T�C�Y�ݒ�֐�
    void SetSize(int width, int height);

    // ���L�Ҏ擾�֐�
    std::shared_ptr<CharacterBase> GetOwner();

private:
    std::weak_ptr<CharacterBase> mOwner; // �L�����N�^�[�̏��L��

    float mMaxHP = 0; // �ő�HP
    float mHP = 0; // ���݂�HP
    int mWidth = 60; // ��
    int mHeight = 15; // ����
};


