#include "PlayerThrowBombComponent.h"
#include "CollisionManager.h"
#include "StatusManager.h"
#include "ActorManager.h"
#include "SceneManager.h"
#include "WeaponStatus.h"

PlayerThrowBombComponent::PlayerThrowBombComponent(std::shared_ptr<CharacterBase> owner) :WeaponComponent(owner)
{
    //������
	mComponentName = "PlayerThrowBombComponent";
	Init();
}

void PlayerThrowBombComponent::Init()
{
    //�X�e�[�^�X�ݒ�
	SetStatus(STATUS.GetCurrentWeaponStatus(mComponentName));
}

void PlayerThrowBombComponent::Update(const float deltaTime)
{
	auto owner = GetWeaponOwner();

	if (!IsEnable() || !owner->IsActive() || owner->IsDead()) { return; }

    //�N�[���^�C�����I����Ă��邩
	if (mCurrentTime > 0) {
		mCurrentTime -= deltaTime;
		return;
	}
    //���݂̃}�b�v�擾
    auto map = SCENE_M.GetCurrentScene()->GetMap();
    //������ʒu�A�p�x�擾
    auto throwPosi = CalcRandomSlowPosition(owner,mThrowRange,map);
    float angle = Vector2D<float>::GetLookAtAngle(owner->GetWorldPosition2D(), throwPosi);

    //���e�X�|�[��
    auto bomb = ACTOR_M.GetBomb(mBombName.c_str(),CharacterType::PLAYER,map->GetLocalFromWorldPosition(throwPosi), throwPosi,angle,true);

    //���e���A�N�e�B�u�ɂ���
    if (!bomb) {
        DEBUG_HELPER.Add("Bomb is generate error", 1.0f);
        return;
    }

    //���e�̔����͈͂��v�Z
    auto size = mDefaultBombSize * (mExplosionRange / mDefaultExplosionRange);
    //�����̕`�摬�x�v�Z
    auto playSpeed = defaultPlaySpeed / (mExplosionTime / mDefaultExplosionTime);
    //�X�e�[�^�X�ݒ�
    bomb->SetStatus(mDefaultBombSize,size,playSpeed,mAttack, mKnockBackStrength, mTimeToExplode,mExplosionTime,mExplosionRange,mHasContinuousDamage);
    //����炷
    SOUND_M.PlaySoundByID(mSlowSound);

    //�N�[���^�C���ݒ�
    mCurrentTime = mThrowRate;
}

void PlayerThrowBombComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
    //�X�e�[�^�X�ݒ�
    auto bombStatus = std::dynamic_pointer_cast<BombWeaponStatus>(status);
    mAttack = bombStatus->mAttack;
    mThrowRate = bombStatus->mThrowRate.GetValue();
    mExplosionTime = bombStatus->mExplosionTime.GetValue();
    mTimeToExplode = bombStatus->mTimeToExplode.GetValue();
    mKnockBackStrength = bombStatus->mKnockBackStrength.GetValue();
    mThrowRange = bombStatus->mThrowRange;
    mExplosionRange = bombStatus->mExplosionRange.GetValue();
    mHasContinuousDamage = bombStatus->mHasContinuousDamage;

    //�����X�e�[�^�X�ݒ�
    mDefaultExplosionRange = bombStatus->mExplosionRange.GetDefaultValue();
    mDefaultExplosionTime = bombStatus->mExplosionTime.GetDefaultValue();
}

Vector2D<float> PlayerThrowBombComponent::CalcRandomSlowPosition(std::shared_ptr<CharacterBase>owner, float radius, std::shared_ptr<Map> map)
{
    int maxCount = 10;
    auto centerLocation = owner->GetWorldPosition2D();

    auto& dh = Singleton<DebugHelper>::get_instance();

    for (int count = 0; count < maxCount; count++) {

        // 0����2�΂܂ł̃����_���Ȋp�x�𐶐�
        double angle = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;

        // 0����radius�܂ł̃����_���ȋ����𐶐� 
        float distanceX = static_cast<float>(rand()) / RAND_MAX * radius + mThrowRangeMin;
        float distanceY = static_cast<float>(rand()) / RAND_MAX * radius + mThrowRangeMin;

        // �ɍ��W����f�J���g���W�ւ̕ϊ� 
        float x = centerLocation.x + static_cast<float>(distanceX * cos(angle));
        float y = centerLocation.y + static_cast<float>(distanceY * sin(angle));

        Vector2D<float>spawnPosition(x, y);
        
        if (map->GetTileOnWorldLocation(spawnPosition) == TileType::GROUND) {
            return spawnPosition;
        }
    }

    return Vector2D<float>();
}
