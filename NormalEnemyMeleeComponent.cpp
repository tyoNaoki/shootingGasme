#include "NormalEnemyMeleeComponent.h"
#include "DebugHelper.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "StatusManager.h"
#include "CharacterBase.h"
#include "CollisionManager.h"
#include "GraphicManager.h"
#include "ActorManager.h"
#include "Collision.h"
#include "PlayerCharacter.h"
#include "WeaponStatus.h"

<<<<<<< HEAD
NormalEnemyMeleeComponent::NormalEnemyMeleeComponent(std::shared_ptr<CharacterBase> owner):WeaponComponent(owner)
=======
NormalEnemyMeleeComponent::NormalEnemyMeleeComponent(std::shared_ptr<CharacterBase> owner):Component(owner)
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
{
	//初期化
	mComponentName = "NormalEnemyMeleeComponent";
	Init();
	Vector2D<float>size;
}

void NormalEnemyMeleeComponent::Init()
{
	//ステータス設定
	SetStatus(STATUS.GetCurrentWeaponStatus("NormalEnemyMeleeComponent"));
	currentTime = meleeSpeed;
}

void NormalEnemyMeleeComponent::Update(const float deltaTime)
{
	auto owner = GetOwner();
	auto player = ACTOR_M.GetCurrentPlayer();
	if (!owner->IsActive()||!player||!player->IsActive()) { return; }

	//クールタイムが終わっているか
	if (currentTime > 0) {
		currentTime -= deltaTime;
		return;
	}

	//オーナーの角度から近接範囲を取得
	float meleeAngle = owner->GetRotation();;
	Collision::RotatedRect attackRange = Collision::CalculateAttackRange(owner->GetWorldPosition2D(), meleeWidthRange, meleeHeightRange, meleeAngle);

	//音を鳴らす
	SOUND_M.PlaySoundByID("testSound");
	bool isHit = false;

	//近接攻撃の衝突判定
<<<<<<< HEAD
	auto collision = player->GetCollision<Collision::Rect>();
	if(Collision::IsColliding(attackRange,*collision)){
=======
	if(Collision::IsColliding(attackRange,player->GetCollision())){
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
		isHit =true;
		//プレイヤーまでの角度をノックバック角度に設定
		float targetRadian = Vector2D<float>::GetLookAtRadian(owner->GetWorldPosition2D(), player->GetWorldPosition2D());
		//ダメージ処理
		player->TakeDamage(attack, Vector2D<float>(cos(targetRadian), sin(targetRadian)), meleeShock, 0.2f);
	}
	//デバッグコリジョン表示
	DEBUG_HELPER.DrawCollision(attackRange, 0.2f, isHit);
	//近接エフェクト描画
	DrawMelee(owner, attackRange);
	//クールタイム設定
	currentTime = meleeSpeed;
	return;
}

void NormalEnemyMeleeComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//ステータス設定
	auto meleeStatus = std::dynamic_pointer_cast<MeleeWeaponStatus>(status);
	attack = meleeStatus->mAttack;
	meleeSpeed = meleeStatus->mSwingSpeed;
	meleeWidthRange = meleeStatus->mWidthRange;
	meleeHeightRange = meleeStatus->mHeightRange;
}

void NormalEnemyMeleeComponent::DrawMelee(std::shared_ptr<CharacterBase> owner, Collision::RotatedRect& rotRect)
{
	//描画の位置微調整
	float angle = rotRect.angle;
	auto vertices = rotRect.GetVertices();
	auto offsetYvec = (vertices[0] - vertices[3]) / meleeHeightRange;//y軸
	auto offsetXvec = (vertices[0] - vertices[1]) / meleeWidthRange;//x軸
	auto start = (vertices[0] - vertices[3]) / 2 + vertices[0];

	//画像を反転させるか
	bool reverseYFlag = owner->IsReverse() ? false : true;
	//反転時の位置微調整
	if (reverseYFlag) {
		start += offsetYvec * -190.0f;
		start += offsetXvec * -100;
	}else {
		start += offsetYvec * -190.0f;
		start += offsetXvec * -100;
	}

	
	//近接エフェクト描画
	for(int i = 0;i<3;i++){
		if (reverseYFlag) {
			start += offsetYvec * 0.0f;
			start += offsetXvec * (-5 + i*22);
		}
		else {
			start += offsetYvec * 0.0f;
			start += offsetXvec * (-5+i*22);
		}

		auto center = Vector2D<int>(100 / 2, 55 / 2);
		EFFECT_M.AddEffectRota3(mActionName, start, center, Vector2D<float>(2.5f, 2.5f), angle, true, false, reverseYFlag, drawTime);
	}
}
