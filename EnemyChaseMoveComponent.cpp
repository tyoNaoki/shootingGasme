#include "EnemyChaseMoveComponent.h"
#include "DebugHelper.h"
#include "PlayerCharacter.h"
#include "ActorManager.h"

void EnemyChaseMoveComponent::Update(const float deltaTime)
{
	auto owner = mMoveOwner.lock();
	auto player = Singleton<ActorManager>::get_instance().GetCurrentPlayer();
	auto& dh = Singleton<DebugHelper>::get_instance();

	if(owner && player && owner->IsActive()){
		//ノックバック状態なら座標と速度をそのままで更新して、終了
		if(owner->IsKnockBack()){
			owner->SetVelocity(Vector2D<float>(0,0));
			owner->SetWorldPosition2D(owner->GetWorldPosition2D(), false);
			return;
		}

		//プレイヤーへの方向取得
		float rad = (float)Vector2D<float>::GetLookAtRadian(owner->GetWorldPosition2D(), player->GetWorldPosition2D());
		//角度設定
		owner->SetRotation(Vector2D<float>::GetDegreesFromRadians(rad));

		Vector2D<float>newPosi = owner->GetWorldPosition2D();
		bool isMove = false;
		//現在の角度から進行ベクトル計算
		auto unitVec = Vector2D<float>::GetUnitVectorByRadian(rad);
		float speed = owner->GetMoveSpeed();
		//移動量計算
		auto moveVel =  unitVec * speed * deltaTime;

		newPosi = moveVel + owner->GetWorldPosition2D();
		//速度更新
		owner->SetVelocity(moveVel);
		
		//移動フラグオン
		isMove = true;

		//座標更新
		owner->SetWorldPosition2D(newPosi,isMove);
	}
}
