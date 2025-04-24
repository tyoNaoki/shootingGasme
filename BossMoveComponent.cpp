#include "BossMoveComponent.h"
#include "BossBase.h"
#include "ActorManager.h"
#include "SceneManager.h"
#include "PlayerCharacter.h"

BossMoveComponent::BossMoveComponent(std::shared_ptr<CharacterBase> owner) :Component(owner)
{
	mComponentName = "BossMoveComponent";
	mBossOwner = std::dynamic_pointer_cast<BossBase>(owner);
	mPlayer =ACTOR_M.GetCurrentPlayer();
	mMap = SCENE_M.GetCurrentScene()->GetMap();
};

void BossMoveComponent::Update(const float deltaTime)
{
	auto owner = mBossOwner.lock();

	if(!owner||!owner->IsActive()||owner->IsDead()){return;}

	if(!mPlayer||!mPlayer->IsActive()||mPlayer->IsDead()){return;}
	//�p�x�ݒ�
	owner->SetRotation(Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), mPlayer->GetWorldPosition2D()));

	//�����Ă��Ȃ��Ȃ�
	if(!mIsMove){
		//�ړI�n�������_���ɐݒ肵�āA�ړ��t���O�����Ă�
		mNextPosition = GetRandomMovePosition(100);
		mIsMove = true;
	}
	//�ړI�n�ɂ��ǂ蒅������
	if(IsArrived(owner)){
		//�ړ��I�����I�[�i�[�ɒʒm
		owner->FinishState();
		mIsMove = false;
		return;
	}

	Move(deltaTime, owner);
}

Vector2D<float> BossMoveComponent::GetRandomMovePosition(const float radius)
{
	int width, height;
	GetWindowSize(&width, &height);
	int maxCount = 10;

	auto centerLocation = mPlayer->GetWorldPosition2D();
    for (int count = 0; count < maxCount; count++) {

        // 0����2�΂܂ł̃����_���Ȋp�x�𐶐�
        float angle = static_cast<float>(static_cast<float>(rand()) / RAND_MAX * 2 * M_PI);

        // 0����radius�܂ł̃����_���ȋ����𐶐� 
        float distanceX = static_cast<float>(static_cast<float>(rand()) / RAND_MAX * radius + width * 0.3);
        float distanceY = static_cast<float>(static_cast<float>(rand()) / RAND_MAX * radius + height * 0.3);

        // �ɍ��W����f�J���g���W�ւ̕ϊ� 
        float x = centerLocation.x + static_cast<float>(distanceX * cos(angle));
        float y = centerLocation.y + static_cast<float>(distanceY * sin(angle));

        Vector2D<float>movePosition(x, y);

        if (mMap->GetTileOnWorldLocation(movePosition) == TileType::GROUND) {
            return movePosition;
        }
    }

    return Vector2D<float>();
}

void BossMoveComponent::Move(float deltaTime, std::shared_ptr<BossBase>owner)
{
	//���̍��W�ւ̕����擾
	float rad = (float)Vector2D<float>::GetLookAtRadian(owner->GetWorldPosition2D(), mNextPosition);

	Vector2D<float>newPosi = owner->GetWorldPosition2D();
	//���W�A������O���x�N�g�����擾
	auto unitVec = Vector2D<float>::GetUnitVectorByRadian(rad);
	
	float speed = 0.0f;
	float dis = (mNextPosition - newPosi).Length();
	if(dis<= speed * deltaTime){
		speed = dis;
	}else{
		speed = owner->GetMoveSpeed();
	}
	
	//�ړ��ʂ��v�Z
	auto moveVel = unitVec * speed * deltaTime;
	//���x�A���W�X�V
	newPosi = moveVel + owner->GetWorldPosition2D();
	owner->SetVelocity(moveVel);
	owner->SetWorldPosition2D(newPosi, true);
}

bool BossMoveComponent::IsArrived(std::shared_ptr<BossBase>owner)const
{
	//���ǂ蒅���Ă��邩
	auto posi = owner->GetWorldPosition2D();
	return((mNextPosition - posi).Length() <= mDistanceToArrived);
}
