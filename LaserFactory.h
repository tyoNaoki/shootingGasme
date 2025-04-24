#pragma once
#include "ActorFactory.h"

// ���[�U�[�����t�@�N�g���N���X
class LaserFactory : public ActorFactory {
public:
    // �I�u�W�F�N�g�����֐�
    std::shared_ptr<Actor> CreateObject(std::string name) override;
    // ID�����֐�
    virtual int CreateID(std::string name) override;

private:
    int nextID = 0; // ����ID
};


