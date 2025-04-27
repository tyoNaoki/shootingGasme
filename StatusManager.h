#ifndef STATUSMANAGER_H
#define STATUSMANAGER_H

#pragma once
#include "Singleton.h"
#include <unordered_map>
#include "CharacterBase.h"

#define STATUS Singleton<StatusManager>::get_instance()

class LevelUpBonus;
class WeaponStatus;

struct StatusValue {

	float mCurrentValue;
	float mPowerUpMaxValue;
	float mDefaultValue;

public:
	StatusValue(float defaultValue, float maxValue = 0)
		: mCurrentValue(defaultValue), mPowerUpMaxValue(maxValue), mDefaultValue(defaultValue) {}

	StatusValue() = default;

	void ResetToDefault() {
		mCurrentValue = mDefaultValue;
	}

	float GetValue(){return mCurrentValue;}

	float GetDefaultValue(){return mDefaultValue;}

	void SetValue(float value){
		mCurrentValue = ClampMaxAndMin(value);
	}

	void AddValue(float value){
		mCurrentValue = ClampMaxAndMin(mCurrentValue + value);
	}

	StatusValue& operator =(const float value) {
		mCurrentValue = value; 
		mDefaultValue = value;
		mPowerUpMaxValue = value;
		return *this; 
	}

	bool CanPowerUp(const float value)
	{
		if (mDefaultValue > mPowerUpMaxValue)
		{
			return value >= mPowerUpMaxValue;
		}
		else
		{
			return value <= mPowerUpMaxValue;
		}
	}

private:

	float ClampMaxAndMin(float value) {
		if(mDefaultValue > mPowerUpMaxValue)
		{
			return max(value,mPowerUpMaxValue);
		}
		else
		{
			return min(value,mPowerUpMaxValue);
		}
	}
};

struct DropItem {
	//�h���b�v�i�̖��O
	std::string itemName;
	//�h���b�v�A�C�e���I���̒��I�m��
	float dropWeight;
};

struct PlayerStatusDict {
	float hp = 0.0f;
	float moveSpeed = 0.0f;
	
	//�o���l�e�[�u���쐬�ׂ̈̒����p�����[�^
	int maxLevel;
	//��b�o���l+��b�o���l^expWeight
	float baseExp;
	float expWeight;
};

struct PlayerStatus {
	int level = 0;
	float hp = 0.0f;
	float moveSpeed = 0.0f;
};

struct EnemyStatusDict {
	//�G�̖��O
	std::string name;
	float hp = 0.0f;
	float moveSpeed = 0.0f;
	float score = 0.0f;

	//���x���A�b�v����HP�̑�����
	float multiplyValueLevelUp = 2.0f;
	//���x���A�b�v���̍U���͂̑���
	float weaponPlusValueLevelUp = 10.0f;
	//���x���A�b�v���̌��j�X�R�A��
	float scorePlusValueLevelUp = 2.0f;
	//���j���ꂽ�Ƃ��̃h���b�v�A�C�e��
	std::vector<DropItem>dropItems;
	//�ő僌�x��
	int maxLevel = 0;
};

struct EnemyStatus {
	//�G�̖��O
	std::string name;
	int level = 0;
	float hp = 0.0f;
	float moveSpeed = 0.0f;
	//���j���ꂽ�Ƃ��̃X�R�A
	float score = 0.0f;
};

struct BossEnemyStatusDict {
	//�{�X�̖��O
	std::string name;
	float hp = 0.0f;
	float moveSpeed = 0.0f;
	//���j���ꂽ�Ƃ��̃X�R�A
	float score = 0.0f;
};

struct BossEnemyStatus{
	//�{�X�̖��O
	std::string name;

	float hp = 0.0f;
	float moveSpeed = 0.0f;
	//���j���ꂽ�Ƃ��̃X�R�A
	float score = 0.0f;
};
//�h���b�v�A�C�e���̌o���l
struct LevelUpItemStatusDict {
	//�����o���l��
	float baseTakeExp;
	//���j�����G�̃��x�����A�o���l�ʂ������Z�ő���
	float plusValueLevelUp;
};

struct HealItemStatusDict {
	//�����񕜗�
	float baseHealValue;
	//���j�����G�̃��x�����A�񕜗ʂ������Z�ő���
	float plusValHealValueLevelUp;
};

struct PlayResult {
	//�v���C���[�̃��x���\�L
	float level;
	//�v���C���[�̓G���j�̍��v�X�R�A
	float score;
	//�G���G�̌��j��
	int normalEnemyEliminatedCount = 0;
	//�{�X�̌��j��
	int bossEliminatedCount = 0;
};

class StatusManager
{
friend class Singleton<StatusManager>;
	public:
		//�G��|�������̃X�R�A
		void AddScore(int score);
		int GetScore() const;
		//�O�ɏ�����
		void ResetScore();

		//�G��|������
		void AddCount(CharacterType enemyType);
		int GetCount(CharacterType enemyType);
		void ResetCount();

		//���݂̃X�e�[�^�X���擾
		PlayerStatus GetCurrentPlayerStatus();
		BossEnemyStatus GetCurrentBossStatus();
		EnemyStatus GetCurrentEnemyStatus(const std::string enemyTypeName);
		std::shared_ptr<WeaponStatus> GetCurrentWeaponStatus(const std::string weaponTypeName);

		//�X�e�[�^�X���w�肵�����x���Ń��Z�b�g
		PlayerStatus ResetPlayerStatus(const int level);
		//�{�X�̃X�e�[�^�X�����Z�b�g
		BossEnemyStatus ResetBossStatus();
		//�w�肵���G�̃J�e�S���[���Ŏw�背�x���̃X�e�[�^�X�Ƀ��Z�b�g
		EnemyStatus ResetEnemyStatus(const int level,const std::string& characterTypeName);
		//����̃X�e�[�^�X�𖢋����O�̏�ԂɃ��Z�b�g����
		std::shared_ptr<WeaponStatus> ResetWeaponStatus(const std::string& weaponType);
		//���I������V�ꗗ���X�^�[�g���̏�Ԃɖ߂�
		void ResetRewardS();
		//�������Ԃɂ��{�[�i�X�o���l�����ʂ����Z�b�g
		void ResetSurvivalBonus();
		//�h���b�v�A�C�e�����w�肵���G�ƃ��x���ɑΉ������X�e�[�^�X�Ŏ擾
		float GetExpLevelUpItem(const int level,const std::string charaType);
		float GetHealValueItem(const int level,const std::string charaType);

		//�v���C���[�̃��x���A�b�v�̌o���l�֌W
		int GetNeedExp(int level);
		int GetPlayerCurrentLevel()const;
		//�v���C���[�̍ő僌�x��
		int GetPlayerMaxLevel()const;
		//�G�̍ő僌�x��
		int GetEnemyMaxLevel(const std::string typeName);
		//���I�őI�΂���V�̍ő吔�擾
		int GetRewardMaxCount()const;
		//���݂̐������Ԃɂ��{�[�i�X�o���l�����ʎ擾
		float GetSurvivalBonus()const;
		//���݂̑S�Ă̓G�̃X�e�[�^�X�̎擾
		std::vector<EnemyStatus>GetAllCurrentEnemyStatus();
		//���݃A�N�e�B�u��Ԃ̓G�̐�
		int GetEnemyCount();
		//���x���A�b�v���̕�V�ꗗ���痐����(3~5)�I��ŕԂ�
		std::vector<std::shared_ptr<LevelUpBonus>>GetRandomSelectRewards();
		//���j���ēG�̃h���b�v�A�C�e���ꗗ���璊�I���ĕԂ�
		std::vector<DropItem>GetDropItems(const std::string charaTypeName);

		//���݂̃X�e�[�^�X���X�V
		void UpdatePlayerStatus(PlayerStatus newStatus);
		EnemyStatus UpdateEnemyStatus(const std::string enemyTypeName);
		void UpdateWeaponStatus(const std::string weaponName,std::shared_ptr<WeaponStatus> newStatus);
		void UpdateEnemyWeaponStatus(const std::string weaponName,const std::string enemyTypeName);
		void UpdateRewards(const std::string tag);
		void RemoveReward(const std::string name);
		void UpdateSurvivalBonus();

		//���݃X�|�[�����Ă���{�X�̖��O�擾
		std::string GetCurrentBossName();

		// �R�s�[�R���X�g���N�^�Ƒ�����Z�q���폜
		StatusManager(const StatusManager&) = delete;
		StatusManager& operator=(const StatusManager&) = delete;

private:
	StatusManager();
	~StatusManager();
	//�o���l�e�[�u��
	std::vector<int>mPlayerExpTable;
	//���I������V�̍ő吔
	const int mRewardMaxCount = 5;

	//���݂̃v���C���[�̌��j�X�R�A
	int mPlayerScore = 0;
	//�G�̌��j�J�E���g
	int normalEnemyEleminatedCount = 0;
	int bossEnemyEleminatedCount = 0;
	//�v���C���[�������Ԃɂ��o���l������
	float mSurvivalBonus = 1;

	//�w�肵�����x���̓G�̃X�e�[�^�X�v�Z
	EnemyStatus calculateStatus(const int level,EnemyStatusDict& status);
	//���I�őI�ԕ�V�̐��𗐐��ŕԂ�(3~5��)
	int weightedRandom();
	
	//�Q�ƃX�e�[�^�X��b�l
	PlayerStatusDict mPlayerStatusDict;
	std::unordered_map<std::string, EnemyStatusDict> mEnemyStatusDict;
	BossEnemyStatusDict mBossStatusDict;
	std::unordered_map<std::string, LevelUpItemStatusDict> mLevelUpItemStatusDict;
	std::unordered_map<std::string, HealItemStatusDict> mHealItemStatusDict;
	std::vector<std::shared_ptr<LevelUpBonus>>mRewardDict;

	//���݂̃X�e�[�^�X���擾
	PlayerStatus mCurrentPlayerStatus;
	std::unordered_map<std::string, EnemyStatus> mCurrentEnemyStatuses;
	std::unordered_map<std::string, std::shared_ptr<WeaponStatus>>mCurrentWeaponStatuses;
	std::vector<std::shared_ptr<LevelUpBonus>>mCurrentRewards;
	BossEnemyStatus mCurrentBossStatus;
};

#endif // STATUSMANAGER_H