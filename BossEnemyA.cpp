#include "BossEnemyA.h"
#include "BossMoveComponent.h"
#include "BossTackleComponent.h"
#include "BossLaserComponent.h"
#include "BossGunComponent.h"
#include "GraphicManager.h"
#include <random>

<<<<<<< HEAD
BossEnemyA::BossEnemyA():BossBase()
=======
BossEnemyA::BossEnemyA(Collision::Rect rect):BossBase(rect)
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
{
    mDrawOffset = Vector2D<float>(-240, -240);
    mDrawExtendSize = Vector2D<float>(5,5);
}

void BossEnemyA::Init(CharacterType ct, std::string name, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    BossBase::Init(ct,name,id,localPosition,worldPosition,rotation,isVisible);

    //衝突判定除外用
    std::shared_ptr<CharacterBase> self = std::static_pointer_cast<CharacterBase>(shared_from_this());
    //コンポーネント追加
<<<<<<< HEAD
    GetComponentManager().AddComponent<BossMoveComponent>(0,self);
    GetComponentManager().AddComponent<BossTackleComponent>(1,self);
    GetComponentManager().AddComponent<BossLaserComponent>(2,self);
    GetComponentManager().AddComponent<BossGunComponent>(3, self);
=======
    AddComponent(std::make_shared<BossMoveComponent>(self), 0 );
    AddComponent(std::make_shared<BossTackleComponent>(self), 1);
    AddComponent(std::make_shared<BossLaserComponent>(self),2);
    AddComponent(std::make_shared<BossGunComponent>(self), 3);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
}

void BossEnemyA::Update(float deltaTime)
{
    auto scene = Singleton<SceneManager>::get_instance().GetCurrentScene();
    if (!scene) { return; }

    //速度取得
    Vector2D<float> cur_vel = GetVelocity();

    //動いているかどうか
    bool isMove = IsMoving();

    switch (GetCurrentState())
    {
    case BossState::Move://移動
<<<<<<< HEAD
        GetComponentManager().GetComponent<BossMoveComponent>()->Update(deltaTime);
        //移動時に自分に触れたプレイヤーにタックルダメージを与える
        GetComponentManager().GetComponent<BossTackleComponent>()->Update(deltaTime);
=======
        GetComponent("BossMoveComponent")->Update(deltaTime);
        //移動時に自分に触れたプレイヤーにタックルダメージを与える
        GetComponent("BossTackleComponent")->Update(deltaTime);
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
        break;
    case BossState::Attack://攻撃
        mAttackComponent->Update(deltaTime);
        break;
    case BossState::Wait://停止

        //70%で弾発射
        if(IsChooseLeft(0.7f)){
<<<<<<< HEAD
            mAttackComponent = GetComponentManager().GetComponent<BossGunComponent>();
            
        }else{//30%でレーザー
            mAttackComponent = GetComponentManager().GetComponent<BossLaserComponent>();
=======
            mAttackComponent = GetComponent("BossGunComponent");
            
        }else{//30%でレーザー
            mAttackComponent = GetComponent("BossLaserComponent");
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
        }
        //現在の攻撃の名前取得
        mWeaponName = mAttackComponent->GetName();
        //停止状態の終了処理
        FinishState();
        break;
    case BossState::Dead:
        FinishState();
        break;
    default:
        break;
    }

    //現在の描画角度取得
    float currentRotation = GetDrawRotation();
    DEBUG_HELPER.Add("boss Rotation is "+std::to_string(currentRotation));

    bool isReverse = false;
    if (nearEqual(currentRotation, 90.0f) || nearEqual(currentRotation, 270.0f)) {
        //角度が90度か270度に近い数値なら、向きはそのままの向きで固定
        isReverse = mReverse;
    }
    //左向きにさせる
    else if (currentRotation > 90.0f && currentRotation < 270.0f) {
        isReverse = true;
        //角度が90度か270度に近い数値なら向きはそのままで返す
    }
    else {
        //右向き
        isReverse = false;
    }

    mReverse = isReverse;

    auto newPosi = GetWorldPosition2D();
    auto localPosi = scene->GetMap()->GetLocalFromWorldPosition(newPosi);

    //座標更新
    SetLocalPosition2D(localPosi);
    SetWorldPosition2D(newPosi);

<<<<<<< HEAD
    //DEBUG_HELPER.DrawCollision(GetCollision());
=======
    DEBUG_HELPER.DrawCollision(GetCollision());
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420

    std::string preAction = GetCurrentAnimState()->GetActionName();
    ANIM_M.Update(this);

    if (preAction != GetCurrentAnimState()->GetActionName()) {
        GRAPHIC_M.TransitionToAction(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize, GetTypeName(), GetCurrentAnimState()->GetActionName());
    }else {
        //現在のアニメーション画像更新
        GRAPHIC_M.updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(), GetCurrentAnimState()->GetActionName());
    }
}

void BossEnemyA::FinishState()
{
    switch (GetCurrentState())
    {
    case BossState::Move://移動
        //一旦その場で止まる
        ChangeState(BossState::Wait);
        break;
    case BossState::Attack://攻撃
        //現在の攻撃コンポーネント変数をNULLで初期化
        mAttackComponent = nullptr;
        //移動状態に遷移
        ChangeState(BossState::Move);
        break;
    case BossState::Wait://停止中
        //攻撃が設定されているなら、攻撃状態に遷移
        if(mAttackComponent){
            ChangeState(BossState::Attack);
            break;
        }
        //移動に遷移
        ChangeState(BossState::Move);
        break;
    case BossState::Dead:
        
        break;
    default:
        break;
    }
}

bool BossEnemyA::IsChooseLeft(float leftWeight)
{
    if(0.0>leftWeight||1.0<leftWeight ){
        leftWeight = 0.0f;
    }
    // 0.0から1.0までの範囲で乱数を生成するエンジン
    std::random_device rd; std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f); // 乱数を生成して確率と比較 
    float randomValue = distribution(generator);
    return randomValue < leftWeight;
}
