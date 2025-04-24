#include "ItemBase.h"
#include "ActorManager.h"
#include "PlayerCharacter.h"
#include "DebugHelper.h"
#include "CollisionManager.h"
#include "GraphicManager.h"

ItemBase::ItemBase()
{
}

void ItemBase::Init(CharacterType ct, std::string typeName, int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
	if (id == -1) {
		return;
	}
	//������
	Initialize(ct, typeName, id, localPosition, worldPosition, rotation, isVisible);

	SetWorldPosition2D(worldPosition);
	//�A�j���[�V����������
	ANIM_M.InitializeStates(this, mStartAnimStateName);

	//�摜������
	Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,typeName, mStartAnimStateName);

	mCurrentLifeTime = mMaxLifeTime;
	mIsTaked = false;
}

void ItemBase::Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
	if (id == -1) {
		return;
	}
	//������
	Initialize(GetActorType(), GetTypeName(), id, localPosition, worldPosition, rotation, isVisible);

	//�R���W�������W�X�V
	SetWorldPosition2D(worldPosition);

	//�A�j���[�V����������
	ANIM_M.InitializeStates(this, mStartAnimStateName);

	Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,GetTypeName(), mStartAnimStateName);

	mCurrentLifeTime = mMaxLifeTime;
	mIsTaked = false;
}

void ItemBase::Update(const float deltaTime)
{
	//�v���C���[���A�N�e�B�u��
	if(auto player = Singleton<ActorManager>::get_instance().GetCurrentPlayer()){
		if(!player->IsActive()){return;}

		//�v���C���[����苗���ȓ��Ȃ�A�v���C���[�Ɍ������Đi��
		auto length = (player->GetWorldPosition2D() - GetWorldPosition2D()).Length_Square();
		if(length <= mChaseDistance){
			ChaseMovement(deltaTime, player);
			mIsChase = true;
			auto collision = player->GetCollision<Collision::Rect>();
			if(length <= collision->mSize.x && length <= collision->mSize.y && !IsTaked()){
				mIsTaked = true;
				GetItemBonus();

				SetActive(false);
			}
		}
	}

	//�ǐՏ�Ԃ���Ȃ���΁A�������Ԃ��v��
	if(!mIsChase){
		calcLifeTime(deltaTime);
	}
	
	//�������Ԃ�������΁A��A�N�e�B�u��
	if (!IsLife()) {
		SetActive(false);
	}

	//���W�X�V
	SetLocalPosition2D(GetLocalFromWorldPosi(GetWorldPosition2D()));

	//�A�j���[�V�����X�V
	ANIM_M.Update(this);

	//���݂̃A�j���[�V�����摜�X�V
	Singleton<GraphicManager>::get_instance().updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(),GetCurrentAnimState()->GetActionName());
}

void ItemBase::Draw(const float deltaTime)
{

    //�_�~�[����
    if (mVisible && mCurrent_gHandle == -2) {
		auto itemSize = Vector2D<float>(16, 16);
        auto localPosi = GetLocalPosition2D();
        int x1 = static_cast<int>(localPosi.x - itemSize.x);
        int y1 = static_cast<int>(localPosi.y - itemSize.y);
        int x2 = static_cast<int>(localPosi.x + itemSize.x);
        int y2 = static_cast<int>(localPosi.y + itemSize.y);

        DrawBox(x1, y1, x2, y2, GetColor(0, 0, 200), TRUE);
        //DEBUG_HELPER.DrawCollision(GetCollision());
        //DrawBox(x1,y1,x2,y2,GetColor(200,0,0),TRUE);
        return;
    }

	//�`��֌W
	auto localPosi = GetLocalPosition2D();
	float x = localPosi.x + mDrawOffset.x;
	float y = localPosi.y + mDrawOffset.y;
	float width = mDrawSize.x * mDrawExtendSize.x;
	float height = mDrawSize.y * mDrawExtendSize.y;
	Vector2D<float>leftTopDrawPosi = Vector2D<float>(x, y);
	Vector2D<float>rightDownDrawPosi = Vector2D<float>(x + width, y + height);

	if (mVisible && mCurrent_gHandle != -1) {
		DrawExtendGraph(static_cast<int>(leftTopDrawPosi.x), static_cast<int>(leftTopDrawPosi.y), static_cast<int>(rightDownDrawPosi.x), static_cast<int>(rightDownDrawPosi.y), mCurrent_gHandle, TRUE);
	}
}

void ItemBase::ApplyDrawOffset(Vector2D<float> offset)
{
	mDrawOffset = offset;
}

void ItemBase::ChaseMovement(float deltaTime,std::shared_ptr<Actor>target)
{
	float rad = (float)Vector2D<float>::GetLookAtRadian(GetWorldPosition2D(), target->GetWorldPosition2D());
	//�p�x�ݒ�
	SetRotation(Vector2D<float>::GetDegreesFromRadians(rad));

	Vector2D<float>newPosi = GetWorldPosition2D();

	//���݂̊p�x����i�s�x�N�g���擾
	auto unitVec = Vector2D<float>::GetUnitVectorByRadian(rad);
	float speed = GetMoveSpeed();
	//�ړ��ʌv�Z
	auto moveVel = unitVec * speed * deltaTime;

	newPosi = moveVel + GetWorldPosition2D();
	//���x�A���W�X�V
	SetVelocity(moveVel);
	SetWorldPosition2D(newPosi);
}

float ItemBase::GetMoveSpeed() const
{
	return mMoveSpeed;
}

bool ItemBase::IsTaked()
{
	return mIsTaked;
}

bool ItemBase::IsLife()
{
	return mCurrentLifeTime > 0.0f;
}

void ItemBase::calcLifeTime(float deltaTime)
{
	mCurrentLifeTime-=deltaTime;
}

