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
	//初期化
	Initialize(ct, typeName, id, localPosition, worldPosition, rotation, isVisible);

	SetWorldPosition2D(worldPosition);
	//アニメーション初期化
	ANIM_M.InitializeStates(this, mStartAnimStateName);

	//画像初期化
	Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,typeName, mStartAnimStateName);

	mCurrentLifeTime = mMaxLifeTime;
	mIsTaked = false;
}

void ItemBase::Reset(int id, Vector2D<float> localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
	if (id == -1) {
		return;
	}
	//初期化
	Initialize(GetActorType(), GetTypeName(), id, localPosition, worldPosition, rotation, isVisible);

	//コリジョン座標更新
	SetWorldPosition2D(worldPosition);

	//アニメーション初期化
	ANIM_M.InitializeStates(this, mStartAnimStateName);

	Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,GetTypeName(), mStartAnimStateName);

	mCurrentLifeTime = mMaxLifeTime;
	mIsTaked = false;
}

void ItemBase::Update(const float deltaTime)
{
	//プレイヤーがアクティブか
	if(auto player = Singleton<ActorManager>::get_instance().GetCurrentPlayer()){
		if(!player->IsActive()){return;}

		//プレイヤーが一定距離以内なら、プレイヤーに向かって進む
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

	//追跡状態じゃなければ、生存時間を計測
	if(!mIsChase){
		calcLifeTime(deltaTime);
	}
	
	//生存時間がすぎれば、非アクティブに
	if (!IsLife()) {
		SetActive(false);
	}

	//座標更新
	SetLocalPosition2D(GetLocalFromWorldPosi(GetWorldPosition2D()));

	//アニメーション更新
	ANIM_M.Update(this);

	//現在のアニメーション画像更新
	Singleton<GraphicManager>::get_instance().updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(),GetCurrentAnimState()->GetActionName());
}

void ItemBase::Draw(const float deltaTime)
{

    //ダミー処理
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

	//描画関係
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
	//角度設定
	SetRotation(Vector2D<float>::GetDegreesFromRadians(rad));

	Vector2D<float>newPosi = GetWorldPosition2D();

	//現在の角度から進行ベクトル取得
	auto unitVec = Vector2D<float>::GetUnitVectorByRadian(rad);
	float speed = GetMoveSpeed();
	//移動量計算
	auto moveVel = unitVec * speed * deltaTime;

	newPosi = moveVel + GetWorldPosition2D();
	//速度、座標更新
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

