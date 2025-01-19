#include "BossHPBase.h"

void BossHPBase::Update(float deltaTime)
{
	//Å‘åHPAŒ»İ‚Ì‚g‚o‚ğHPUI‚Éİ’è
	auto owner = GetOwner();
	SetMaxHP(owner->GetMaxHP());
	SetHP(owner->GetHP());
}

void BossHPBase::Draw(float deltaTime)
{
	if (!IsVisible()) { return; }

	//Œ»İ‚Ì‚g‚o‚ÌŠ„‡‚ğŒvZ
	float hpRatio = mHP / mMaxHP;

	//F‚ÍÔ
	int color = UI::red;

	//Œ»İ‚ÌHP‚ğlŠp‚Å•`‰æ
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
