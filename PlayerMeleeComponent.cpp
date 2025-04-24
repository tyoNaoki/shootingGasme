#include "PlayerMeleeComponent.h"
#include "PlayerCharacter.h"
#include "ActorManager.h"
#include "DebugHelper.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include <vector>
#include "GraphicManager.h"
#include "StatusManager.h"
#include "CharacterBase.h"
#include "WeaponStatus.h"

<<<<<<< HEAD
PlayerMeleeComponent::PlayerMeleeComponent(std::shared_ptr<CharacterBase> owner) :WeaponComponent(owner)
=======
PlayerMeleeComponent::PlayerMeleeComponent(std::shared_ptr<CharacterBase> owner) :Component(owner)
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
{
	//初期化
	mComponentName = "PlayerMeleeComponent";
	Init();
}

PlayerMeleeComponent::~PlayerMeleeComponent()
{
}

void PlayerMeleeComponent::Init()
{
	//ステータス設定
	SetStatus(STATUS.GetCurrentWeaponStatus("PlayerMeleeComponent"));
	mIsWidth = false;
}

void PlayerMeleeComponent::Update(const float deltaTime)
{
	auto owner = GetOwner();

	if (!IsEnable() || !owner->IsActive() || owner->IsDead()) { return; }
	//クールタイムが終わっているか
	if(currentTime>0){
		currentTime-=deltaTime;
		return;
	}

	DEBUG_HELPER.Add("Player Attack damage " + std::to_string(attack), meleeSpeed);
	//オーナーの角度取得
	float ownerRot = owner->GetRotation();
	//衝突判定除外にオーナーを追加
	std::vector<std::weak_ptr<CharacterBase>>charas{ owner };
	//周囲の敵取得
	auto nearEnemies = COLLISION_M.GetNearCharacters(owner->GetWorldPosition2D(), meleeRange, charas);
	//角度から近接範囲を計算
	float meleeAngle = ownerRot;
	Collision::RotatedRect attackRange = Collision::CalculateAttackRange(owner->GetWorldPosition2D(), meleeWidthRange, meleeHeightRange, meleeAngle);

	//周囲に敵がいる場合
	if (nearEnemies.size() != 0 || ACTOR_M.IsSpawnBoss()) {

		//音を鳴らす
		SOUND_M.PlaySoundByID(meleeSound);

		//衝突判定計算
		bool isHit = false;
		for (auto& x : nearEnemies) {
			//命中
<<<<<<< HEAD
			if (Collision::IsColliding(attackRange,*x->GetCollision<Collision::Rect>())) {
=======
			if (Collision::IsColliding(attackRange, x->GetCollision())) {
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
				isHit = true;
				//敵までの角度をノックバックとして取得
				float targetRadian = Vector2D<float>::GetLookAtRadian(owner->GetWorldPosition2D(), x->GetWorldPosition2D());
				//ダメージ処理
				x->TakeDamage(attack,Vector2D<float>(cos(targetRadian),sin(targetRadian)),meleeShock,0.2f);
			}
		}
		//ボスとの衝突判定
<<<<<<< HEAD
		if(ACTOR_M.IsSpawnBoss()&&Collision::IsColliding(attackRange,*ACTOR_M.GetCurrentBossEnemy()->GetCollision<Collision::Rect>())){
=======
		if(ACTOR_M.IsSpawnBoss()&&Collision::IsColliding(attackRange,ACTOR_M.GetCurrentBossEnemy()->GetCollision())){
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
			//ダメージ処理
			//ボスはノックバック無効
			ACTOR_M.GetCurrentBossEnemy()->TakeDamage(attack);
		}
		//近接エフェクト描画
		DEBUG_HELPER.DrawCollision(attackRange, 0.2f, isHit);
		DrawMelee(owner,attackRange);

		nearEnemies.clear();
		//クールタイム設定
		currentTime = meleeSpeed;
		return;
	}else{ //周囲に敵がいない場合、衝突検知無し
		//音を鳴らす
		SOUND_M.PlaySoundByID(meleeSound);
		//エフェクト描画
		DEBUG_HELPER.DrawCollision(attackRange,0.2f,false);
		DrawMelee(owner,attackRange);
		//クールタイム設定
		currentTime = meleeSpeed;
		return;
	}
}

void PlayerMeleeComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//ステータス設定
	auto meleeStatus = std::dynamic_pointer_cast<MeleeWeaponStatus>(status);
	attack = meleeStatus->mAttack;
	meleeSpeed = meleeStatus->mSwingSpeed;
	if(meleeWidthRange+1 < meleeStatus->mWidthRange){
		mIsWidth = true;
	}
	meleeWidthRange = meleeStatus->mWidthRange;
	meleeHeightRange = meleeStatus->mHeightRange;
}

void PlayerMeleeComponent::DrawMelee(std::shared_ptr<CharacterBase>owner,Collision::RotatedRect& rotRect)
{
	//位置微調整
	float angle = rotRect.angle;
	auto vertices = rotRect.GetVertices();
	auto offsetYvec = (vertices[0] - vertices[3])/meleeHeightRange;//y軸
	auto offsetXvec = (vertices[0] - vertices[1])/meleeWidthRange;//x軸
	auto start = (vertices[0] - vertices[3]) / 2 + vertices[0];
	
	std::string text = owner->IsReverse() ? "true" : "false";
	DEBUG_HELPER.Add("player is reverse " + text, 3.0f);

	//反転フラグ
	bool reverseYFlag = owner->IsReverse() ? false : true;
	//widthMeleeVer
	//描画位置修正
	if(mIsWidth){
		if (reverseYFlag) {
			start += offsetYvec * -200.0f;
			start += offsetXvec * 100;
		}
		else {
			start += offsetYvec * -200.0f;
			start += offsetXvec * 100;
		}
	}else{
		if (reverseYFlag) {
			start += offsetYvec * -190.0f;
			start += offsetXvec * 30;
		}
		else {
			start += offsetYvec * -190.0f;
			start += offsetXvec * 30;
		}
	}

	auto center = Vector2D<int>(100 / 2, 55 / 2);
	//描画
	if(mIsWidth){//近接がワイドに強化されている時
		EFFECT_M.AddEffectRota3(mActionName,start,center,Vector2D<float>(1.0f,1.5f),angle,true,false,reverseYFlag,drawTime);
	}else{//通常近接
		EFFECT_M.AddEffectRota3(mActionName, start, center, Vector2D<float>(1.0f, 0.3f), angle, true, false, reverseYFlag, drawTime);
	}
}
