#include "LevelUpItem.h"
#include "StatusManager.h"
#include "GraphicManager.h"
#include "ActorManager.h"

LevelUpItem::LevelUpItem()
{
	//初期アニメーション設定
	mStartAnimStateName = "Flight";
	//描画関係の位置、大きさ微調整
	mDrawExtendSize = Vector2D<float>(2, 2);
	mDrawOffset = Vector2D<float>(-15, -15);
}

void LevelUpItem::Init(CharacterType ct, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
	//初期化
	ItemBase::Init(ct,typeName,id,localPosition,worldPosition,rotation,isVisible);
	mIsInitialize = true;
}

void LevelUpItem::Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
	//初期化
	ItemBase::Reset(id,localPosition,worldPosition,rotation,isVisible);
	mIsInitialize = true;
}

void LevelUpItem::Update(float deltaTime)
{
	//更新
	ItemBase::Update(deltaTime);
}

void LevelUpItem::SetExp(const int level,std::string dropOwnerTypeName)
{
	//ドロップした敵のレベルに対応した経験値を計算、設定する
	mExp = STATUS.GetExpLevelUpItem(level,dropOwnerTypeName);
}

bool LevelUpItem::IsInitialize()
{
	return mIsInitialize;
}

void LevelUpItem::GetItemBonus() const
{
	//プレイヤーに経験値を付与
	ACTOR_M.PlayerTakeExp(mExp);
}
