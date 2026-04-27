#include "PlayerMovementComponent.h"
#include "Actor.h"
#include "InputManager.h"
#include "CharacterBase.h"
#include "Vector2D.h"
#include "Map.h"
#include "DxLib.h"
#include <string>
#include "DebugHelper.h"


void PlayerMovementComponent::Update(const float deltaTime)
{
	//DrawFormatString(0, 50, GetColor(255, 0, 0), "PlayerMovementComponent::Update");
	auto owner = GetOwner();

	//移動
	if(owner && owner->IsActive() && !owner->IsKnockBack()){
		auto direction = Vector2D<float>(0,0);
		InputManager &input = Singleton<InputManager>::get_instance();
		//Wキー
		if(input.IsPushingKey(KEY_INPUT_W)){
			direction += Vector2D<float>(0,-1);
		}
		//Sキー
		if(input.IsPushingKey(KEY_INPUT_S)) {
			direction += Vector2D<float>(0, 1);
		}
		//Aキー
		if (input.IsPushingKey(KEY_INPUT_A)) {
			direction += Vector2D<float>(-1, 0);
		}
		//Dキー
		if(input.IsPushingKey(KEY_INPUT_D)) {
			direction += Vector2D<float>(1, 0);
		}

		//何も押されていない時、角度は現在の角度で更新
		if(direction != Vector2D<float>(0, 0)){
			owner->SetRotation(Vector2D<float>::GetDegreeFromOrigin(direction));
		}
		//移動量設定
		float speed = owner->GetMoveSpeed();
		Vector2D<float>newVel = direction* deltaTime* speed;
		Vector2D<float>newPosi = owner->GetWorldPosition2D() + newVel;

		//速度、座標更新
		owner->SetVelocity(owner->GetVelocity() + newVel);
		owner->SetWorldPosition2D(newPosi,direction != Vector2D<float>(0,0));
	}
}
