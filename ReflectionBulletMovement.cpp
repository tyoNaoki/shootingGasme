#include "ReflectionBulletMovement.h"

void ReflectionBulletMovement::Init()
{
	
}

void ReflectionBulletMovement::Update(const float deltaTime)
{
    auto owner = mBulletOwner.lock();

    if(!owner||!owner->IsActive()) return;

    //生存時間が０秒以下
    if (mLifeTime <= 0.0f) {
        owner->StartDeadAnimation();
        return;
    }

    //角度から進行ベクトルを計算
    double rad = Vector2D<float>::GetRadiansFromDegrees(owner->GetRotation());
    auto unitVec = Vector2D<float>::GetUnitVectorByRadian(rad);
    //移動量を計算
    float speed = owner->GetMoveSpeed();
    auto moveVel = unitVec * speed * deltaTime;
    //座標計算
    auto newPosi = moveVel + owner->GetWorldPosition2D();

    int width, height;
    GetWindowSize(&width, &height);

    auto localPosi = owner->GetLocalFromWorldPosi(newPosi);
    bool refrection = false;
    
    auto collision = owner->GetCollision<Collision::Circle>();
    //プレイヤーの画面外の場合、移動ベクトルを反転させる
    if (localPosi.x < collision->mRadius || localPosi.x > width - collision->mRadius) {
        moveVel.x *= -1;
        refrection = true;
    }
    if (localPosi.y < collision->mRadius || localPosi.y > height - collision->mRadius) {
        moveVel.y *= -1;
        refrection = true;
    }

    //反射フラグがたっている場合
    if (refrection) {
        //反射回数が最大に達していた場合、消去処理
        if (mReflectionMaxCount < mReflectionCurrentCount + 1) {
            std::static_pointer_cast<Bullet>(owner)->StartDeadAnimation();
            return;
        }
        //現在の反射回数を記録
        mReflectionCurrentCount++;
        //角度、座標再設定
        owner->SetRotation(Vector2D<float>::GetLookAtAngle(moveVel));
        newPosi = moveVel + owner->GetWorldPosition2D();
    }

    owner->SetVelocity(moveVel);
    localPosi = SCENE_M.GetCurrentScene()->GetMap()->GetLocalFromWorldPosition(newPosi);
    //座標更新
    owner->SetLocalPosition2D(localPosi);
    owner->SetWorldPosition2D(newPosi);

    mLifeTime -= deltaTime;
}
