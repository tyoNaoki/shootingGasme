#pragma once
#include <string>

//レベルアップ時の選択する報酬のカテゴリー
enum class RewardType {
	WEAPON_UP,
	STATUS_UP,
	WEAPON_GET
};

class LevelUpBonus
{
public:
	//対応した武器タグ、報酬カテゴリー、報酬選択時の表記される名前、説明を設定
	LevelUpBonus(std::string tag,RewardType type,std::string name,std::string description);

	//対応した武器タグを取得
	std::string GetTag() const;
	//報酬カテゴリーを取得
	RewardType GetType() const;
	//報酬選択時の表記される名前
	std::string GetName() const;
	//報酬選択時の表記される説明
	std::string GetDescription() const;
	//継承先の設定された報酬を付与する
	virtual void Apply()const = 0;
	//現在のプレイヤーのステータスを更新
	void UpdateCurrentPlayer();

private:
	//武器タグ
	std::string mTag;
	//報酬カテゴリー
	RewardType mType;
	//報酬選択時の表記される名前
	std::string mName;
	//報酬選択時の表記される説明
	std::string mDescription;
};

