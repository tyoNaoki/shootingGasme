#pragma once
#include "NormalEnemyBase.h"

class NormalEnemyC :
    public NormalEnemyBase
{
public:
    //�f�t�H���g�R���X�g���N�^�폜
<<<<<<< HEAD
    NormalEnemyC();
=======
    NormalEnemyC() = delete;
    //�R���W�����������A�I������
    NormalEnemyC(Collision::Rect rect);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
    ~NormalEnemyC();
  //��b�f�[�^�̏�����
    void Init(CharacterType ct, std::string name, int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    //�X�V
    void Update(const float deltaTime)override;
};

