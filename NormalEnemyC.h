#pragma once
#include "NormalEnemyBase.h"

class NormalEnemyC :
    public NormalEnemyBase
{
public:
    //�f�t�H���g�R���X�g���N�^�폜
    NormalEnemyC();
    ~NormalEnemyC();
  //��b�f�[�^�̏�����
    void Init(CharacterType ct, std::string name, int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    //�X�V
    void Update(const float deltaTime)override;
};

