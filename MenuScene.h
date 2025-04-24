#pragma once
#include "Scene.h"

// ���j���[�V�[���̃N���X
class MenuScene : public Scene {
public:
    // �R���X�g���N�^
    MenuScene();

    // �f�X�g���N�^
    ~MenuScene();

    // �������֐�
    bool Initialize() override;

    // �X�V�֐�
    void Update(const float deltaTime) override;

    // �`��֐�
    void Render(const float deltaTime) override;

    // �}�b�v�擾�֐�
    std::shared_ptr<Map> GetMap() const override;

    // �I�������֐�
    void End() override;

private:
    int mSelectIndex = 0; // �I���C���f�b�N�X
    bool mInitialze = false; // �������t���O
    int mHandle = -1; // �n���h��
};


