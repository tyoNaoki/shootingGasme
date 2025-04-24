#include "Actor.h"
#include "GraphicManager.h"

Actor::Actor()
{
    mTypeName = "EMPTY";
    mCharaType = CharacterType::EMPTY;
    mId = -1;
    mWorldPosition = Vector2D<float>();
    mWorldOldPosition = Vector2D<float>();
    mLocalPosition = Vector2D<float>();
    mVelocity = Vector2D<float>();
    mRotation = 0.0f;
    mRotation_vel = 0.0f;

    mVisible = false;
}

void Actor::Initialize(CharacterType ct,std::string typeName,int id, Vector2D<float> localPosition,Vector2D<float>worldPosition,float rot,bool visible)
{
    mTypeName = typeName;
    mCharaType = ct;
    mId = id;
    mWorldPosition = worldPosition;
    mWorldOldPosition = worldPosition;
    mLocalPosition = localPosition;
    mVelocity = Vector2D<float>(0,0);
    mRotation = rot;
    mRotation_vel = 0.0f;
    mVisible = visible;
}

void Actor::SetLocalPosition2D(Vector2D<float> newPosition)
{
    mLocalPosition = newPosition;
}

void Actor::SetWorldPosition2D(Vector2D<float> worldPosition)
{
    mWorldPosition = worldPosition;
}

void Actor::SetOldWorldPosition2D(Vector2D<float> worldPosition)
{
    mWorldOldPosition = worldPosition;
}

<<<<<<< HEAD
void Actor::SetCollision(std::shared_ptr<Collision::Shape> collision)
{
    mActorCollision = collision;
}


=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
void Actor::SetGridIndex(int index)
{
    mGridIndex = index;
}

int Actor::GetGridIndex()
{
    return mGridIndex;
}

Vector2D<float> Actor::GetWorldFromLocalPosi(Vector2D<float> localPosition) const
{
    auto scene = SCENE_M.GetCurrentScene();
    if(scene){
        return scene->GetMap()->GetCellSize() * localPosition;
    }
    return Vector2D<float>();
}

Vector2D<float> Actor::GetLocalFromWorldPosi(Vector2D<float> worldPosition) const
{
    auto scene = SCENE_M.GetCurrentScene();
    if (scene) {
        return scene->GetMap()->GetLocalFromWorldPosition(worldPosition);
    }

    return Vector2D<float>();
}

CharacterType Actor::GetActorType() const
{
    return mCharaType;
}

std::string Actor::GetName() const
{
    return mTypeName + std::to_string(mId);
}

std::string Actor::GetTypeName() const
{
    return mTypeName;
}

int Actor::GetId() const
{
    return mId;
}

void Actor::SetVelocity(Vector2D<float> vel)
{
    mVelocity = vel;
}

void Actor::ApplyVelocity()
{
    mWorldPosition += mVelocity;
}

void Actor::SetRotation(float rot)
{
    mRotation = rot;
}

void Actor::SetRatationVelocity(float rot_Vel)
{
    mRotation_vel = rot_Vel;
}


Vector2D<float> Actor::GetWorldPosition2D() const
{
    return mWorldPosition;
}
Vector2D<float> Actor::GetOldWorldPosition2D() const
{
    return mWorldOldPosition;
}
int Actor::GetGridIndex() const
{
    return mGridIndex;
}
Vector2D<float> Actor::GetLocalPosition2D() const
{
    return mLocalPosition;
}


Vector2D<float> Actor::GetVelocity() const
{
    return mVelocity;
}

float Actor::GetRotation() const
{
    return mRotation;
}

float Actor::GetDrawRotation() const
{
    float rot = (mRotation >= 0) ? mRotation : (mRotation + 360);
    return fmod(rot,360.0f);
}

std::shared_ptr<AnimationState> Actor::GetCurrentAnimState() const
{
    return mCurrentAnimState;
}

bool Actor::HasAnimationState(std::string actionName)
{
    return GRAPHIC_M.IsReadAction(GetTypeName(),actionName);
}

void Actor::SetAnimationState(std::shared_ptr<AnimationState> animState)
{
    mCurrentAnimState = animState;
}

int Actor::GetCurrentAnimFrame()const
{
    return mCurrent_AnimFrame;
}

float Actor::GetCurrentAnimFrameAdjust()const
{
    return mCurrent_AnimFrame_adjust;
}

void Actor::StartDeadAnimation()
{
    mVisible = false;
    SetActive(false);
}

void Actor::FinishDeadAnimation()
{
    mVisible = false;
    SetActive(false);
}

bool Actor::IsReverse() const
{
    return mReverse;
}

bool Actor::IsMoving() const
{
    return mIsMove;
}

bool Actor::IsActive() const
{
    return mActive;
}
<<<<<<< HEAD

=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
void Actor::SetActive(bool active)
{
    mActive = active;
}

<<<<<<< HEAD
ComponentManager& Actor::GetComponentManager()
{
    return mComponentManager;
}

=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
