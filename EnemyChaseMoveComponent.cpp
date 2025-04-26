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
		//�m�b�N�o�b�N��ԂȂ���W�Ƒ��x�����̂܂܂ōX�V���āA�I��
		if(owner->IsKnockBack()){
			owner->SetVelocity(Vector2D<float>(0,0));
			owner->SetWorldPosition2D(owner->GetWorldPosition2D(), false);
			return;
		}

		//�v���C���[�ւ̕����擾
		float rad = (float)Vector2D<float>::GetLookAtRadian(owner->GetWorldPosition2D(), player->GetWorldPosition2D());
		//�p�x�ݒ�
		owner->SetRotation(Vector2D<float>::GetDegreesFromRadians(rad));

		Vector2D<float>newPosi = owner->GetWorldPosition2D();
		bool isMove = false;
		//���݂̊p�x����i�s�x�N�g���v�Z
		auto unitVec = Vector2D<float>::GetUnitVectorByRadian(rad);
		float speed = owner->GetMoveSpeed();
		//�ړ��ʌv�Z
		auto moveVel =  unitVec * speed * deltaTime;

		newPosi = moveVel + owner->GetWorldPosition2D();
		//���x�X�V
		owner->SetVelocity(moveVel);
		
		//�ړ��t���O�I��
		isMove = true;

		//���W�X�V
		owner->SetWorldPosition2D(newPosi,isMove);
	}
}
