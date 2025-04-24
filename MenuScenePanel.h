#pragma once
#include "UIBase.h"

class SelectUIRect;
class UIControllDescription;
class UICredit;

// ���j���[�V�[���̃p�l���N���X
class MenuScenePanel : public UIBase {
public:
    // �R���X�g���N�^�F���ƍ�����������
    MenuScenePanel(int width, int height);
    // �f�X�g���N�^
    ~MenuScenePanel();

    // �X�V�֐�
    virtual void Update(float deltaTime) override;
    // �`��֐�
    virtual void Draw(float deltaTime) override;

private:
    // �f�t�H���g�R���X�g���N�^���폜
    MenuScenePanel() = default;

    // UI�v�f�ǉ��֐�
    void AddUIElement(std::shared_ptr<UIBase> ui);
    void AddUIElement(std::shared_ptr<SelectUIRect> ui);
    void AddRect(UI::Rect rect);

    std::vector<std::shared_ptr<UIBase>> mUIList; // UI�v�f�̃��X�g

    //���������\������֐�
    void ViewControllDescription(bool IsOpen);
    // �N���W�b�g��\������֐�
    void ViewCredit(bool IsOpen);

    std::vector<UI::Rect> rects; // ���ڂ̘g�g�݃��X�g
    std::vector<std::shared_ptr<SelectUIRect>> mSelectRect; // �I���\�ȍ��ڂ̃��X�g

    std::shared_ptr<UIControllDescription> mControllDescription; // �������
    std::shared_ptr<UICredit> mCredit; // �N���W�b�g

    int mSelectIndex = 0; // �I���C���f�b�N�X
    int mFont; // �t�H���g
};


