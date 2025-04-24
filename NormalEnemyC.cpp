#include "NormalEnemyC.h"
#include "NormalEnemyMeleeComponent.h"

NormalEnemyC::NormalEnemyC() : NormalEnemyBase()
{
    //�`��ʒu������
    mDrawOffset = Vector2D<float>(-220, -240);
    mDrawExtendSize = Vector2D<float>(3, 3);
}

NormalEnemyC::~NormalEnemyC()
{
}

void NormalEnemyC::Init(CharacterType ct, std::string name, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    //������
    NormalEnemyBase::Init(ct, name, id, localPosition, worldPosition, rotation, isVisible);

    std::shared_ptr<NormalEnemyBase> self = std::static_pointer_cast<NormalEnemyBase>(shared_from_this());
    //�ˌ��ǉ�
    GetComponentManager().AddComponent<NormalEnemyMeleeComponent>(2,self);
}

void NormalEnemyC::Update(const float deltaTime)
{
    //�m�b�N�o�b�N�͖����ɐݒ�
    mKnockBack = false;
    //�R���|�[�l���g�A���W�A�����蔻��ȂǍX�V
    NormalEnemyBase::Update(deltaTime);
}
