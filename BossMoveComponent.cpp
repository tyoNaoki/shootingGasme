#include "BossMoveComponent.h"
#include "BossBase.h"
#include "ActorManager.h"
#include "SceneManager.h"
#include "PlayerCharacter.h"

BossMoveComponent::BossMoveComponent(std::shared_ptr<CharacterBase> owner) :Component(owner)
{
	mComponentName = "BossMoveComponent";
	mBossOwner = std::dynamic_pointer_cast<BossBase>(owner);
	mPlayer =ACTOR_M.GetCurrentPlayer();
	mMap = SCENE_M.GetCurrentScene()->GetMap();
};

void BossMoveComponent::Update(const float deltaTime)
{
	auto owner = mBossOwner.lock();

	if(!owner||!owner->IsActive()||owner->IsDead()){return;}

	if(!mPlayer||!mPlayer->IsActive()||mPlayer->IsDead()){return;}
	//角度設定
	owner->SetRotation(Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), mPlayer->GetWorldPosition2D()));

	//動いていないなら
	if(!mIsMove){
		//目的地をランダムに設定して、移動フラグをたてる
		mNextPosition = GetRandomMovePosition(100);
		mIsMove = true;
	}
	//目的地にたどり着いたか
	if(IsArrived(owner)){
		//移動終了をオーナーに通知
		owner->FinishState();
		mIsMove = false;
		return;
	}

	Move(deltaTime, owner);
}

Vector2D<float> BossMoveComponent::GetRandomMovePosition(const float radius)
{
	int width, height;
	GetWindowSize(&width, &height);
	int maxCount = 10;

	auto centerLocation = mPlayer->GetWorldPosition2D();
    for (int count = 0; count < maxCount; count++) {

        // 0から2πまでのランダムな角度を生成
        float angle = static_cast<float>(static_cast<float>(rand()) / RAND_MAX * 2 * M_PI);

        // 0からradiusまでのランダムな距離を生成 
        float distanceX = static_cast<float>(static_cast<float>(rand()) / RAND_MAX * radius + width * 0.3);
        float distanceY = static_cast<float>(static_cast<float>(rand()) / RAND_MAX * radius + height * 0.3);

        // 極座標からデカルト座標への変換 
        float x = centerLocation.x + static_cast<float>(distanceX * cos(angle));
        float y = centerLocation.y + static_cast<float>(distanceY * sin(angle));

        Vector2D<float>movePosition(x, y);

        if (mMap->GetTileOnWorldLocation(movePosition) == TileType::GROUND) {
            return movePosition;
        }
    }

    return Vector2D<float>();
}

void BossMoveComponent::Move(float deltaTime, std::shared_ptr<BossBase>owner)
{
	//次の座標への方向取得
	float rad = (float)Vector2D<float>::GetLookAtRadian(owner->GetWorldPosition2D(), mNextPosition);

	Vector2D<float>newPosi = owner->GetWorldPosition2D();
	//ラジアンから前方ベクトルを取得
	auto unitVec = Vector2D<float>::GetUnitVectorByRadian(rad);
	
	float speed = 0.0f;
	float dis = (mNextPosition - newPosi).Length();
	if(dis<= speed * deltaTime){
		speed = dis;
	}else{
		speed = owner->GetMoveSpeed();
	}
	
	//移動量を計算
	auto moveVel = unitVec * speed * deltaTime;
	//速度、座標更新
	newPosi = moveVel + owner->GetWorldPosition2D();
	owner->SetVelocity(moveVel);
	owner->SetWorldPosition2D(newPosi, true);
}

bool BossMoveComponent::IsArrived(std::shared_ptr<BossBase>owner)const
{
	//たどり着いているか
	auto posi = owner->GetWorldPosition2D();
	return((mNextPosition - posi).Length() <= mDistanceToArrived);
}
