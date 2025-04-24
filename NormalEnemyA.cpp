#include "NormalEnemyA.h"
#include "DebugHelper.h"
#include "PlayerCharacter.h"
#include "ActorManager.h"

<<<<<<< HEAD
NormalEnemyA::NormalEnemyA() : NormalEnemyBase()
=======
NormalEnemyA::NormalEnemyA(Collision::Rect rect) : NormalEnemyBase(rect)
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
{
    //�`��ʒu������
    mDrawOffset = Vector2D<float>(-100, -110);
    mDrawExtendSize = Vector2D<float>(1.3,1.3);
}

NormalEnemyA::~NormalEnemyA()
{
}

void NormalEnemyA::Update(const float deltaTime)
{
    //�R���|�[�l���g�A���W�A�����蔻��ȂǍX�V
    NormalEnemyBase::Update(deltaTime);

    //�v���C���[�܂ł̋����擾
    auto length = (ACTOR_M.GetCurrentPlayer()->GetWorldPosition2D()-GetWorldPosition2D()).Length_Square();
    //�v���C���[���痣�ꂷ�����ꍇ�A��A�N�e�B�u�ɂ���
    if (length > 4000000.0f) {
        DEBUG_HELPER.Add("reSpawn",1.0f);
        SetActive(false);
    }
}