#pragma once
#include "Component.h"
#include "InputManager.h"

class CharacterBase;

// �v���C���[�̈ړ��𐧌䂷��R���|�[�l���g�N���X
class PlayerMovementComponent : public Component {
public:
    // �R���X�g���N�^�F�L�����N�^�[�̏��L�҂�������
    PlayerMovementComponent(std::shared_ptr<CharacterBase> owner) : Component(owner) {
        mComponentName = "PlayerMovementComponent";
    }

    // �X�V�֐�
    void Update(const float deltaTime) override;
};


