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
	//�v���C���[�̃X�e�[�^�X�ݒ�
	//���O,�ړ����x,�ő僌�x��,�o���l�e�[�u���̂��߂̊�b�o���l,�o���l�e�[�u���̂��߂̌o���l�d��
	mPlayerStatusDict = {300.0f, 300.0f,60,100.0f,1.5f };
	
	//�G�L�����̃X�e�[�^�X�ݒ�
	//���O,�̗�,�ړ����x,�X�R�A,�̗͂̃��x���A�b�v���Ƃ̏オ�蕝,����̃��x���A�b�v���Ƃ̏�蕝,�|���ꂽ�Ƃ��̃h���b�v�A�C�e���ƑI�������m��(0~1.0f),�ő僌�x��
	//�G��A
	mEnemyStatusDict["NormalEnemyA"] = {"�G��A",20.0f, 70.0f,10.0f,2.0f,2.0f,2.0f,{{"LevelUpItem",0.8},{"HealItem",0.2},{"None",0.1}},5};

	//�G��B
	mEnemyStatusDict["NormalEnemyB"] = {"�G��B",25.0f, 120.0f,20.0f,1.1f,10.0f,2.0f,{{"LevelUpItem",0.9},{"HealItem",0.1},{"None",0.0}},5 };

	//�G��C
	mEnemyStatusDict["NormalEnemyC"] = {"�G��C",400.0f, 40.0f,250.0f,1.5f,100.0f,2.0f,{{"LevelUpItem",1.0},{"HealItem",0.1},{"None",0.0}},3 };

	//�{�X
	//���O,�̗�,�ړ����x�A�X�R�A
	mBossStatusDict = {"�{�XA",5000,1600.0f,3000.0f};

	//����
	//�G�ߐڕ���
	float attack = 10.0f;
	mCurrentWeaponStatuses["NormalEnemyAttackComponent"] = std::make_shared<WeaponStatus>(attack);

	//�G�����ߐڕ���
	attack = 50.0f;
	StatusValue swingSpeed = 5.0f;
	StatusValue widthRange = 100.0f;
	float heightRange = 200.0f;
	mCurrentWeaponStatuses["NormalEnemyMeleeComponent"] = std::make_shared<MeleeWeaponStatus>(attack, swingSpeed, widthRange, heightRange);

	//�v���C���[�ߐڕ���
	attack = 40.0f;
	swingSpeed = StatusValue(0.6f,0.4f);
	widthRange = 50.0f;
	heightRange = 150.0f;
	mCurrentWeaponStatuses["PlayerMeleeComponent"] = std::make_shared<MeleeWeaponStatus>(attack, swingSpeed,widthRange,heightRange);

	//�v���C���[�e
	attack = 10.0f;
	float bulletSpeed = 800.0f;
	StatusValue shotRate = StatusValue(0.5f,0.2f);
	float lockOnRange = 600.0f;
	float maxBulletSpeed = 2000.0f;
	mCurrentWeaponStatuses["PlayerGunComponent"] = std::make_shared<GunWeaponStatus>(attack, shotRate, bulletSpeed, lockOnRange,maxBulletSpeed);

	//�v���C���[���ˏe
	attack = 30.0f;
	bulletSpeed = 200.0f;
	shotRate = StatusValue(2.0f,0.9f);
	lockOnRange = 800.0f;
	int reflectionMaxCount = 1;
	maxBulletSpeed = 600.0f;
	int maxReflectionMaxCount = 4;
	mCurrentWeaponStatuses["PlayerReflectionGunComponent"] = std::make_shared<GunWeaponStatus>(attack, shotRate,bulletSpeed,lockOnRange, reflectionMaxCount,maxBulletSpeed,maxReflectionMaxCount);

	//�v���C���[���e����
	attack = 150.0f;
	StatusValue throwRate = StatusValue(1.3f,1.0f);
	StatusValue explosionTime = 0.5f;
	StatusValue timeToExplode = 1.0f;
	StatusValue knockBackStrength = (10.0f,0.0f);
	float throwRange = 300.0f;
	StatusValue explosionRange = 100.0f;
	mCurrentWeaponStatuses["PlayerThrowBombComponent"] = std::make_shared<BombWeaponStatus>(attack,throwRate,throwRange,knockBackStrength,explosionRange,timeToExplode,explosionTime,false);

	//�G�e
	attack = 5.0f;
	bulletSpeed = 200.0f;
	shotRate = 2.0f;
	lockOnRange = 800.0f;
	mCurrentWeaponStatuses["NormalEnemyGunComponent"] = std::make_shared<GunWeaponStatus>(attack, shotRate, bulletSpeed,lockOnRange,0);

	//�{�X�̑̓�����
	attack = 20.0f;
	float tackleKnockBack = 1000.0f;
	mCurrentWeaponStatuses["BossTackleComponent"] = std::make_shared<TackleWeaponStatus>(attack,tackleKnockBack);

	//�{�X�̃��[�U�[�U��
	attack = 40.0f;
	float knockBack = 1000.0f;
	float knockBackTime = 0.3f;
	int laserCount = 6;
	float laserspeed = 1500.0f;
	float widthSize = 50.0f;
	float spreadAngle = 20.0f;
	float fireTime = 3.0f;
	mCurrentWeaponStatuses["BossLaserComponent"] = std::make_shared<LaserWeaponStatus>(attack, knockBack,knockBackTime,laserCount,laserspeed,widthSize,spreadAngle,fireTime);

	//�{�X�̃z�[�~���O�e
	attack = 10.0f;
	bulletSpeed = 150.0f;
	shotRate = 2.0f;
	float homingTime = 5.0f;
	mCurrentWeaponStatuses["BossGunComponent"] = std::make_shared<GunWeaponStatus>(attack, shotRate, bulletSpeed,true,homingTime,bulletSpeed);

	//mItemStatuses
	//�o���l(�����l�A�G���x���ɉ������ǉ��o���l)
	mLevelUpItemStatusDict["NormalEnemyA"] = {60.0f,100.0f};

	mLevelUpItemStatusDict["NormalEnemyB"] = { 120.0f,120.0f };

	mLevelUpItemStatusDict["NormalEnemyC"] = { 800.0f,600.0f };

	//�̗͉�(�����l�A�G���x���ɉ������ǉ��񕜗�)
	mHealItemStatusDict["NormalEnemyA"] = {50,50};

	mHealItemStatusDict["NormalEnemyB"] = { 200.0f,200.0f };

	mHealItemStatusDict["NormalEnemyC"] = { 400.0f,100.0f };

	//���x���A�b�v��V
	//�X�e�[�^�X�֌W(��V�^�C�v�A��V���l�A���O�A����)
	//����֌W(�����V�^�O�A��V�^�C�v�A��V���l�A���O�A����)
	//�̗̓A�b�v
	mRewardDict.push_back(
		std::make_shared<LevelUpHP>(RewardType::STATUS_UP, 50.0f, "�̗̓A�b�v", "�L�����N�^�[�̗̑͂�50�グ��"));
	
	//�ߐڍU���̓A�b�v
	mRewardDict.push_back(std::make_shared<LevelUpPlayerMelee>("PlayerMeleeComponent",RewardType::WEAPON_UP,30,"�U���̓A�b�v","�ߐڂ̍U���͂�30�グ��"));

	//�ߐڑ��x�A�b�v
	mRewardDict.push_back(
		std::make_shared<LevelUpPlayerMeleeSpeed>("PlayerMeleeComponent", RewardType::WEAPON_UP, 0.1, "�ߐڑ��x�A�b�v", "�L�����N�^�[�̋ߐڑ��x��0.1�b��������"));

	//�ߐډ��͈͋���
	mRewardDict.push_back(
		std::make_shared<LevelUpPlayerMeleeWidthRange>("PlayerMeleeComponent", RewardType::WEAPON_UP, 200, "�ߐڔ͈̓A�b�v", "�L�����N�^�[�̋ߐڔ͈͂��L������"));

	//�e�U���̓A�b�v
	mRewardDict.push_back(
		std::make_shared<LevelUpGunAttack>("PlayerGunComponent", RewardType::WEAPON_UP, 20.0f, "�e�З̓A�b�v", "���˂���e�̈З͂�20�グ��"));

	//�e���ˑ��x�A�b�v
	mRewardDict.push_back(
		std::make_shared<LevelUpGunShotRate>("PlayerGunComponent", RewardType::WEAPON_UP, 0.1f, "�e���ˑ��x�A�b�v", "�e�̔��ˊԊu��0.1�b�Z������"));

	//�e�ђʃA�b�v
	mRewardDict.push_back(
		std::make_shared<LevelUpGunAddPenetration>("PlayerGunComponent", RewardType::WEAPON_UP, 1, "�e�ђʗ̓A�b�v", "�e�̊ђʗ͂��P�ǉ�����"));

	//���˂���e���擾
	mRewardDict.push_back(
		std::make_shared<LevelUpGetReflectionGun>("PlayerReflectionGunComponent",RewardType::WEAPON_GET,"���˕���Q�b�g","��ʂ̘g�Ŕ��˂��镐�����肷��"));

	//���ˈЗ̓A�b�v
	mRewardDict.push_back(
		std::make_shared<LevelUpGunAttack>("PlayerReflectionGunComponent", RewardType::WEAPON_UP, 20.0, "���ˈЗ̓A�b�v", "���˂���e�̈З͂�20�グ��"));

	//���ˉ񐔃A�b�v
	mRewardDict.push_back(
		std::make_shared<LevelUpReflectionCountUp>("PlayerReflectionGunComponent",RewardType::WEAPON_UP,1,"���ˉ񐔃A�b�v", "�e�̔��˂���񐔂�1��A�b�v����"));

	//���˒e���x�A�b�v
	mRewardDict.push_back(
		std::make_shared<LevelUpGunBulletSpeed>("PlayerReflectionGunComponent", RewardType::WEAPON_UP, 200, "���ˑ��x�A�b�v", "���˂���e�̑��x��200�オ��"));

	//���˔��ˑ��x�A�b�v
	mRewardDict.push_back(
		std::make_shared<LevelUpReflectionShotRateUp>("PlayerReflectionGunComponent", RewardType::WEAPON_UP, 0.2f, "���˂̔��ˑ��x�A�b�v", "���˂���e�̔��ˑ��x��0.2�b��������"));

	//���e�������擾
	mRewardDict.push_back(
		std::make_shared<LevelUpGetThrowBomb>("PlayerThrowBombComponent", RewardType::WEAPON_GET, "���e����Q�b�g", "���͂𐁂���΂����e��500m�ȓ��̃����_���ȏꏊ�ɓ�����"));

	//���e�З̓A�b�v
	mRewardDict.push_back(
		std::make_shared<LevelUpBombAttack>("PlayerThrowBombComponent", RewardType::WEAPON_UP, 20.0, "�����З̓A�b�v", "�G�ɗ^���锚���̃_���[�W��20%�グ��"));

	//���e������Ԋu�Z�k
	mRewardDict.push_back(
		std::make_shared<LevelUpThrowBombRate>("PlayerThrowBombComponent", RewardType::WEAPON_UP, 0.1f, "���e������Ԋu�Z�k", "���e�̓�����Ԋu��0.1�b��������"));

	//���e�����͈͏㏸
	mRewardDict.push_back(
		std::make_shared<LevelUpBombExplosionRange>("PlayerThrowBombComponent", RewardType::WEAPON_UP, 100.0f, "�����͈̓A�b�v", "�����͈̔͂�100������"));

	//���e�������_���[�W�ɕύX
	mRewardDict.push_back(
		std::make_shared<LevelUpBombAddContinuous>("PlayerThrowBombComponent", RewardType::WEAPON_UP, 20.0f,2.0f, "�����_���[�W�`���ύX", "�����͈͓̔��̓G�Ɏ������ă_���[�W������悤�ɂȂ�B"));

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
	//�G���G���j�J�E���g�L�^
	if (enemyType == CharacterType::ENEMY) {
		return normalEnemyEleminatedCount;
	}
	//�{�X���j�J�E���g�ǉ�
	if (enemyType == CharacterType::BOSS) {
		return bossEnemyEleminatedCount;
	}
	return -1;
}

void StatusManager::ResetCount()
{
	//���j�J�E���g������
	normalEnemyEleminatedCount = 0;
	bossEnemyEleminatedCount = 0;
}

EnemyStatus StatusManager::calculateStatus(const int level, EnemyStatusDict& status)
{
	//���x������X�e�[�^�X���v�Z
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
	//���x���A�b�v���̕�V�̒��I���X�g�ɒǉ�
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
	//���݂̃v���C���[�̃X�e�[�^�X��������
	mCurrentPlayerStatus = {level,mPlayerStatusDict .hp,mPlayerStatusDict.moveSpeed};
	return mCurrentPlayerStatus;
};

BossEnemyStatus StatusManager::ResetBossStatus(){
	//���݂̃{�X�̃X�e�[�^�X��������
	mCurrentBossStatus = { mBossStatusDict.name,mBossStatusDict.hp,mBossStatusDict.moveSpeed };
	return mCurrentBossStatus;
}

EnemyStatus StatusManager::ResetEnemyStatus(const int level, const std::string& characterTypeName) {
	auto it = mEnemyStatusDict.find(characterTypeName);
	//���x���ɑΉ������X�e�[�^�X�ɏ�����
	if (it != mEnemyStatusDict.end()) {
		mCurrentEnemyStatuses[characterTypeName] = calculateStatus(level, it->second);
		return mCurrentEnemyStatuses[characterTypeName];
	}

	// �f�t�H���g�̃X�e�[�^�X��Ԃ�
	return EnemyStatus();
};

std::shared_ptr<WeaponStatus> StatusManager::ResetWeaponStatus(const std::string& weaponType){
	auto it = mCurrentWeaponStatuses.find(weaponType);
	//�e����̃X�e�[�^�X�̏���������
	if (it != mCurrentWeaponStatuses.end()) {
		mCurrentWeaponStatuses[weaponType]->Reset();
		return mCurrentWeaponStatuses[weaponType];
	}

	//��Unull
	return nullptr;
}

float StatusManager::GetExpLevelUpItem(const int level, const std::string charaType)
{
	auto it = mLevelUpItemStatusDict.find(charaType);

	//���x���ɑΉ������h���b�v����o���l�ʂ̌v�Z
	if (it != mLevelUpItemStatusDict.end()) {
		return it->second.baseTakeExp + (level-1)*it->second.plusValueLevelUp;
	}

	return 0.0f;
}

float StatusManager::GetHealValueItem(const int level,const std::string charaType)
{
	auto it = mHealItemStatusDict.find(charaType);
	//���x���ɑΉ������h���b�v����񕜗ʂ̌v�Z
	if (it != mHealItemStatusDict.end()) {
		return it->second.baseHealValue + (level - 1) * it->second.plusValHealValueLevelUp;
	}

	return 0.0f;
}

int StatusManager::GetNeedExp(int level)
{
	//���x�����O�ȉ��͑��݂��Ȃ��̂ŁA�G���[���l��Ԃ�
	if(level <=0){
		return 999999999999;
	}
	//���x�����ő傩
	if(level >= mPlayerStatusDict.maxLevel){
		return 0;
	}
	//���x���A�b�v�̂��߂̕K�v�Ȍo���l���擾
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
	
	// �����_���G���W���ƕ��z��������
	std::random_device rd;
	std::mt19937 gen(rd());
	
	// �V���b�t�� 
	std::shuffle(shuffled.begin(), shuffled.end(), gen);
	
	// �ŏ���numElements�̗v�f��I��
	randomRewards = std::vector<std::shared_ptr<LevelUpBonus>>(shuffled.begin(), shuffled.begin() + rewardCount);
	
	return randomRewards;
}

std::vector<DropItem> StatusManager::GetDropItems(const std::string charaTypeName)
{
	auto it = mEnemyStatusDict.find(charaTypeName);
	//�Ώۂ̓G�̃h���b�v����A�C�e�����X�g���擾
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
	//�G�̃��x���A�b�v���̃X�e�[�^�X�X�V
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
	//���x���A�b�v���̒��I������V�̒ǉ�
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
	//���x���A�b�v���̒��I������V�̍폜
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
	// �d�ݕt���̒l
	std::vector<int> values = {3, 3, 3, 3, 4, 4, 4, 5};
	
	// �����_���G���W���̏����� 
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, values.size() - 1);
	
	// �d�ݕt�����ꂽ�l���烉���_���ɑI�� 
	return values[dis(gen)];
}

