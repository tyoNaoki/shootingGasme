#pragma once
#include "UIBase.h"

class UILevelUpReward;
class SelectUIRect;
class LevelUpBonus;
class StatusInfoPanel;

// ���x���A�b�v�p�l���N���X
class LevelUpPanel : public UIBase {
public:
    // �R���X�g���N�^�F��V�I���p�l���A�X�e�[�^�X�\���p�l���A�e�L�X�g�Ȃǂ̍쐬����
    LevelUpPanel(const int width, const int height, const int rewardMaxCount);
    // �f�X�g���N�^
    ~LevelUpPanel() {};

    // �X�e�[�^�X���p�l���ݒ�֐�
    void SetStatusInfoPanel();
    // ���x���A�b�v��V�ݒ�֐�
    void SetLevelUpReward(const std::vector<std::shared_ptr<LevelUpBonus>>& rewards);
    // �X�V�֐�
    void Update(float deltaTime) override;
    // �`��֐�
    void Draw(float deltaTime) override;

private:
    // �f�t�H���g�R���X�g���N�^���폜
    LevelUpPanel() = default;
    // ��V�I���֐�
    void SelectedReward() const;
    // UI�v�f�ǉ��֐�
    void AddUIElement(std::shared_ptr<UIBase> ui);

    std::vector<std::shared_ptr<UIBase>> mUIList; // UI�v�f�̃��X�g

    Vector2D<int> mCenter; // ���S�ʒu
    Vector2D<int> mTextPosi; // �e�L�X�g�ʒu
    Vector2D<int> mSelectRectSize; // �I�����ڃT�C�Y
    Vector2D<int> mLevelUpPanelSize; // ���x���A�b�v�p�l���T�C�Y

    std::shared_ptr<StatusInfoPanel> mStatusInfoPanel; // �X�e�[�^�X���p�l��
    std::vector<std::shared_ptr<UILevelUpReward>> mRewards; // ���x���A�b�v��V

    int rewardCount = 0; // ��V��
    int selectIndex = 0; // �I���C���f�b�N�X
    int mFont; // �t�H���g
};


