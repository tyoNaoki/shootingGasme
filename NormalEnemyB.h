#pragma once
#include "NormalEnemyBase.h"

class NormalEnemyB :
    public NormalEnemyBase
{
public:
    //�f�t�H���g�R���X�g���N�^�폜
    NormalEnemyB() = delete;
    //�R���W�����������A�I������
    NormalEnemyB(Collision::Rect rect);
    ~NormalEnemyB();
    //��b�f�[�^�̏�����
    void Init(CharacterType ct, std::string name, int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    //�R���|�[�l���g����W�A�Փˌ��m�̍X�V
    void Update(const float deltaTime)override;
};

