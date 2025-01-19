#pragma once
#include "ActorFactory.h"
#include "ItemBase.h"
#include <unordered_map>

// �A�C�e�������t�@�N�g���N���X
class ItemFactory : public ActorFactory {
    friend class Singleton<ItemFactory>;

public:
    // �I�u�W�F�N�g�����֐�
    virtual std::shared_ptr<Actor> CreateObject(std::string name) override;
    // ID�����֐�
    virtual int CreateID(std::string name) override;

private:
    //�A�C�e�����Ƃ̎���ID�̂��߂̔z��쐬
    ItemFactory();
    //ID����
    ~ItemFactory();

    std::unordered_map<std::string, int> mItem_Indexs; // �A�C�e���C���f�b�N�X�̃}�b�v
};


