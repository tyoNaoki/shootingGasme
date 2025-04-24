#include "LevelUpItem.h"
#include "StatusManager.h"
#include "GraphicManager.h"
#include "ActorManager.h"

LevelUpItem::LevelUpItem()
{
	//�����A�j���[�V�����ݒ�
	mStartAnimStateName = "Flight";
	//�`��֌W�̈ʒu�A�傫��������
	mDrawExtendSize = Vector2D<float>(2, 2);
	mDrawOffset = Vector2D<float>(-15, -15);
}

void LevelUpItem::Init(CharacterType ct, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
	//������
	ItemBase::Init(ct,typeName,id,localPosition,worldPosition,rotation,isVisible);
	mIsInitialize = true;
}

void LevelUpItem::Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
	//������
	ItemBase::Reset(id,localPosition,worldPosition,rotation,isVisible);
	mIsInitialize = true;
}

void LevelUpItem::Update(float deltaTime)
{
	//�X�V
	ItemBase::Update(deltaTime);
}

void LevelUpItem::SetExp(const int level,std::string dropOwnerTypeName)
{
	//�h���b�v�����G�̃��x���ɑΉ������o���l���v�Z�A�ݒ肷��
	mExp = STATUS.GetExpLevelUpItem(level,dropOwnerTypeName);
}

bool LevelUpItem::IsInitialize()
{
	return mIsInitialize;
}

void LevelUpItem::GetItemBonus() const
{
	//�v���C���[�Ɍo���l��t�^
	ACTOR_M.PlayerTakeExp(mExp);
}
