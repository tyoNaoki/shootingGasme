#include "NormalEnemyMeleeComponent.h"
#include "DebugHelper.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "StatusManager.h"
#include "CharacterBase.h"
#include "CollisionManager.h"
#include "GraphicManager.h"
#include "ActorManager.h"
#include "Collision.h"
#include "PlayerCharacter.h"
#include "WeaponStatus.h"

<<<<<<< HEAD
NormalEnemyMeleeComponent::NormalEnemyMeleeComponent(std::shared_ptr<CharacterBase> owner):WeaponComponent(owner)
=======
NormalEnemyMeleeComponent::NormalEnemyMeleeComponent(std::shared_ptr<CharacterBase> owner):Component(owner)
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
{
	//������
	mComponentName = "NormalEnemyMeleeComponent";
	Init();
	Vector2D<float>size;
}

void NormalEnemyMeleeComponent::Init()
{
	//�X�e�[�^�X�ݒ�
	SetStatus(STATUS.GetCurrentWeaponStatus("NormalEnemyMeleeComponent"));
	currentTime = meleeSpeed;
}

void NormalEnemyMeleeComponent::Update(const float deltaTime)
{
	auto owner = GetOwner();
	auto player = ACTOR_M.GetCurrentPlayer();
	if (!owner->IsActive()||!player||!player->IsActive()) { return; }

	//�N�[���^�C�����I����Ă��邩
	if (currentTime > 0) {
		currentTime -= deltaTime;
		return;
	}

	//�I�[�i�[�̊p�x����ߐڔ͈͂��擾
	float meleeAngle = owner->GetRotation();;
	Collision::RotatedRect attackRange = Collision::CalculateAttackRange(owner->GetWorldPosition2D(), meleeWidthRange, meleeHeightRange, meleeAngle);

	//����炷
	SOUND_M.PlaySoundByID("testSound");
	bool isHit = false;

	//�ߐڍU���̏Փ˔���
<<<<<<< HEAD
	auto collision = player->GetCollision<Collision::Rect>();
	if(Collision::IsColliding(attackRange,*collision)){
=======
	if(Collision::IsColliding(attackRange,player->GetCollision())){
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
		isHit =true;
		//�v���C���[�܂ł̊p�x���m�b�N�o�b�N�p�x�ɐݒ�
		float targetRadian = Vector2D<float>::GetLookAtRadian(owner->GetWorldPosition2D(), player->GetWorldPosition2D());
		//�_���[�W����
		player->TakeDamage(attack, Vector2D<float>(cos(targetRadian), sin(targetRadian)), meleeShock, 0.2f);
	}
	//�f�o�b�O�R���W�����\��
	DEBUG_HELPER.DrawCollision(attackRange, 0.2f, isHit);
	//�ߐڃG�t�F�N�g�`��
	DrawMelee(owner, attackRange);
	//�N�[���^�C���ݒ�
	currentTime = meleeSpeed;
	return;
}

void NormalEnemyMeleeComponent::SetStatus(std::shared_ptr<WeaponStatus> status)
{
	//�X�e�[�^�X�ݒ�
	auto meleeStatus = std::dynamic_pointer_cast<MeleeWeaponStatus>(status);
	attack = meleeStatus->mAttack;
	meleeSpeed = meleeStatus->mSwingSpeed;
	meleeWidthRange = meleeStatus->mWidthRange;
	meleeHeightRange = meleeStatus->mHeightRange;
}

void NormalEnemyMeleeComponent::DrawMelee(std::shared_ptr<CharacterBase> owner, Collision::RotatedRect& rotRect)
{
	//�`��̈ʒu������
	float angle = rotRect.angle;
	auto vertices = rotRect.GetVertices();
	auto offsetYvec = (vertices[0] - vertices[3]) / meleeHeightRange;//y��
	auto offsetXvec = (vertices[0] - vertices[1]) / meleeWidthRange;//x��
	auto start = (vertices[0] - vertices[3]) / 2 + vertices[0];

	//�摜�𔽓]�����邩
	bool reverseYFlag = owner->IsReverse() ? false : true;
	//���]���̈ʒu������
	if (reverseYFlag) {
		start += offsetYvec * -190.0f;
		start += offsetXvec * -100;
	}else {
		start += offsetYvec * -190.0f;
		start += offsetXvec * -100;
	}

	
	//�ߐڃG�t�F�N�g�`��
	for(int i = 0;i<3;i++){
		if (reverseYFlag) {
			start += offsetYvec * 0.0f;
			start += offsetXvec * (-5 + i*22);
		}
		else {
			start += offsetYvec * 0.0f;
			start += offsetXvec * (-5+i*22);
		}

		auto center = Vector2D<int>(100 / 2, 55 / 2);
		EFFECT_M.AddEffectRota3(mActionName, start, center, Vector2D<float>(2.5f, 2.5f), angle, true, false, reverseYFlag, drawTime);
	}
}
