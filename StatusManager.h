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
	//ドロップ品の名前
	std::string itemName;
	//ドロップアイテム選択の抽選確率
	float dropWeight;
};

struct PlayerStatusDict {
	float hp = 0.0f;
	float moveSpeed = 0.0f;
	
	//経験値テーブル作成の為の調整パラメータ
	int maxLevel;
	//基礎経験値+基礎経験値^expWeight
	float baseExp;
	float expWeight;
};

struct PlayerStatus {
	int level = 0;
	float hp = 0.0f;
	float moveSpeed = 0.0f;
};

struct EnemyStatusDict {
	//敵の名前
	std::string name;
	float hp = 0.0f;
	float moveSpeed = 0.0f;
	float score = 0.0f;

	//レベルアップ時のHPの増加量
	float multiplyValueLevelUp = 2.0f;
	//レベルアップ時の攻撃力の増加
	float weaponPlusValueLevelUp = 10.0f;
	//レベルアップ時の撃破スコア量
	float scorePlusValueLevelUp = 2.0f;
	//撃破されたときのドロップアイテム
	std::vector<DropItem>dropItems;
	//最大レベル
	int maxLevel = 0;
};

struct EnemyStatus {
	//敵の名前
	std::string name;
	int level = 0;
	float hp = 0.0f;
	float moveSpeed = 0.0f;
	//撃破されたときのスコア
	float score = 0.0f;
};

struct BossEnemyStatusDict {
	//ボスの名前
	std::string name;
	float hp = 0.0f;
	float moveSpeed = 0.0f;
	//撃破されたときのスコア
	float score = 0.0f;
};

struct BossEnemyStatus{
	//ボスの名前
	std::string name;

	float hp = 0.0f;
	float moveSpeed = 0.0f;
	//撃破されたときのスコア
	float score = 0.0f;
};
//ドロップアイテムの経験値
struct LevelUpItemStatusDict {
	//初期経験値量
	float baseTakeExp;
	//撃破した敵のレベル分、経験値量が足し算で増加
	float plusValueLevelUp;
};

struct HealItemStatusDict {
	//初期回復量
	float baseHealValue;
	//撃破した敵のレベル分、回復量が足し算で増加
	float plusValHealValueLevelUp;
};

struct PlayResult {
	//プレイヤーのレベル表記
	float level;
	//プレイヤーの敵撃破の合計スコア
	float score;
	//雑魚敵の撃破数
	int normalEnemyEliminatedCount = 0;
	//ボスの撃破数
	int bossEliminatedCount = 0;
};

class StatusManager
{
friend class Singleton<StatusManager>;
	public:
		//敵を倒した時のスコア
		void AddScore(int score);
		int GetScore() const;
		//０に初期化
		void ResetScore();

		//敵を倒した数
		void AddCount(CharacterType enemyType);
		int GetCount(CharacterType enemyType);
		void ResetCount();

		//現在のステータスを取得
		PlayerStatus GetCurrentPlayerStatus();
		BossEnemyStatus GetCurrentBossStatus();
		EnemyStatus GetCurrentEnemyStatus(const std::string enemyTypeName);
		std::shared_ptr<WeaponStatus> GetCurrentWeaponStatus(const std::string weaponTypeName);

		//ステータスを指定したレベルでリセット
		PlayerStatus ResetPlayerStatus(const int level);
		//ボスのステータスをリセット
		BossEnemyStatus ResetBossStatus();
		//指定した敵のカテゴリー毎で指定レベルのステータスにリセット
		EnemyStatus ResetEnemyStatus(const int level,const std::string& characterTypeName);
		//武器のステータスを未強化前の状態にリセットする
		std::shared_ptr<WeaponStatus> ResetWeaponStatus(const std::string& weaponType);
		//抽選される報酬一覧をスタート時の状態に戻す
		void ResetRewardS();
		//生存時間によるボーナス経験値増加量をリセット
		void ResetSurvivalBonus();
		//ドロップアイテムを指定した敵とレベルに対応したステータスで取得
		float GetExpLevelUpItem(const int level,const std::string charaType);
		float GetHealValueItem(const int level,const std::string charaType);

		//プレイヤーのレベルアップの経験値関係
		int GetNeedExp(int level);
		int GetPlayerCurrentLevel()const;
		//プレイヤーの最大レベル
		int GetPlayerMaxLevel()const;
		//敵の最大レベル
		int GetEnemyMaxLevel(const std::string typeName);
		//抽選で選ばれる報酬の最大数取得
		int GetRewardMaxCount()const;
		//現在の生存時間によるボーナス経験値増加量取得
		float GetSurvivalBonus()const;
		//現在の全ての敵のステータスの取得
		std::vector<EnemyStatus>GetAllCurrentEnemyStatus();
		//現在アクティブ状態の敵の数
		int GetEnemyCount();
		//レベルアップ時の報酬一覧から乱数で(3~5)個選んで返す
		std::vector<std::shared_ptr<LevelUpBonus>>GetRandomSelectRewards();
		//撃破して敵のドロップアイテム一覧から抽選して返す
		std::vector<DropItem>GetDropItems(const std::string charaTypeName);

		//現在のステータスを更新
		void UpdatePlayerStatus(PlayerStatus newStatus);
		EnemyStatus UpdateEnemyStatus(const std::string enemyTypeName);
		void UpdateWeaponStatus(const std::string weaponName,std::shared_ptr<WeaponStatus> newStatus);
		void UpdateEnemyWeaponStatus(const std::string weaponName,const std::string enemyTypeName);
		void UpdateRewards(const std::string tag);
		void RemoveReward(const std::string name);
		void UpdateSurvivalBonus();

		//現在スポーンしているボスの名前取得
		std::string GetCurrentBossName();

		// コピーコンストラクタと代入演算子を削除
		StatusManager(const StatusManager&) = delete;
		StatusManager& operator=(const StatusManager&) = delete;

private:
	StatusManager();
	~StatusManager();
	//経験値テーブル
	std::vector<int>mPlayerExpTable;
	//抽選される報酬の最大数
	const int mRewardMaxCount = 5;

	//現在のプレイヤーの撃破スコア
	int mPlayerScore = 0;
	//敵の撃破カウント
	int normalEnemyEleminatedCount = 0;
	int bossEnemyEleminatedCount = 0;
	//プレイヤー生存時間による経験値増加量
	float mSurvivalBonus = 1;

	//指定したレベルの敵のステータス計算
	EnemyStatus calculateStatus(const int level,EnemyStatusDict& status);
	//抽選で選ぶ報酬の数を乱数で返す(3~5個)
	int weightedRandom();
	
	//参照ステータス基礎値
	PlayerStatusDict mPlayerStatusDict;
	std::unordered_map<std::string, EnemyStatusDict> mEnemyStatusDict;
	BossEnemyStatusDict mBossStatusDict;
	std::unordered_map<std::string, LevelUpItemStatusDict> mLevelUpItemStatusDict;
	std::unordered_map<std::string, HealItemStatusDict> mHealItemStatusDict;
	std::vector<std::shared_ptr<LevelUpBonus>>mRewardDict;

	//現在のステータスを取得
	PlayerStatus mCurrentPlayerStatus;
	std::unordered_map<std::string, EnemyStatus> mCurrentEnemyStatuses;
	std::unordered_map<std::string, std::shared_ptr<WeaponStatus>>mCurrentWeaponStatuses;
	std::vector<std::shared_ptr<LevelUpBonus>>mCurrentRewards;
	BossEnemyStatus mCurrentBossStatus;
};

#endif // STATUSMANAGER_H