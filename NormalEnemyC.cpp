#include "NormalEnemyC.h"
#include "NormalEnemyMeleeComponent.h"

<<<<<<< HEAD
NormalEnemyC::NormalEnemyC() : NormalEnemyBase()
=======
NormalEnemyC::NormalEnemyC(Collision::Rect rect) : NormalEnemyBase(rect)
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
{
    //描画位置微調整
    mDrawOffset = Vector2D<float>(-220, -240);
    mDrawExtendSize = Vector2D<float>(3, 3);
}

NormalEnemyC::~NormalEnemyC()
{
}

void NormalEnemyC::Init(CharacterType ct, std::string name, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    //初期化
    NormalEnemyBase::Init(ct, name, id, localPosition, worldPosition, rotation, isVisible);

    std::shared_ptr<NormalEnemyBase> self = std::static_pointer_cast<NormalEnemyBase>(shared_from_this());
    //射撃追加
<<<<<<< HEAD
    GetComponentManager().AddComponent<NormalEnemyMeleeComponent>(2,self);
=======
    AddComponent(std::make_shared<NormalEnemyMeleeComponent>(self), 2);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
}

void NormalEnemyC::Update(const float deltaTime)
{
    //ノックバックは無効に設定
    mKnockBack = false;
    //コンポーネント、座標、当たり判定など更新
    NormalEnemyBase::Update(deltaTime);
}
