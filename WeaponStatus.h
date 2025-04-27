#pragma once
#ifndef WEAPONSTATUS_H
#define WEAPONSTATUS_H

#include <unordered_map>
#include <string>
#include <memory>
#include "BulletStatus.h"
#include "StatusManager.h"

struct StatusValue;

class WeaponStatus {
public:
	float mAttack;
	float mDefaultAttack;
	
	//攻撃力のみ特に強化回数制限を入れない
	WeaponStatus(float attack) : mAttack(attack),mDefaultAttack(mAttack){}
	virtual ~WeaponStatus() = default;
	virtual void Reset() { mAttack = mDefaultAttack;}
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
	StatusValue mSwingSpeed;
	StatusValue mWidthRange;
	float mHeightRange;

	MeleeWeaponStatus(float attack, StatusValue swingSpeed, StatusValue widthRange,float heightRange) : mSwingSpeed(swingSpeed),mWidthRange(widthRange),mHeightRange(heightRange),WeaponStatus(attack) {}
	void Reset() override {
		WeaponStatus::Reset();
		mSwingSpeed.ResetToDefault();
		mWidthRange.ResetToDefault();
	}
};

class GunWeaponStatus : public WeaponStatus{
public:
	StatusValue mShotRate;
	float mBulletSpeed;
	float mLockOnRange = 0.0f;
	bool mDefaultIsPenetration = false;
	bool mIsPenetration = false;
	bool mIsReflection = false;
	bool mIsHoming = false;
	float mHomingTime = 3.0f;
	int mPenetrationMaxCount = 0;
	int mReflectionMaxCount = 0;
	float mDefaultBulletSpeed;
	int mDefaultPenetrationMaxCount = 0;
	int mDefaultReflectionMaxCount = 0;
	float mMaxBulletSpeed;
	int mMaxPenetrationMaxCount = 2;
	int mMaxReflectionMaxCount = 5;

	//非貫通銃
	GunWeaponStatus(float attack, StatusValue shotRate,float bulletSpeed,float lockOnRange,float maxBulletSpeed) : mShotRate(shotRate),mBulletSpeed(bulletSpeed),mLockOnRange(lockOnRange),mDefaultBulletSpeed(bulletSpeed),mMaxBulletSpeed(maxBulletSpeed),WeaponStatus(attack) {}

	//貫通銃
	GunWeaponStatus(float attack, StatusValue shotRate, float bulletSpeed,float lockOnRange,int penetrationMaxCount,int maxPenetrationMaxCount, float maxBulletSpeed) : mShotRate(shotRate), mBulletSpeed(bulletSpeed),mLockOnRange(lockOnRange),mIsPenetration(true), mDefaultIsPenetration(true),mPenetrationMaxCount(penetrationMaxCount),mMaxPenetrationMaxCount(maxPenetrationMaxCount),mDefaultBulletSpeed(bulletSpeed), mMaxBulletSpeed(maxBulletSpeed), WeaponStatus(attack) {}

	//ホーミング銃
	GunWeaponStatus(float attack, StatusValue shotRate, float bulletSpeed,bool isHoming,float homingTime, float maxBulletSpeed) : mShotRate(shotRate), mBulletSpeed(bulletSpeed),mDefaultBulletSpeed(bulletSpeed),mIsHoming(isHoming),mHomingTime(homingTime), mMaxBulletSpeed(maxBulletSpeed), WeaponStatus(attack) {}

	//反射銃
	GunWeaponStatus(float attack, StatusValue shotRate, float bulletSpeed,float lockOnRange,int reflectionMaxCount,float maxBulletSpeed,int maxReflectionMaxCount) :mIsReflection(true),mShotRate(shotRate), mBulletSpeed(bulletSpeed),mLockOnRange(lockOnRange), mDefaultBulletSpeed(bulletSpeed),mDefaultReflectionMaxCount(reflectionMaxCount),mMaxBulletSpeed(maxBulletSpeed),mMaxReflectionMaxCount(maxReflectionMaxCount), WeaponStatus(attack) {}

	void Reset() override {
		WeaponStatus::Reset();
		mShotRate.ResetToDefault();
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
	StatusValue mThrowRate;
	float mThrowRange;
	StatusValue mKnockBackStrength;
	StatusValue mExplosionRange;
	StatusValue mTimeToExplode;
	StatusValue mExplosionTime;
	bool mHasContinuousDamage;

	BombWeaponStatus(float attack, StatusValue throwRate,float throwRange, StatusValue knockBackStrength, StatusValue explosionRange, StatusValue timeToExplode, StatusValue explosionTime,bool hasContinuousDamage):mThrowRate(throwRate), mThrowRange(throwRange),mKnockBackStrength(knockBackStrength),mExplosionRange(explosionRange),mTimeToExplode(timeToExplode),mExplosionTime(explosionTime),mHasContinuousDamage(hasContinuousDamage),WeaponStatus(attack){}

	void Reset() override {
		WeaponStatus::Reset();
		mKnockBackStrength.ResetToDefault();
		mThrowRate.ResetToDefault();
		mTimeToExplode.ResetToDefault();
		mExplosionTime.ResetToDefault();
		mExplosionRange.ResetToDefault();
		mHasContinuousDamage = false;
	}
};

#endif // WEAPONSTATUS_H

