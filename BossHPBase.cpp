#include "BossHPBase.h"

void BossHPBase::Update(float deltaTime)
{
	//最大HP、現在のＨＰをHPUIに設定
	auto owner = GetOwner();
	SetMaxHP(owner->GetMaxHP());
	SetHP(owner->GetHP());
}

void BossHPBase::Draw(float deltaTime)
{
	if (!IsVisible()) { return; }

	//現在のＨＰの割合を計算
	float hpRatio = mHP / mMaxHP;

	//色は赤
	int color = UI::red;

	//現在のHPを四角で描画
	DrawBox(mLocalPosition.x, mLocalPosition.y, mLocalPosition.x + static_cast<int>(mWidth * hpRatio), mLocalPosition.y + mHeight, color, TRUE);
	DrawBox(mLocalPosition.x, mLocalPosition.y, mLocalPosition.x + mWidth, mLocalPosition.y + mHeight, GetColor(0, 255, 0), FALSE);
}

void BossHPBase::SetHP(float hp)
{
	mHP = hp > 0 ? hp : 0.0f;
}

void BossHPBase::SetMaxHP(float maxHp)
{
	mMaxHP = maxHp;
}

void BossHPBase::SetSize(int width, int height)
{
	mWidth = width;
	mHeight = height;
}

std::shared_ptr<CharacterBase> BossHPBase::GetOwner()
{
    return mOwner.lock();
}
