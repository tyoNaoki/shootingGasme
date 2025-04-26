#include "NormalEnemyAttackComponent.h"
#include "ActorManager.h"
#include "DebugHelper.h"
#include "NormalEnemyA.h"
#include "StatusManager.h"
#include "PlayerCharacter.h"
#include "CollisionManager.h"

NormalEnemyAttackComponent::NormalEnemyAttackComponent(std::shared_ptr<CharacterBase> owner) :WeaponComponent(owner)
{
	//初期化
	Init();
	mComponentName = "NormalEnemyAttackComponent";
}

void NormalEnemyAttackComponent::Init()
{
	//ステータス設定
	SetStatus(STATUS.GetCurrentWeaponStatus("NormalEnemyAttackComponent"));
}

void NormalEnemyAttackComponent::Update(const float deltaTime)
{
	auto owner = GetWeaponOwner();
	auto player = Singleton<ActorManager>::get_instance().GetCurrentPlayer();
	if(owner && player && owner->IsActive()){
		//攻撃衝突判定
		bool isHit = COLLISION_M.IsCollidingCharacter(owner,player);
		bool canAttack = currentTime >= mAttackTime;
		//命中
		if(isHit && canAttack){
			std::string message = "Enemy Attack " + std::to_string(mAttack) + " damage";
			Singleton<DebugHelper>::get_instance().Add(message,1.0f);
			//ダメージ処理
			player->TakeDamage(mAttack);
			currentTime = 0.0f;
		}
		//クールタイム設定
		if(!canAttack){
			currentTime += deltaTime;
		}
		
	}
}

void NormalEnemyAttackComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//ステータス設定
	mAttack = status->mAttack;
}
