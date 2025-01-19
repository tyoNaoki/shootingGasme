#pragma once
#include "UIBase.h"

class UIText;

// �I���\��UI��`�N���X
class SelectUIRect : public UIBase {
public:
    // �R���X�g���N�^�F��`�A�I��F�A�_�ł̗L����������
    SelectUIRect(UI::Rect rect, unsigned int selectColor, bool isFlash)
        : mRect(rect), mDefaultColor(rect.mColor), mSelectColor(selectColor), mIsFlash(isFlash) {}

    // �f�X�g���N�^
    ~SelectUIRect();

    // �X�V�֐�
    void Update(float deltaTime) override;

    // �`��֐�
    void Draw(float deltaTime) override;

    // �e�L�X�g�ݒ�֐�
    void SetText(std::shared_ptr<UIText> text);

    // �I����ԃt���O
    bool mIsSelect = false;

private:
    UI::Rect mRect; // ��`�f�[�^
    std::shared_ptr<UIText> mText; // �e�L�X�g�I�u�W�F�N�g
    unsigned int mSelectColor; // �I�����̐F
    unsigned int mDefaultColor; // �f�t�H���g�̐F
    bool mIsFlash; // �_�ł̗L��
    float flashTime = 0.2f; // �_�ŊԊu
    float currentTime = 0.0f; // ���݂̎���
};


