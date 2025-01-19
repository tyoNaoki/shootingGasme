#pragma once
#include "UIBase.h"
#include <unordered_map>

class SelectUIRect;
class UIControllDescription;
class UISoundSetting;

// ���j���[�p�l���N���X
class MenuPanel : public UIBase {
public:
    //���j���[UI��e�L�X�g�ȂǊe��쐬����
    MenuPanel(int width, int height);
    //UI�̍폜
    ~MenuPanel();

    // �X�V�֐�
    virtual void Update(float deltaTime) override;
    // �`��֐�
    virtual void Draw(float deltaTime) override;

private:
    // �f�t�H���g�R���X�g���N�^���폜
    MenuPanel() = default;

    // UI�v�f�ǉ��֐�
    void AddUIElement(std::shared_ptr<UIBase> ui);
    void AddUIElement(std::shared_ptr<SelectUIRect> ui);
    void AddRect(UI::Rect rect);

    // ���������\������֐�
    void ViewControllDescription(bool IsOpen);
    // ���ʐݒ��\������֐�
    void ViewSoundSetting(bool IsOpen);

    std::vector<std::shared_ptr<UIBase>> mUIList; // UI�v�f�̃��X�g
    std::vector<UI::Rect> rects; // ��`�̃��X�g
    std::vector<std::shared_ptr<SelectUIRect>> mSelectRect; // �I���\�ȋ�`�̃��X�g

    std::shared_ptr<UIControllDescription> mControllDescription; // �R���g���[�������I�u�W�F�N�g
    std::shared_ptr<UISoundSetting> mSoundSetting; // �T�E���h�ݒ�I�u�W�F�N�g

    int mSelectIndex = 0; // �I���C���f�b�N�X
    int mFont; // �t�H���g
};


