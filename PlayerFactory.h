#pragma once
#include "CharacterFactory.h"

class PlayerCharacter;

// �v���C���[�L�����N�^�[�𐶐�����t�@�N�g���N���X
class PlayerFactory : public CharacterFactory {
    friend class Singleton<PlayerFactory>;

public:
    // �I�u�W�F�N�g�����֐�
    std::shared_ptr<CharacterBase> CreateObject(std::string name) override;
    // ID�����֐�
    int CreateID(std::string name) override;

private:
    PlayerFactory() = default; // �R���X�g���N�^
    ~PlayerFactory() = default; // �f�X�g���N�^

    int mChara_Index = 0; // �L�����N�^�[�C���f�b�N�X
};


