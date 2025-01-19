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

	//�ړ�
	if(owner && owner->IsActive() && !owner->IsKnockBack()){
		auto direction = Vector2D<float>(0,0);
		InputManager &input = Singleton<InputManager>::get_instance();
		//W�L�[
		if(input.IsPushingKey(KEY_INPUT_W)){
			direction += Vector2D<float>(0,-1);
		}
		//S�L�[
		if(input.IsPushingKey(KEY_INPUT_S)) {
			direction += Vector2D<float>(0, 1);
		}
		//A�L�[
		if (input.IsPushingKey(KEY_INPUT_A)) {
			direction += Vector2D<float>(-1, 0);
		}
		//D�L�[
		if(input.IsPushingKey(KEY_INPUT_D)) {
			direction += Vector2D<float>(1, 0);
		}

		//����������Ă��Ȃ����A�p�x�͌��݂̊p�x�ōX�V
		if(direction != Vector2D<float>(0, 0)){
			owner->SetRotation(Vector2D<float>::GetDegreeFromOrigin(direction));
		}
		//�ړ��ʐݒ�
		float speed = owner->GetMoveSpeed();
		Vector2D<float>newVel = direction* deltaTime* speed;
		Vector2D<float>newPosi = owner->GetWorldPosition2D() + newVel;

		//���x�A���W�X�V
		owner->SetVelocity(owner->GetVelocity() + newVel);
		owner->SetWorldPosition2D(newPosi,direction != Vector2D<float>(0,0));
	}
}
