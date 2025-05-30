#include "NormalEnemyB.h"
#include "NormalEnemyGunComponent.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"

NormalEnemyB::NormalEnemyB() : NormalEnemyBase()
{
    //初期アニメーション設定
    mStartAnimStateName = "Flight";
    //描画位置、大きさ微調整
    mDrawOffset = Vector2D<float>(-90, -100);
    mDrawExtendSize = Vector2D<float>(1.3, 1.3);
}

NormalEnemyB::~NormalEnemyB()
{
}

void NormalEnemyB::Init(CharacterType ct, std::string name, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    //初期化
    NormalEnemyBase::Init(ct,name,id,localPosition,worldPosition,rotation,isVisible);

    std::shared_ptr<NormalEnemyBase> self = std::static_pointer_cast<NormalEnemyBase>(shared_from_this());
    //射撃追加
    GetComponentManager().AddComponent<NormalEnemyGunComponent>(2,self);
}

void NormalEnemyB::Update(const float deltaTime)
{
    //コンポーネント、座標、当たり判定など更新
    NormalEnemyBase::Update(deltaTime);

    //プレイヤーまでの距離取得
    auto length = (ACTOR_M.GetCurrentPlayer()->GetWorldPosition2D() - GetWorldPosition2D()).Length_Square();
    //プレイヤーから離れすぎた場合、非アクティブにする
    if (length > 4000000.0f) {
        DEBUG_HELPER.Add("reSpawn", 1.0f);
        SetActive(false);
    }
}
