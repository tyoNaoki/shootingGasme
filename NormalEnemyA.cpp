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
    //描画位置微調整
    mDrawOffset = Vector2D<float>(-100, -110);
    mDrawExtendSize = Vector2D<float>(1.3,1.3);
}

NormalEnemyA::~NormalEnemyA()
{
}

void NormalEnemyA::Update(const float deltaTime)
{
    //コンポーネント、座標、当たり判定など更新
    NormalEnemyBase::Update(deltaTime);

    //プレイヤーまでの距離取得
    auto length = (ACTOR_M.GetCurrentPlayer()->GetWorldPosition2D()-GetWorldPosition2D()).Length_Square();
    //プレイヤーから離れすぎた場合、非アクティブにする
    if (length > 4000000.0f) {
        DEBUG_HELPER.Add("reSpawn",1.0f);
        SetActive(false);
    }
}