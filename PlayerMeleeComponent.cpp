#include "PlayerMeleeComponent.h"
#include "PlayerCharacter.h"
#include "ActorManager.h"
#include "DebugHelper.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include <vector>
#include "GraphicManager.h"
#include "StatusManager.h"
#include "CharacterBase.h"
#include "WeaponStatus.h"

<<<<<<< HEAD
PlayerMeleeComponent::PlayerMeleeComponent(std::shared_ptr<CharacterBase> owner) :WeaponComponent(owner)
=======
PlayerMeleeComponent::PlayerMeleeComponent(std::shared_ptr<CharacterBase> owner) :Component(owner)
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
{
	//������
	mComponentName = "PlayerMeleeComponent";
	Init();
}

PlayerMeleeComponent::~PlayerMeleeComponent()
{
}

void PlayerMeleeComponent::Init()
{
	//�X�e�[�^�X�ݒ�
	SetStatus(STATUS.GetCurrentWeaponStatus("PlayerMeleeComponent"));
	mIsWidth = false;
}

void PlayerMeleeComponent::Update(const float deltaTime)
{
	auto owner = GetOwner();

	if (!IsEnable() || !owner->IsActive() || owner->IsDead()) { return; }
	//�N�[���^�C�����I����Ă��邩
	if(currentTime>0){
		currentTime-=deltaTime;
		return;
	}

	DEBUG_HELPER.Add("Player Attack damage " + std::to_string(attack), meleeSpeed);
	//�I�[�i�[�̊p�x�擾
	float ownerRot = owner->GetRotation();
	//�Փ˔��菜�O�ɃI�[�i�[��ǉ�
	std::vector<std::weak_ptr<CharacterBase>>charas{ owner };
	//���͂̓G�擾
	auto nearEnemies = COLLISION_M.GetNearCharacters(owner->GetWorldPosition2D(), meleeRange, charas);
	//�p�x����ߐڔ͈͂��v�Z
	float meleeAngle = ownerRot;
	Collision::RotatedRect attackRange = Collision::CalculateAttackRange(owner->GetWorldPosition2D(), meleeWidthRange, meleeHeightRange, meleeAngle);

	//���͂ɓG������ꍇ
	if (nearEnemies.size() != 0 || ACTOR_M.IsSpawnBoss()) {

		//����炷
		SOUND_M.PlaySoundByID(meleeSound);

		//�Փ˔���v�Z
		bool isHit = false;
		for (auto& x : nearEnemies) {
			//����
<<<<<<< HEAD
			if (Collision::IsColliding(attackRange,*x->GetCollision<Collision::Rect>())) {
=======
			if (Collision::IsColliding(attackRange, x->GetCollision())) {
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
				isHit = true;
				//�G�܂ł̊p�x���m�b�N�o�b�N�Ƃ��Ď擾
				float targetRadian = Vector2D<float>::GetLookAtRadian(owner->GetWorldPosition2D(), x->GetWorldPosition2D());
				//�_���[�W����
				x->TakeDamage(attack,Vector2D<float>(cos(targetRadian),sin(targetRadian)),meleeShock,0.2f);
			}
		}
		//�{�X�Ƃ̏Փ˔���
<<<<<<< HEAD
		if(ACTOR_M.IsSpawnBoss()&&Collision::IsColliding(attackRange,*ACTOR_M.GetCurrentBossEnemy()->GetCollision<Collision::Rect>())){
=======
		if(ACTOR_M.IsSpawnBoss()&&Collision::IsColliding(attackRange,ACTOR_M.GetCurrentBossEnemy()->GetCollision())){
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
			//�_���[�W����
			//�{�X�̓m�b�N�o�b�N����
			ACTOR_M.GetCurrentBossEnemy()->TakeDamage(attack);
		}
		//�ߐڃG�t�F�N�g�`��
		DEBUG_HELPER.DrawCollision(attackRange, 0.2f, isHit);
		DrawMelee(owner,attackRange);

		nearEnemies.clear();
		//�N�[���^�C���ݒ�
		currentTime = meleeSpeed;
		return;
	}else{ //���͂ɓG�����Ȃ��ꍇ�A�Փˌ��m����
		//����炷
		SOUND_M.PlaySoundByID(meleeSound);
		//�G�t�F�N�g�`��
		DEBUG_HELPER.DrawCollision(attackRange,0.2f,false);
		DrawMelee(owner,attackRange);
		//�N�[���^�C���ݒ�
		currentTime = meleeSpeed;
		return;
	}
}

void PlayerMeleeComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//�X�e�[�^�X�ݒ�
	auto meleeStatus = std::dynamic_pointer_cast<MeleeWeaponStatus>(status);
	attack = meleeStatus->mAttack;
	meleeSpeed = meleeStatus->mSwingSpeed;
	if(meleeWidthRange+1 < meleeStatus->mWidthRange){
		mIsWidth = true;
	}
	meleeWidthRange = meleeStatus->mWidthRange;
	meleeHeightRange = meleeStatus->mHeightRange;
}

void PlayerMeleeComponent::DrawMelee(std::shared_ptr<CharacterBase>owner,Collision::RotatedRect& rotRect)
{
	//�ʒu������
	float angle = rotRect.angle;
	auto vertices = rotRect.GetVertices();
	auto offsetYvec = (vertices[0] - vertices[3])/meleeHeightRange;//y��
	auto offsetXvec = (vertices[0] - vertices[1])/meleeWidthRange;//x��
	auto start = (vertices[0] - vertices[3]) / 2 + vertices[0];
	
	std::string text = owner->IsReverse() ? "true" : "false";
	DEBUG_HELPER.Add("player is reverse " + text, 3.0f);

	//���]�t���O
	bool reverseYFlag = owner->IsReverse() ? false : true;
	//widthMeleeVer
	//�`��ʒu�C��
	if(mIsWidth){
		if (reverseYFlag) {
			start += offsetYvec * -200.0f;
			start += offsetXvec * 100;
		}
		else {
			start += offsetYvec * -200.0f;
			start += offsetXvec * 100;
		}
	}else{
		if (reverseYFlag) {
			start += offsetYvec * -190.0f;
			start += offsetXvec * 30;
		}
		else {
			start += offsetYvec * -190.0f;
			start += offsetXvec * 30;
		}
	}

	auto center = Vector2D<int>(100 / 2, 55 / 2);
	//�`��
	if(mIsWidth){//�ߐڂ����C�h�ɋ�������Ă��鎞
		EFFECT_M.AddEffectRota3(mActionName,start,center,Vector2D<float>(1.0f,1.5f),angle,true,false,reverseYFlag,drawTime);
	}else{//�ʏ�ߐ�
		EFFECT_M.AddEffectRota3(mActionName, start, center, Vector2D<float>(1.0f, 0.3f), angle, true, false, reverseYFlag, drawTime);
	}
}
