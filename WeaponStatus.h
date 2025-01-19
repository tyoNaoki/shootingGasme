#pragma once
#ifndef WEAPONSTATUS_H
#define WEAPONSTATUS_H

#include <unordered_map>
#include <string>
#include <memory>


class WeaponStatus {
public:
	float mAttack;
	float mDefaultAttack;
	
	WeaponStatus(float attack) : mAttack(attack),mDefaultAttack(attack) {}
	virtual ~WeaponStatus() = default;
	virtual void Reset() { mAttack = mDefaultAttack; }
};

class TackleWeaponStatus : public WeaponStatus {
public:
	float mTackleKnockBack;

	TackleWeaponStatus(float attack,float tackleKnockBack):mTackleKnockBack(tackleKnockBack),WeaponStatus(attack){}
};

class LaserWeaponStatus : public WeaponStatus {
public:
	float mKnockBack;
	float mKnockBackTime;
	int mLaserCount;
	float mSpeed;
	float mWidthSize;
	float mSpreadAngle;
	float mFireTime;

	LaserWeaponStatus(float attack, float laserKnockBack,float knockBackTime,int laserCount,float laserSpeed,float widthSize,float spreadAngle,float fireTime) :mKnockBack(laserKnockBack),mKnockBackTime(knockBackTime),mLaserCount(laserCount),mSpeed(laserSpeed),mWidthSize(widthSize),mSpreadAngle(spreadAngle),mFireTime(fireTime),WeaponStatus(attack) {}
};

class MeleeWeaponStatus : public WeaponStatus {
public:
	float mSwingSpeed;
	float mWidthRange;
	float mHeightRange;
	float mDefaultSwingSpeed;
	float mDefaultWidthRange;
	float mMinSwingSpeed;

	MeleeWeaponStatus(float attack,float swingSpeed,float widthRange,float heightRange,float minSwingSpeed) : mSwingSpeed(swingSpeed),mWidthRange(widthRange),mHeightRange(heightRange),mDefaultSwingSpeed(swingSpeed),mDefaultWidthRange(widthRange),mMinSwingSpeed(minSwingSpeed),WeaponStatus(attack) {}
	void Reset() override { WeaponStatus::Reset(); mSwingSpeed = mDefaultSwingSpeed;mWidthRange = mDefaultWidthRange; }
};

class GunWeaponStatus : public WeaponStatus{
public:
	float mShotRate;
	float mBulletSpeed;
	float mLockOnRange = 0.0f;
	bool mDefaultIsPenetration = false;
	bool mIsPenetration = false;
	bool mIsReflection = false;
	bool mIsHoming = false;
	float mHomingTime = 3.0f;
	int mPenetrationMaxCount = 0;
	int mReflectionMaxCount = 0;
	float mDefaultShotRate;
	float mDefaultBulletSpeed;
	int mDefaultPenetrationMaxCount = 0;
	int mDefaultReflectionMaxCount = 0;
	float mMinShotRate;
	float mMaxBulletSpeed;
	int mMaxPenetrationMaxCount = 2;
	int mMaxReflectionMaxCount = 5;

	//非貫通銃
	GunWeaponStatus(float attack, float shotRate,float bulletSpeed,float lockOnRange,float minShotRate,float maxBulletSpeed) : mShotRate(shotRate),mBulletSpeed(bulletSpeed),mLockOnRange(lockOnRange),mDefaultShotRate(shotRate),mDefaultBulletSpeed(bulletSpeed),mMinShotRate(minShotRate),mMaxBulletSpeed(maxBulletSpeed),WeaponStatus(attack) {}

	//貫通銃
	GunWeaponStatus(float attack, float shotRate, float bulletSpeed,float lockOnRange,int penetrationMaxCount,int maxPenetrationMaxCount,float minShotRate, float maxBulletSpeed) : mShotRate(shotRate), mBulletSpeed(bulletSpeed),mLockOnRange(lockOnRange),mIsPenetration(true), mDefaultIsPenetration(true),mPenetrationMaxCount(penetrationMaxCount),mMaxPenetrationMaxCount(maxPenetrationMaxCount),mDefaultShotRate(shotRate), mDefaultBulletSpeed(bulletSpeed), mMinShotRate(minShotRate), mMaxBulletSpeed(maxBulletSpeed), WeaponStatus(attack) {}

	//ホーミング銃
	GunWeaponStatus(float attack, float shotRate, float bulletSpeed,bool isHoming,float homingTime,float minShotRate, float maxBulletSpeed) : mShotRate(shotRate), mBulletSpeed(bulletSpeed),mDefaultShotRate(shotRate), mDefaultBulletSpeed(bulletSpeed),mIsHoming(isHoming),mHomingTime(homingTime), mMinShotRate(minShotRate), mMaxBulletSpeed(maxBulletSpeed), WeaponStatus(attack) {}

	//反射銃
	GunWeaponStatus(float attack, float shotRate, float bulletSpeed,float lockOnRange,int reflectionMaxCount,float minShotRate,float maxBulletSpeed,int maxReflectionMaxCount) :mIsReflection(true),mShotRate(shotRate), mBulletSpeed(bulletSpeed),mLockOnRange(lockOnRange), mDefaultShotRate(shotRate), mDefaultBulletSpeed(bulletSpeed),mDefaultReflectionMaxCount(reflectionMaxCount),mMinShotRate(minShotRate), mMaxBulletSpeed(maxBulletSpeed),mMaxReflectionMaxCount(maxReflectionMaxCount), WeaponStatus(attack) {}

	void Reset() override {
		WeaponStatus::Reset();
		mShotRate = mDefaultShotRate;
		mBulletSpeed = mDefaultBulletSpeed;

		//最初から貫通弾の場合、貫通はデフォルト値
		if(mDefaultIsPenetration){
			mPenetrationMaxCount = mDefaultPenetrationMaxCount;
		}else if(mIsReflection){ //最初から反射の場合、デフォルト値にリセット
			mReflectionMaxCount  = mDefaultReflectionMaxCount;
		}else{
			mIsPenetration = false;
			mPenetrationMaxCount = 0;
			mReflectionMaxCount = 0;
		}
	}
};

class BombWeaponStatus : public WeaponStatus {
public:
	float mThrowRate;
	float mThrowRange;
	float mKnockBackStrength;
	float mExplosionRange;
	float mTimeToExplode;
	float mExplosionTime;
	float mMinThrowRate;
	float mDefaultThrowRate;
	float mDefaultTimeToExplode;
	float mDefaultExplosionTime;
	float mDefaultExplosionRange;
	float mDefaultKnockBackStrength;
	bool mHasContinuousDamage;

	BombWeaponStatus(float attack,float throwRate,float throwRange,float knockBackStrength,float explosionRange,float timeToExplode,float explosionTime,float minThrowRate, bool hasContinuousDamage):mThrowRate(throwRate), mThrowRange(throwRange),mKnockBackStrength(knockBackStrength),mExplosionRange(explosionRange),mTimeToExplode(timeToExplode),mExplosionTime(explosionTime), mMinThrowRate(minThrowRate),mDefaultKnockBackStrength(knockBackStrength),mDefaultThrowRate(throwRate),mDefaultTimeToExplode(timeToExplode),mDefaultExplosionTime(explosionTime),mDefaultExplosionRange(explosionRange),mHasContinuousDamage(hasContinuousDamage),WeaponStatus(attack){}

	void Reset() override {
		WeaponStatus::Reset();
		mKnockBackStrength = mDefaultKnockBackStrength;
		mThrowRate = mDefaultThrowRate;
		mTimeToExplode = mDefaultTimeToExplode;
		mExplosionTime = mDefaultExplosionTime;
		mExplosionRange = mDefaultExplosionRange;
		mHasContinuousDamage = false;
	}
};

#endif // WEAPONSTATUS_H

