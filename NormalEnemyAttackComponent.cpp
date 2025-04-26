#include "NormalEnemyAttackComponent.h"
#include "ActorManager.h"
#include "DebugHelper.h"
#include "NormalEnemyA.h"
#include "StatusManager.h"
#include "PlayerCharacter.h"
#include "CollisionManager.h"

NormalEnemyAttackComponent::NormalEnemyAttackComponent(std::shared_ptr<CharacterBase> owner) :WeaponComponent(owner)
{
	//������
	Init();
	mComponentName = "NormalEnemyAttackComponent";
}

void NormalEnemyAttackComponent::Init()
{
	//�X�e�[�^�X�ݒ�
	SetStatus(STATUS.GetCurrentWeaponStatus("NormalEnemyAttackComponent"));
}

void NormalEnemyAttackComponent::Update(const float deltaTime)
{
	auto owner = GetWeaponOwner();
	auto player = Singleton<ActorManager>::get_instance().GetCurrentPlayer();
	if(owner && player && owner->IsActive()){
		//�U���Փ˔���
		bool isHit = COLLISION_M.IsCollidingCharacter(owner,player);
		bool canAttack = currentTime >= mAttackTime;
		//����
		if(isHit && canAttack){
			std::string message = "Enemy Attack " + std::to_string(mAttack) + " damage";
			Singleton<DebugHelper>::get_instance().Add(message,1.0f);
			//�_���[�W����
			player->TakeDamage(mAttack);
			currentTime = 0.0f;
		}
		//�N�[���^�C���ݒ�
		if(!canAttack){
			currentTime += deltaTime;
		}
		
	}
}

void NormalEnemyAttackComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//�X�e�[�^�X�ݒ�
	mAttack = status->mAttack;
}
