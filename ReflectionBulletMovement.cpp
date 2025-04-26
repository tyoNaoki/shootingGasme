#include "ReflectionBulletMovement.h"

void ReflectionBulletMovement::Init()
{
	
}

void ReflectionBulletMovement::Update(const float deltaTime)
{
    auto owner = mBulletOwner.lock();

    if(!owner||!owner->IsActive()) return;

    //�������Ԃ��O�b�ȉ�
    if (mLifeTime <= 0.0f) {
        owner->StartDeadAnimation();
        return;
    }

    //�p�x����i�s�x�N�g�����v�Z
    double rad = Vector2D<float>::GetRadiansFromDegrees(owner->GetRotation());
    auto unitVec = Vector2D<float>::GetUnitVectorByRadian(rad);
    //�ړ��ʂ��v�Z
    float speed = owner->GetMoveSpeed();
    auto moveVel = unitVec * speed * deltaTime;
    //���W�v�Z
    auto newPosi = moveVel + owner->GetWorldPosition2D();

    int width, height;
    GetWindowSize(&width, &height);

    auto localPosi = owner->GetLocalFromWorldPosi(newPosi);
    bool refrection = false;
    
    auto collision = owner->GetCollision<Collision::Circle>();
    //�v���C���[�̉�ʊO�̏ꍇ�A�ړ��x�N�g���𔽓]������
    if (localPosi.x < collision->mRadius || localPosi.x > width - collision->mRadius) {
        moveVel.x *= -1;
        refrection = true;
    }
    if (localPosi.y < collision->mRadius || localPosi.y > height - collision->mRadius) {
        moveVel.y *= -1;
        refrection = true;
    }

    //���˃t���O�������Ă���ꍇ
    if (refrection) {
        //���ˉ񐔂��ő�ɒB���Ă����ꍇ�A��������
        if (mReflectionMaxCount < mReflectionCurrentCount + 1) {
            std::static_pointer_cast<Bullet>(owner)->StartDeadAnimation();
            return;
        }
        //���݂̔��ˉ񐔂��L�^
        mReflectionCurrentCount++;
        //�p�x�A���W�Đݒ�
        owner->SetRotation(Vector2D<float>::GetLookAtAngle(moveVel));
        newPosi = moveVel + owner->GetWorldPosition2D();
    }

    owner->SetVelocity(moveVel);
    localPosi = SCENE_M.GetCurrentScene()->GetMap()->GetLocalFromWorldPosition(newPosi);
    //���W�X�V
    owner->SetLocalPosition2D(localPosi);
    owner->SetWorldPosition2D(newPosi);

    mLifeTime -= deltaTime;
}
