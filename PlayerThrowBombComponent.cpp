#include "PlayerThrowBombComponent.h"
#include "CollisionManager.h"
#include "StatusManager.h"
#include "ActorManager.h"
#include "SceneManager.h"
#include "WeaponStatus.h"

PlayerThrowBombComponent::PlayerThrowBombComponent(std::shared_ptr<CharacterBase> owner) :WeaponComponent(owner)
{
    //初期化
	mComponentName = "PlayerThrowBombComponent";
	Init();
}

void PlayerThrowBombComponent::Init()
{
    //ステータス設定
	SetStatus(STATUS.GetCurrentWeaponStatus(mComponentName));
}

void PlayerThrowBombComponent::Update(const float deltaTime)
{
	auto owner = GetWeaponOwner();

	if (!IsEnable() || !owner->IsActive() || owner->IsDead()) { return; }

    //クールタイムが終わっているか
	if (mCurrentTime > 0) {
		mCurrentTime -= deltaTime;
		return;
	}
    //現在のマップ取得
    auto map = SCENE_M.GetCurrentScene()->GetMap();
    //投げる位置、角度取得
    auto throwPosi = CalcRandomSlowPosition(owner,mThrowRange,map);
    float angle = Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), throwPosi);

    //爆弾スポーン
    auto bomb = ACTOR_M.GetBomb(mBombName.c_str(),CharacterType::PLAYER,map->GetLocalFromWorldPosition(throwPosi), throwPosi,angle,true);

    //爆弾をアクティブにする
    if (!bomb) {
        DEBUG_HELPER.Add("Bomb is generate error", 1.0f);
        return;
    }

    //爆弾の爆発範囲を計算
    auto size = mDefaultBombSize * (mExplosionRange / mDefaultExplosionRange);
    //爆発の描画速度計算
    auto playSpeed = defaultPlaySpeed / (mExplosionTime / mDefaultExplosionTime);
    //ステータス設定
    bomb->SetStatus(mDefaultBombSize,size,playSpeed,mAttack, mKnockBackStrength, mTimeToExplode,mExplosionTime,mExplosionRange,mHasContinuousDamage);
    //音を鳴らす
    SOUND_M.PlaySoundByID(mSlowSound);

    //クールタイム設定
    mCurrentTime = mThrowRate;
}

void PlayerThrowBombComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
    //ステータス設定
    auto bombStatus = std::dynamic_pointer_cast<BombWeaponStatus>(status);
    mAttack = bombStatus->mAttack;
    mThrowRate = bombStatus->mThrowRate.GetValue();
    mExplosionTime = bombStatus->mExplosionTime.GetValue();
    mTimeToExplode = bombStatus->mTimeToExplode.GetValue();
    mKnockBackStrength = bombStatus->mKnockBackStrength.GetValue();
    mThrowRange = bombStatus->mThrowRange;
    mExplosionRange = bombStatus->mExplosionRange.GetValue();
    mHasContinuousDamage = bombStatus->mHasContinuousDamage;

    //初期ステータス設定
    mDefaultExplosionRange = bombStatus->mExplosionRange.GetDefaultValue();
    mDefaultExplosionTime = bombStatus->mExplosionTime.GetDefaultValue();
}

Vector2D<float> PlayerThrowBombComponent::CalcRandomSlowPosition(std::shared_ptr<CharacterBase>owner, float radius, std::shared_ptr<Map> map)
{
    int maxCount = 10;
    auto centerLocation = owner->GetWorldPosition2D();

    auto& dh = Singleton<DebugHelper>::get_instance();

    for (int count = 0; count < maxCount; count++) {

        // 0から2πまでのランダムな角度を生成
        double angle = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;

        // 0からradiusまでのランダムな距離を生成 
        float distanceX = static_cast<float>(rand()) / RAND_MAX * radius + mThrowRangeMin;
        float distanceY = static_cast<float>(rand()) / RAND_MAX * radius + mThrowRangeMin;

        // 極座標からデカルト座標への変換 
        float x = centerLocation.x + static_cast<float>(distanceX * cos(angle));
        float y = centerLocation.y + static_cast<float>(distanceY * sin(angle));

        Vector2D<float>spawnPosition(x, y);
        
        if (map->GetTileOnWorldLocation(spawnPosition) == TileType::GROUND) {
            return spawnPosition;
        }
    }

    return Vector2D<float>();
}
