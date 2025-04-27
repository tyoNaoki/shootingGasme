#include "StatusManager.h"
#include "WeaponStatus.h"
#include "LevelUpHP.h"
#include <random>
#include "LevelUpGetReflectionGun.h"
#include "LevelUpReflectionCountUp.h"
#include "LevelUpReflectionBulletSpeedUp.h"
#include "LevelUpReflectionShotRateUp.h"
#include "LevelUpPlayerMelee.h"
#include "LevelUpPlayerMeleeSpeed.h"
#include "LevelUpPlayerMeleeWidthRange.h"
#include "LevelUpGunAttack.h"
#include "LevelUpGunShotRate.h"
#include "LevelUpGunAddPenetration.h"
#include "LevelUpGunBulletSpeed.h"
#include "LevelUpGetThrowBomb.h"
#include "LevelUpBombAttack.h"
#include "LevelUpThrowBombRate.h"
#include "LevelUpBombExplosionRange.h"
#include "LevelUpBombAddContinuous.h"


StatusManager::StatusManager(){
	//プレイヤーのステータス設定
	//名前,移動速度,最大レベル,経験値テーブルのための基礎経験値,経験値テーブルのための経験値重み
	mPlayerStatusDict = {300.0f, 300.0f,60,100.0f,1.5f };
	
	//敵キャラのステータス設定
	//名前,体力,移動速度,スコア,体力のレベルアップごとの上がり幅,武器のレベルアップごとの上り幅,倒されたときのドロップアイテムと選択される確率(0~1.0f),最大レベル
	//雑魚A
	mEnemyStatusDict["NormalEnemyA"] = {"雑魚A",20.0f, 70.0f,10.0f,2.0f,2.0f,2.0f,{{"LevelUpItem",0.8},{"HealItem",0.2},{"None",0.1}},5};

	//雑魚B
	mEnemyStatusDict["NormalEnemyB"] = {"雑魚B",25.0f, 120.0f,20.0f,1.1f,10.0f,2.0f,{{"LevelUpItem",0.9},{"HealItem",0.1},{"None",0.0}},5 };

	//雑魚C
	mEnemyStatusDict["NormalEnemyC"] = {"雑魚C",400.0f, 40.0f,250.0f,1.5f,100.0f,2.0f,{{"LevelUpItem",1.0},{"HealItem",0.1},{"None",0.0}},3 };

	//ボス
	//名前,体力,移動速度、スコア
	mBossStatusDict = {"ボスA",5000,1600.0f,3000.0f};

	//武器
	//敵近接武器
	float attack = 10.0f;
	mCurrentWeaponStatuses["NormalEnemyAttackComponent"] = std::make_shared<WeaponStatus>(attack);

	//敵強化近接武器
	attack = 50.0f;
	StatusValue swingSpeed = 5.0f;
	StatusValue widthRange = 100.0f;
	float heightRange = 200.0f;
	mCurrentWeaponStatuses["NormalEnemyMeleeComponent"] = std::make_shared<MeleeWeaponStatus>(attack, swingSpeed, widthRange, heightRange);

	//プレイヤー近接武器
	attack = 40.0f;
	swingSpeed = StatusValue(0.6f,0.4f);
	widthRange = 50.0f;
	heightRange = 150.0f;
	mCurrentWeaponStatuses["PlayerMeleeComponent"] = std::make_shared<MeleeWeaponStatus>(attack, swingSpeed,widthRange,heightRange);

	//プレイヤー銃
	attack = 10.0f;
	float bulletSpeed = 800.0f;
	StatusValue shotRate = StatusValue(0.5f,0.2f);
	float lockOnRange = 600.0f;
	float maxBulletSpeed = 2000.0f;
	mCurrentWeaponStatuses["PlayerGunComponent"] = std::make_shared<GunWeaponStatus>(attack, shotRate, bulletSpeed, lockOnRange,maxBulletSpeed);

	//プレイヤー反射銃
	attack = 30.0f;
	bulletSpeed = 200.0f;
	shotRate = StatusValue(2.0f,0.9f);
	lockOnRange = 800.0f;
	int reflectionMaxCount = 1;
	maxBulletSpeed = 600.0f;
	int maxReflectionMaxCount = 4;
	mCurrentWeaponStatuses["PlayerReflectionGunComponent"] = std::make_shared<GunWeaponStatus>(attack, shotRate,bulletSpeed,lockOnRange, reflectionMaxCount,maxBulletSpeed,maxReflectionMaxCount);

	//プレイヤー爆弾投げ
	attack = 150.0f;
	StatusValue throwRate = StatusValue(1.3f,1.0f);
	StatusValue explosionTime = 0.5f;
	StatusValue timeToExplode = 1.0f;
	StatusValue knockBackStrength = (10.0f,0.0f);
	float throwRange = 300.0f;
	StatusValue explosionRange = 100.0f;
	mCurrentWeaponStatuses["PlayerThrowBombComponent"] = std::make_shared<BombWeaponStatus>(attack,throwRate,throwRange,knockBackStrength,explosionRange,timeToExplode,explosionTime,false);

	//敵銃
	attack = 5.0f;
	bulletSpeed = 200.0f;
	shotRate = 2.0f;
	lockOnRange = 800.0f;
	mCurrentWeaponStatuses["NormalEnemyGunComponent"] = std::make_shared<GunWeaponStatus>(attack, shotRate, bulletSpeed,lockOnRange,0);

	//ボスの体当たり
	attack = 20.0f;
	float tackleKnockBack = 1000.0f;
	mCurrentWeaponStatuses["BossTackleComponent"] = std::make_shared<TackleWeaponStatus>(attack,tackleKnockBack);

	//ボスのレーザー攻撃
	attack = 40.0f;
	float knockBack = 1000.0f;
	float knockBackTime = 0.3f;
	int laserCount = 6;
	float laserspeed = 1500.0f;
	float widthSize = 50.0f;
	float spreadAngle = 20.0f;
	float fireTime = 3.0f;
	mCurrentWeaponStatuses["BossLaserComponent"] = std::make_shared<LaserWeaponStatus>(attack, knockBack,knockBackTime,laserCount,laserspeed,widthSize,spreadAngle,fireTime);

	//ボスのホーミング弾
	attack = 10.0f;
	bulletSpeed = 150.0f;
	shotRate = 2.0f;
	float homingTime = 5.0f;
	mCurrentWeaponStatuses["BossGunComponent"] = std::make_shared<GunWeaponStatus>(attack, shotRate, bulletSpeed,true,homingTime,bulletSpeed);

	//mItemStatuses
	//経験値(初期値、敵レベルに応じた追加経験値)
	mLevelUpItemStatusDict["NormalEnemyA"] = {60.0f,100.0f};

	mLevelUpItemStatusDict["NormalEnemyB"] = { 120.0f,120.0f };

	mLevelUpItemStatusDict["NormalEnemyC"] = { 800.0f,600.0f };

	//体力回復(初期値、敵レベルに応じた追加回復量)
	mHealItemStatusDict["NormalEnemyA"] = {50,50};

	mHealItemStatusDict["NormalEnemyB"] = { 200.0f,200.0f };

	mHealItemStatusDict["NormalEnemyC"] = { 400.0f,100.0f };

	//レベルアップ報酬
	//ステータス関係(報酬タイプ、報酬数値、名前、説明)
	//武器関係(武器報酬タグ、報酬タイプ、報酬数値、名前、説明)
	//体力アップ
	mRewardDict.push_back(
		std::make_shared<LevelUpHP>(RewardType::STATUS_UP, 50.0f, "体力アップ", "キャラクターの体力を50上げる"));
	
	//近接攻撃力アップ
	mRewardDict.push_back(std::make_shared<LevelUpPlayerMelee>("PlayerMeleeComponent",RewardType::WEAPON_UP,30,"攻撃力アップ","近接の攻撃力を30上げる"));

	//近接速度アップ
	mRewardDict.push_back(
		std::make_shared<LevelUpPlayerMeleeSpeed>("PlayerMeleeComponent", RewardType::WEAPON_UP, 0.1, "近接速度アップ", "キャラクターの近接速度を0.1秒早くする"));

	//近接横範囲強化
	mRewardDict.push_back(
		std::make_shared<LevelUpPlayerMeleeWidthRange>("PlayerMeleeComponent", RewardType::WEAPON_UP, 200, "近接範囲アップ", "キャラクターの近接範囲を広くする"));

	//銃攻撃力アップ
	mRewardDict.push_back(
		std::make_shared<LevelUpGunAttack>("PlayerGunComponent", RewardType::WEAPON_UP, 20.0f, "弾威力アップ", "発射する弾の威力を20上げる"));

	//銃発射速度アップ
	mRewardDict.push_back(
		std::make_shared<LevelUpGunShotRate>("PlayerGunComponent", RewardType::WEAPON_UP, 0.1f, "弾発射速度アップ", "弾の発射間隔を0.1秒短くする"));

	//銃貫通アップ
	mRewardDict.push_back(
		std::make_shared<LevelUpGunAddPenetration>("PlayerGunComponent", RewardType::WEAPON_UP, 1, "弾貫通力アップ", "弾の貫通力を１追加する"));

	//反射する銃を取得
	mRewardDict.push_back(
		std::make_shared<LevelUpGetReflectionGun>("PlayerReflectionGunComponent",RewardType::WEAPON_GET,"反射武器ゲット","画面の枠で反射する武器を入手する"));

	//反射威力アップ
	mRewardDict.push_back(
		std::make_shared<LevelUpGunAttack>("PlayerReflectionGunComponent", RewardType::WEAPON_UP, 20.0, "反射威力アップ", "反射する弾の威力を20上げる"));

	//反射回数アップ
	mRewardDict.push_back(
		std::make_shared<LevelUpReflectionCountUp>("PlayerReflectionGunComponent",RewardType::WEAPON_UP,1,"反射回数アップ", "弾の反射する回数が1回アップする"));

	//反射弾速度アップ
	mRewardDict.push_back(
		std::make_shared<LevelUpGunBulletSpeed>("PlayerReflectionGunComponent", RewardType::WEAPON_UP, 200, "反射速度アップ", "反射する弾の速度が200上がる"));

	//反射発射速度アップ
	mRewardDict.push_back(
		std::make_shared<LevelUpReflectionShotRateUp>("PlayerReflectionGunComponent", RewardType::WEAPON_UP, 0.2f, "反射の発射速度アップ", "反射する弾の発射速度を0.2秒早くする"));

	//爆弾投げを取得
	mRewardDict.push_back(
		std::make_shared<LevelUpGetThrowBomb>("PlayerThrowBombComponent", RewardType::WEAPON_GET, "爆弾武器ゲット", "周囲を吹き飛ばす爆弾を500m以内のランダムな場所に投げる"));

	//爆弾威力アップ
	mRewardDict.push_back(
		std::make_shared<LevelUpBombAttack>("PlayerThrowBombComponent", RewardType::WEAPON_UP, 20.0, "爆発威力アップ", "敵に与える爆発のダメージを20%上げる"));

	//爆弾投げる間隔短縮
	mRewardDict.push_back(
		std::make_shared<LevelUpThrowBombRate>("PlayerThrowBombComponent", RewardType::WEAPON_UP, 0.1f, "爆弾投げる間隔短縮", "爆弾の投げる間隔を0.1秒早くする"));

	//爆弾爆発範囲上昇
	mRewardDict.push_back(
		std::make_shared<LevelUpBombExplosionRange>("PlayerThrowBombComponent", RewardType::WEAPON_UP, 100.0f, "爆発範囲アップ", "爆発の範囲が100増える"));

	//爆弾が持続ダメージに変更
	mRewardDict.push_back(
		std::make_shared<LevelUpBombAddContinuous>("PlayerThrowBombComponent", RewardType::WEAPON_UP, 20.0f,2.0f, "爆発ダメージ形式変更", "爆発の範囲内の敵に持続してダメージが入るようになる。"));

	mPlayerExpTable.resize(mPlayerStatusDict.maxLevel);
	for (int level = 1; level <= mPlayerStatusDict.maxLevel; ++level) {
		mPlayerExpTable[level - 1] = static_cast<int>(mPlayerStatusDict.baseExp * std::pow(mPlayerStatusDict.expWeight, level - 1));
	}
}

StatusManager::~StatusManager()
{
	mEnemyStatusDict.clear();
	mCurrentEnemyStatuses.clear();
	mCurrentWeaponStatuses.clear();
	mLevelUpItemStatusDict.clear();
	mHealItemStatusDict.clear();
	mCurrentWeaponStatuses.clear();
	mRewardDict.clear();
	mCurrentRewards.clear();
}

void StatusManager::AddScore(int score)
{
	mPlayerScore+=score;
}

int StatusManager::GetScore()const
{
	return mPlayerScore;
}

void StatusManager::ResetScore()
{
	mPlayerScore = 0.0f;
}

void StatusManager::AddCount(CharacterType enemyType)
{
	if(enemyType== CharacterType::ENEMY){
		normalEnemyEleminatedCount++;
	}
	if(enemyType == CharacterType::BOSS){
		bossEnemyEleminatedCount++;
	}
	return;
}

int StatusManager::GetCount(CharacterType enemyType)
{
	//雑魚敵撃破カウント記録
	if (enemyType == CharacterType::ENEMY) {
		return normalEnemyEleminatedCount;
	}
	//ボス撃破カウント追加
	if (enemyType == CharacterType::BOSS) {
		return bossEnemyEleminatedCount;
	}
	return -1;
}

void StatusManager::ResetCount()
{
	//撃破カウント初期化
	normalEnemyEleminatedCount = 0;
	bossEnemyEleminatedCount = 0;
}

EnemyStatus StatusManager::calculateStatus(const int level, EnemyStatusDict& status)
{
	//レベルからステータスを計算
	return {status.name,level,status.hp + status.hp * (level - 1) * status.multiplyValueLevelUp,status.moveSpeed,status.score + status.score *(level - 1) * status.scorePlusValueLevelUp };
}

PlayerStatus StatusManager::GetCurrentPlayerStatus()
{
	return mCurrentPlayerStatus;
}

BossEnemyStatus StatusManager::GetCurrentBossStatus()
{
	return mCurrentBossStatus;
}

EnemyStatus StatusManager::GetCurrentEnemyStatus(const std::string enemyTypeName)
{
	return mCurrentEnemyStatuses[enemyTypeName];
}

std::shared_ptr<WeaponStatus> StatusManager::GetCurrentWeaponStatus(const std::string weaponTypeName)
{
	return mCurrentWeaponStatuses[weaponTypeName];
}

void StatusManager::ResetRewardS()
{
	mCurrentRewards.clear();
	//レベルアップ時の報酬の抽選リストに追加
	for(auto &x:mRewardDict){
		if(x->GetType()==RewardType::STATUS_UP||x->GetType()==RewardType::WEAPON_GET){
			mCurrentRewards.push_back(x);
		}
	}
}

void StatusManager::ResetSurvivalBonus()
{
	mSurvivalBonus = 1.0f;
}

PlayerStatus StatusManager::ResetPlayerStatus(const int level){
	//現在のプレイヤーのステータスを初期化
	mCurrentPlayerStatus = {level,mPlayerStatusDict .hp,mPlayerStatusDict.moveSpeed};
	return mCurrentPlayerStatus;
};

BossEnemyStatus StatusManager::ResetBossStatus(){
	//現在のボスのステータスを初期化
	mCurrentBossStatus = { mBossStatusDict.name,mBossStatusDict.hp,mBossStatusDict.moveSpeed };
	return mCurrentBossStatus;
}

EnemyStatus StatusManager::ResetEnemyStatus(const int level, const std::string& characterTypeName) {
	auto it = mEnemyStatusDict.find(characterTypeName);
	//レベルに対応したステータスに初期化
	if (it != mEnemyStatusDict.end()) {
		mCurrentEnemyStatuses[characterTypeName] = calculateStatus(level, it->second);
		return mCurrentEnemyStatuses[characterTypeName];
	}

	// デフォルトのステータスを返す
	return EnemyStatus();
};

std::shared_ptr<WeaponStatus> StatusManager::ResetWeaponStatus(const std::string& weaponType){
	auto it = mCurrentWeaponStatuses.find(weaponType);
	//各武器のステータスの初期化処理
	if (it != mCurrentWeaponStatuses.end()) {
		mCurrentWeaponStatuses[weaponType]->Reset();
		return mCurrentWeaponStatuses[weaponType];
	}

	//一旦null
	return nullptr;
}

float StatusManager::GetExpLevelUpItem(const int level, const std::string charaType)
{
	auto it = mLevelUpItemStatusDict.find(charaType);

	//レベルに対応したドロップする経験値量の計算
	if (it != mLevelUpItemStatusDict.end()) {
		return it->second.baseTakeExp + (level-1)*it->second.plusValueLevelUp;
	}

	return 0.0f;
}

float StatusManager::GetHealValueItem(const int level,const std::string charaType)
{
	auto it = mHealItemStatusDict.find(charaType);
	//レベルに対応したドロップする回復量の計算
	if (it != mHealItemStatusDict.end()) {
		return it->second.baseHealValue + (level - 1) * it->second.plusValHealValueLevelUp;
	}

	return 0.0f;
}

int StatusManager::GetNeedExp(int level)
{
	//レベルが０以下は存在しないので、エラー数値を返す
	if(level <=0){
		return 999999999999;
	}
	//レベルが最大か
	if(level >= mPlayerStatusDict.maxLevel){
		return 0;
	}
	//レベルアップのための必要な経験値を取得
	return mPlayerExpTable[level];
}

int StatusManager::GetPlayerCurrentLevel()const
{
	return mCurrentPlayerStatus.level;
}

int StatusManager::GetPlayerMaxLevel()const
{
	return mPlayerStatusDict.maxLevel;
}

std::vector<std::shared_ptr<LevelUpBonus>>StatusManager::GetRandomSelectRewards()
{
	std::vector<std::shared_ptr<LevelUpBonus>>randomRewards;
	int rewardCount = weightedRandom();
	//int rewardCount = 2;
	
	if(mCurrentRewards.size()<rewardCount){
		rewardCount = mCurrentRewards.size();
	}

	auto shuffled(mCurrentRewards); 
	
	// ランダムエンジンと分布を初期化
	std::random_device rd;
	std::mt19937 gen(rd());
	
	// シャッフル 
	std::shuffle(shuffled.begin(), shuffled.end(), gen);
	
	// 最初のnumElements個の要素を選択
	randomRewards = std::vector<std::shared_ptr<LevelUpBonus>>(shuffled.begin(), shuffled.begin() + rewardCount);
	
	return randomRewards;
}

std::vector<DropItem> StatusManager::GetDropItems(const std::string charaTypeName)
{
	auto it = mEnemyStatusDict.find(charaTypeName);
	//対象の敵のドロップするアイテムリストを取得
	if (it != mEnemyStatusDict.end()) {
		return it->second.dropItems;
	}

	return std::vector<DropItem>();
}

int StatusManager::GetEnemyMaxLevel(std::string typeName)
{
	return mEnemyStatusDict[typeName].maxLevel;
}

int StatusManager::GetRewardMaxCount()const
{
	return mRewardMaxCount;
}

float StatusManager::GetSurvivalBonus()const
{
	return mSurvivalBonus;
}

std::vector<EnemyStatus> StatusManager::GetAllCurrentEnemyStatus()
{
	std::vector<EnemyStatus>result;
	result.reserve(mCurrentEnemyStatuses.size());
	for(auto &x: mCurrentEnemyStatuses){
		result.push_back(x.second);
	}

	return result;
}

void StatusManager::UpdatePlayerStatus(PlayerStatus newStatus)
{
	mCurrentPlayerStatus = newStatus;
}

EnemyStatus StatusManager::UpdateEnemyStatus(const std::string enemyTypeName)
{
	//敵のレベルアップ時のステータス更新
	mCurrentEnemyStatuses[enemyTypeName].level++;
	mCurrentEnemyStatuses[enemyTypeName].hp = mCurrentEnemyStatuses[enemyTypeName].hp *mEnemyStatusDict[enemyTypeName].multiplyValueLevelUp;
	mCurrentEnemyStatuses[enemyTypeName].score = mCurrentEnemyStatuses[enemyTypeName].score * mEnemyStatusDict[enemyTypeName].multiplyValueLevelUp;
	return mCurrentEnemyStatuses[enemyTypeName];
}

int StatusManager::GetEnemyCount()
{
	return mCurrentEnemyStatuses.size();
}

void StatusManager::UpdateWeaponStatus(const std::string weaponName,std::shared_ptr<WeaponStatus> newStatus)
{
	mCurrentWeaponStatuses[weaponName] = newStatus;
}

void StatusManager::UpdateEnemyWeaponStatus(const std::string weaponName,const std::string enemyTypeName)
{
	mCurrentWeaponStatuses[weaponName]->mAttack += mEnemyStatusDict[enemyTypeName].weaponPlusValueLevelUp;
}

void StatusManager::UpdateRewards(const std::string tag)
{
	//レベルアップ時の抽選される報酬の追加
	for (auto itr = mRewardDict.begin(); itr != mRewardDict.end();) {
		if ((*itr)->GetType() == RewardType::WEAPON_UP && (*itr)->GetTag() == tag) {
			mCurrentRewards.push_back(*itr);
		}else if((*itr)->GetType() == RewardType::WEAPON_GET && (*itr)->GetTag() == tag) {
			mCurrentRewards.erase(std::remove_if(mCurrentRewards.begin(), mCurrentRewards.end(), [&](std::shared_ptr<LevelUpBonus> reward) {
				return reward == (*itr);
			}), mCurrentRewards.end());
		}
		itr++;
	}
	
}

void StatusManager::RemoveReward(const std::string name)
{
	//レベルアップ時の抽選される報酬の削除
	for (auto itr = mCurrentRewards.begin(); itr != mCurrentRewards.end();itr++) {
		if((*itr)->GetName()==name){
			itr = mCurrentRewards.erase(itr);
			return;
		}
	}
}

void StatusManager::UpdateSurvivalBonus()
{
	mSurvivalBonus *=2;
}

std::string StatusManager::GetCurrentBossName()
{
	return mCurrentBossStatus.name;
}

int StatusManager::weightedRandom()
{
	// 重み付きの値
	std::vector<int> values = {3, 3, 3, 3, 4, 4, 4, 5};
	
	// ランダムエンジンの初期化 
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, values.size() - 1);
	
	// 重み付けされた値からランダムに選択 
	return values[dis(gen)];
}

