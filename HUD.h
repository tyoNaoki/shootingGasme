#pragma once
#include "UIBase.h"
#include <vector>
#include <unordered_map>

class BossBase;

// HUD�N���X
class HUD : public UIBase {
public:
    //�Q�[�����ԁAHP�o�[�A�o���l�o�[�쐬����
    HUD(const int width, const int height);
    // �f�X�g���N�^
    ~HUD();

    // �X�V�֐�
    virtual void Update(float deltaTime) override;
    // �`��֐�
    virtual void Draw(float deltaTime) override;
    // �{�XHUD�����֐�
    void SpawnBossHUD();

private:
    // �f�t�H���g�R���X�g���N�^���폜
    HUD() = default;

    bool IsSpawnBossHUD = false;

    // UI�v�f�擾�֐�
    std::shared_ptr<UIBase> GetElement(std::string name);
    // UI�v�f�ǉ��֐�
    void AddUIElement(std::string name, std::shared_ptr<UIBase> ui);

    // UI�v�f�폜�֐�
    void RemoveUIElement(std::string name);

    std::unordered_map<std::string, std::shared_ptr<UIBase>> mUIList; // UI�v�f�̃��X�g
};




