#include "HealItem.h"
#include "StatusManager.h"
#include "GraphicManager.h"
#include "ActorManager.h"

HealItem::HealItem()
{
	mStartAnimStateName = "Flight";
	mDrawExtendSize = Vector2D<float>(2,2);
	mDrawOffset = Vector2D<float>(-15,-15);
}

void HealItem::Init(CharacterType ct, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
	//èâä˙âª
	ItemBase::Init(ct, typeName, id, localPosition, worldPosition, rotation, isVisible);
	mIsInitialize = true;
}

void HealItem::Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
	//èâä˙âª
	ItemBase::Reset(id, localPosition, worldPosition, rotation, isVisible);
	mIsInitialize = true;
}

void HealItem::Update(float deltaTime)
{
	ItemBase::Update(deltaTime);
}

void HealItem::SetHeal(const int level, std::string dropOwnerTypeName)
{
	mHeal = STATUS.GetHealValueItem(level, dropOwnerTypeName);
}

bool HealItem::IsInitialize()
{
    return mIsInitialize;
}

void HealItem::GetItemBonus() const
{
	ACTOR_M.PlayerTakeHeal(mHeal);
}
