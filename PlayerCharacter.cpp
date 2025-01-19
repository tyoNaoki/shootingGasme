#include "PlayerCharacter.h"
#include "DxLib.h"
#include "InputManager.h"
#include "GraphicManager.h"
#include "PlayerMovementComponent.h"
#include "PlayerMeleeComponent.h"
#include "PlayerGunComponent.h"
#include "PlayerReflectionGunComponent.h"
#include "PlayerThrowBombComponent.h"
#include "BossBase.h"
#include "StatusManager.h"
#include "UIManager.h"
#include "HPBase.h"
#include "UIText.h"
#include "CollisionManager.h"

PlayerCharacter::PlayerCharacter(Collision::Rect rect):CharacterBase(rect)
{
    mDrawOffset = Vector2D<float>(-70,-45);
    mDrawExtendSize = Vector2D<float>(1.5,1.5);
}

void PlayerCharacter::Init(CharacterType ct,std::string typeName,int id,Vector2D<float>localPosition,Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    if(id == -1){
        return;
    }

    Initialize(ct,typeName,id,localPosition,worldPosition, rotation, isVisible);

    auto status = STATUS.GetCurrentPlayerStatus();
    mLevel = status.level;
    mHP = status.hp;
    mMaxHP = status.hp;

    mMoveSpeed = status.moveSpeed;

    //�R���W�������W�X�V
    SetWorldPosition2D(worldPosition);

    std::shared_ptr<PlayerCharacter> self = std::static_pointer_cast<PlayerCharacter>(shared_from_this());

    //�R���|�[�l���g�ǉ�
    AddPlayerComponents(self);

    float drawRot = GetDrawRotation();
    
    //�������ɂ�����
    if (drawRot > 90.0f && drawRot < 270.0f) {
        mReverse = true;
        //�p�x��90�x��270�x�ɋ߂����l�Ȃ�E����
    }else if (nearEqual(drawRot, 90.0f) || nearEqual(drawRot, 270.0f)) {
        mReverse = false;
    }else {
        //�E����
        mReverse = false;
    }

    InitKnockBack();

    ANIM_M.InitializeStates(this, mStartAnimStateName);
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust,mDrawSize,typeName, mStartAnimStateName);

    mInitialize = true;
}

void PlayerCharacter::Reset(int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible)
{
    if(id == -1){
        mInitialize = false;
        return;
    }
    Initialize(GetActorType(), GetTypeName(), id, localPosition,worldPosition, rotation, isVisible);

    auto status = STATUS.GetCurrentPlayerStatus();
    mLevel= status.level;
    mHP = status.hp;
    mMaxHP = status.hp;
    mMoveSpeed = status.moveSpeed;

    //�R���|�[�l���g�̃X�e�[�^�X���Z�b�g
    InitComponents();

    //���������Ƀ��Z�b�g
    ResetPlayerComponents();

    //�o���l���Z�b�g
    ResetExp();

    //�R���W�������W�X�V
    SetWorldPosition2D(worldPosition);

    float drawRot = GetDrawRotation();

    //�������ɂ�����
    if (drawRot > 90.0f && drawRot < 270.0f) {
        mReverse = true;
        //�p�x��90�x��270�x�ɋ߂����l�Ȃ�E����
    }
    else if (nearEqual(drawRot, 90.0f) || nearEqual(drawRot, 270.0f)) {
        mReverse = false;
    }
    else {
        //�E����
        mReverse = false;
    }

    InitKnockBack();

    ANIM_M.InitializeStates(this, mStartAnimStateName);
    Singleton<GraphicManager>::get_instance().initializeHandle(mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, mDrawSize,GetTypeName(), mStartAnimStateName);

    mIsDead = false;
    mInitialize = true;
}

void PlayerCharacter::Update(const float deltaTime)
{
    //�f�o�b�O
    auto& dh = Singleton<DebugHelper>::get_instance();
    auto scene = Singleton<SceneManager>::get_instance().GetCurrentScene();
    if (!scene) { return; }

    //����ł���
    if(mIsDead){
        SetLocalPosition2D(GetLocalFromWorldPosi(GetWorldPosition2D()));
        //����ł���ꍇ�A�摜�X�V�����s��
        ANIM_M.Update(this);
        GRAPHIC_M.updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(), GetCurrentAnimState()->GetActionName());
        return;
    }

    //�R���|�[�l���g�X�V
    CharacterBase::Update(deltaTime);

    //�����Ă��邩�ǂ���
    bool isMove = IsMoving();

    //���݂̕`��p�x�擾
    float currentRotation = GetDrawRotation();

    //���������ǂ���
    bool isReverse = false;

    //�A�j���[�V�����摜�^�C�v�X�V(�ʏ���s�A���]���s)
    if(isMove){
        if (nearEqual(currentRotation, 90.0f) || nearEqual(currentRotation, 270.0f)) {
            //�p�x��90�x��270�x�ɋ߂����l�Ȃ�A�����͂��̂܂܂̌����ŌŒ�
            isReverse = mReverse;
        }
        //�������ɂ�����
        else if(currentRotation > 90.0f && currentRotation < 270.0f) {
            isReverse = true;
            //�p�x��90�x��270�x�ɋ߂����l�Ȃ�����͂��̂܂܂ŕԂ�
        }else{
            //�E����
            isReverse = false;
        }
    //�~�܂��Ă��鎞
    }else if(!isMove){
        isReverse = mReverse;
    }

    mReverse = isReverse;

    //���x�擾
    Vector2D<float> cur_vel = GetVelocity();

    //�Փˌ��m//
    auto newPosi = GetWorldPosition2D();
    auto newCollisionPosi = GetCollision();

    //�}�b�v�Ƃ̓����蔻��Ǘ�
    auto map = scene->GetMap();
    auto& cm = Singleton<CollisionManager>::get_instance();

    //�}�b�v�O�ɂ����ꍇ�A���ɖ߂�
    if(cm.PushBackFromOut(newPosi,newCollisionPosi,map)){
        std::string message = "Player is out";
        dh.Add(message);
    }

    //�ÓI��Q���ƐڐG���Ă����ꍇ�A�����߂����������s
    bool isColliding = cm.PushBackFromWalls(newPosi, newCollisionPosi, map);

    //�{�X�Ƃ̓����蔻��
    auto boss = ACTOR_M.GetCurrentBossEnemy();
    if(ACTOR_M.IsSpawnBoss() && boss->IsActive() && !boss->IsDead()){
        if(!boss->IsState(BossState::Move)){
            if (Collision::IsColliding(newCollisionPosi, boss->GetCollision())) {
                auto rect2 = boss->GetCollision();
                Collision::PushBackRect(newCollisionPosi, rect2);
                newPosi = newCollisionPosi.mLeftTop + (newCollisionPosi.mSize / 2);
                isColliding = true;
            }
        }
    }

    //�f�o�b�O�p�R���W�����`��
    dh.DrawCollision(newCollisionPosi, 0.0f, isColliding);
    
    map->calcMapPosition(newPosi);
    
    Vector2D<float>localPosi = map->GetLocalFromWorldPosition(newPosi);

    //���W�X�V
    SetLocalPosition2D(localPosi);
    SetWorldPosition2D(newPosi);

    std::string preActionName = GetCurrentAnimState()->GetActionName();

    //�A�j���[�V�����J�ڍX�V
    ANIM_M.Update(this);

    //���݂̃A�j���[�V�����摜�X�V
    //�V�A�j���[�V�����J��
    if(preActionName != GetCurrentAnimState()->GetActionName()){
        GRAPHIC_M.TransitionToAction(mCurrent_gHandle,mCurrent_AnimFrame,mCurrent_AnimFrame_adjust,mDrawSize,GetTypeName(), GetCurrentAnimState()->GetActionName());
    }else{
        GRAPHIC_M.updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(), GetCurrentAnimState()->GetActionName());
    }
}

bool PlayerCharacter::IsInitialize()
{
    return mInitialize;
}

void PlayerCharacter::TakeDamage(float damage, Vector2D<float>direction, float strength, float knockBackTime)
{
    //����ł��邩
    if(IsDead()){
        return;
    }
    //�_���[�W
    mHP -= damage;

    //���S�A�j���[�V����
    if (mHP <= 0){
        mHP = 0;
        StartDeadAnimation();
        return;
    }

    //�m�b�N�o�b�N������ꍇ�A�J�n����
    if(strength > 0.0f){
        AddKnockBack(direction,strength,knockBackTime);
    }

    //�_���[�W�A�j���[�V����
    DamageAnimation();
}

void PlayerCharacter::StartLevelUp(int levelUpCount)
{
    //���x���A�b�v�̕�V�I���J�n
    SCENE_M.OnLevelUpEvent(levelUpCount);
}

void PlayerCharacter::LevelUp()
{
    //�X�e�[�^�X�ݒ�
    auto status = STATUS.GetCurrentPlayerStatus();
    mLevel = status.level;
    mHP = status.hp;
    mMaxHP = status.hp;
    mMoveSpeed = status.moveSpeed;
}

void PlayerCharacter::WeaponLevelUp(const std::string componentName)
{
    //����X�e�[�^�X�X�V
    auto comp = GetComponent(componentName);
    if(comp){
        comp->SetStatus(STATUS.GetCurrentWeaponStatus(componentName));
    }
}

void PlayerCharacter::ResetExp()
{
    //�o���l������
    mAllExp = 0.0f;
    mCurrentExp = 0.0f;
}

void PlayerCharacter::TakeExp(float exp)
{
    //���x�����ő僌�x���ɒB���Ă��邩
    if(mLevel >= STATUS.GetPlayerMaxLevel()){
        return;
    }
    
    //�o���l�擾
    mAllExp += exp;
    mCurrentExp += exp;
    
    // ���x���A�b�v����
    bool isLevelUp = false;
    int levelUpCount = 0;
    //�K�v�Ȍo���l�𒴂������������x���A�b�v�������s��
    while (mCurrentExp >= STATUS.GetNeedExp(mLevel + levelUpCount)) {
        isLevelUp = true;
        //���݂̎擾�o���l����K�v�Ȍo���l������
        mCurrentExp -= STATUS.GetNeedExp(mLevel + levelUpCount);
        //���x���A�b�v�̉񐔋L�^
        levelUpCount++;
        //���x�����ő�ɂȂ�����A�擾�o���l��0�ɂ���
        if (mLevel+levelUpCount >= STATUS.GetPlayerMaxLevel()) {
            mCurrentExp = 0.0f;
            break;
        }
    }
    //���x���A�b�v�t���O���o���Ă���Ȃ�A���x���A�b�v�C�x���g�J�n
    if(isLevelUp){
        StartLevelUp(levelUpCount);
    }
}

void PlayerCharacter::TakeHeal(float heal)
{
    //�̗͂����łɍő�Ȃ�X�L�b�v
    if(GetHP()==GetMaxHP()){
        return;
    }
    //�ő�̗͂𒴂��Ȃ��悤�ɁA���݂̗̑͂��񕜂�����
    mHP = min(heal+mHP,GetMaxHP());
}

float PlayerCharacter::GetMoveSpeed() const
{
    return mMoveSpeed;
}

float PlayerCharacter::GetCurrentExp() const
{
    return mCurrentExp;
}

void PlayerCharacter::AddPlayerComponents(std::shared_ptr<PlayerCharacter>self)
{
    //�R���|�[�l���g�f�[�^��`
    struct ComponentInfo {
        std::shared_ptr<Component> component;
        int index;
        bool isEnable;
    };
    //�e�R���|�[�l���g�쐬
    std::vector<ComponentInfo> components = {
        {std::make_shared<PlayerMovementComponent>(self), 0,true},
        {std::make_shared<PlayerMeleeComponent>(self), 1,true},
        {std::make_shared<PlayerGunComponent>(self), 2,true},
        {std::make_shared<PlayerThrowBombComponent>(self), 3,false},
        {std::make_shared<PlayerReflectionGunComponent>(self), 4,false}
    };
    //�e�R���|�[�l���g�ɑΉ����������A�����V�Ȃǂ�ǉ��A�X�V
    for (auto& info : components) {
        info.component->SetEnable(info.isEnable);
        //���߂���g�p�\���ǂ���
        if(info.isEnable){
            STATUS.UpdateRewards(info.component->GetName());
            //�������탊�X�g�ɒǉ�
            mStartEnableComponentList.push_back(info.index);
        }
        //�R���|�[�l���g�ǉ�
        AddComponent(info.component, info.index);
    }
}

void PlayerCharacter::ResetPlayerComponents()
{
    auto components = GetComponents();
    for (auto& component : components) {
        component->SetEnable(false); // �S�ẴR���|�[�l���g����U������
    }
    
    // �K�v�ȃR���|�[�l���g�݂̂�L����
    for(auto &index:mStartEnableComponentList){
        components[index]->SetEnable(true);
        STATUS.UpdateRewards(components[index]->GetName());
    }
}

void PlayerCharacter::EnablePlayerComponent(const std::string componentName)
{
    auto comp = GetComponent(componentName);
    if(!comp) {
        return;
    }

    //���łɒǉ��ς�
    if(comp->IsEnable()){
        return;
    }

    comp->SetEnable(true);
    STATUS.UpdateRewards(componentName);
}
