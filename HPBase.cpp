#include "HPBase.h"

void HPBase::Update(float deltaTime)
{
	auto owner = GetOwner();
	auto posi = owner->GetLocalPosition2D();
	SetLocalPosition(Vector2D<float>(posi.x-mWidth/2,posi.y+20));
	SetMaxHP(owner->GetMaxHP());
	SetHP(owner->GetHP());
}

void HPBase::Draw(float deltaTime)
{
	if (!IsVisible()) { return; }

	//HP‚ÌŠ„‡ŒvŽZ
	float hpRatio = mHP / mMaxHP;

	int color = UI::white;
	auto type = GetOwner()->GetActorType();
	if(type == CharacterType::PLAYER){
		color = GetColor(0, 255, 0);
		if (hpRatio <= 0.3) {
			color = GetColor(255, 0, 0);
		}
	}else{
		color = UI::red;
	}

	//HP‚ðŽlŠp‚Å•`‰æ
	DrawBox(mLocalPosition.x, mLocalPosition.y, mLocalPosition.x + static_cast<int>(mWidth * hpRatio), mLocalPosition.y + mHeight, color, TRUE);
	DrawBox(mLocalPosition.x, mLocalPosition.y, mLocalPosition.x + mWidth, mLocalPosition.y + mHeight, GetColor(0, 255, 0), FALSE);
}

void HPBase::SetHP(float hp)
{
	mHP = hp > 0 ? hp : 0.0f;
}

void HPBase::SetMaxHP(float maxHp)
{
	mMaxHP = maxHp;
}

void HPBase::SetSize(int width, int height)
{
	mWidth = width;
	mHeight = height;
}

std::shared_ptr<CharacterBase> HPBase::GetOwner()
{
	return mOwner.lock();
}
