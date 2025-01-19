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

    //コリジョン座標更新
    SetWorldPosition2D(worldPosition);

    std::shared_ptr<PlayerCharacter> self = std::static_pointer_cast<PlayerCharacter>(shared_from_this());

    //コンポーネント追加
    AddPlayerComponents(self);

    float drawRot = GetDrawRotation();
    
    //左向きにさせる
    if (drawRot > 90.0f && drawRot < 270.0f) {
        mReverse = true;
        //角度が90度か270度に近い数値なら右向き
    }else if (nearEqual(drawRot, 90.0f) || nearEqual(drawRot, 270.0f)) {
        mReverse = false;
    }else {
        //右向き
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

    //コンポーネントのステータスリセット
    InitComponents();

    //初期装備にリセット
    ResetPlayerComponents();

    //経験値リセット
    ResetExp();

    //コリジョン座標更新
    SetWorldPosition2D(worldPosition);

    float drawRot = GetDrawRotation();

    //左向きにさせる
    if (drawRot > 90.0f && drawRot < 270.0f) {
        mReverse = true;
        //角度が90度か270度に近い数値なら右向き
    }
    else if (nearEqual(drawRot, 90.0f) || nearEqual(drawRot, 270.0f)) {
        mReverse = false;
    }
    else {
        //右向き
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
    //デバッグ
    auto& dh = Singleton<DebugHelper>::get_instance();
    auto scene = Singleton<SceneManager>::get_instance().GetCurrentScene();
    if (!scene) { return; }

    //死んでいる
    if(mIsDead){
        SetLocalPosition2D(GetLocalFromWorldPosi(GetWorldPosition2D()));
        //死んでいる場合、画像更新だけ行う
        ANIM_M.Update(this);
        GRAPHIC_M.updateHandle(deltaTime, mCurrent_gHandle, mCurrent_AnimFrame, mCurrent_AnimFrame_adjust, GetTypeName(), GetCurrentAnimState()->GetActionName());
        return;
    }

    //コンポーネント更新
    CharacterBase::Update(deltaTime);

    //動いているかどうか
    bool isMove = IsMoving();

    //現在の描画角度取得
    float currentRotation = GetDrawRotation();

    //左向きかどうか
    bool isReverse = false;

    //アニメーション画像タイプ更新(通常歩行、反転歩行)
    if(isMove){
        if (nearEqual(currentRotation, 90.0f) || nearEqual(currentRotation, 270.0f)) {
            //角度が90度か270度に近い数値なら、向きはそのままの向きで固定
            isReverse = mReverse;
        }
        //左向きにさせる
        else if(currentRotation > 90.0f && currentRotation < 270.0f) {
            isReverse = true;
            //角度が90度か270度に近い数値なら向きはそのままで返す
        }else{
            //右向き
            isReverse = false;
        }
    //止まっている時
    }else if(!isMove){
        isReverse = mReverse;
    }

    mReverse = isReverse;

    //速度取得
    Vector2D<float> cur_vel = GetVelocity();

    //衝突検知//
    auto newPosi = GetWorldPosition2D();
    auto newCollisionPosi = GetCollision();

    //マップとの当たり判定管理
    auto map = scene->GetMap();
    auto& cm = Singleton<CollisionManager>::get_instance();

    //マップ外にいた場合、中に戻す
    if(cm.PushBackFromOut(newPosi,newCollisionPosi,map)){
        std::string message = "Player is out";
        dh.Add(message);
    }

    //静的障害物と接触していた場合、押し戻し処理を実行
    bool isColliding = cm.PushBackFromWalls(newPosi, newCollisionPosi, map);

    //ボスとの当たり判定
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

    //デバッグ用コリジョン描画
    dh.DrawCollision(newCollisionPosi, 0.0f, isColliding);
    
    map->calcMapPosition(newPosi);
    
    Vector2D<float>localPosi = map->GetLocalFromWorldPosition(newPosi);

    //座標更新
    SetLocalPosition2D(localPosi);
    SetWorldPosition2D(newPosi);

    std::string preActionName = GetCurrentAnimState()->GetActionName();

    //アニメーション遷移更新
    ANIM_M.Update(this);

    //現在のアニメーション画像更新
    //新アニメーション遷移
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
    //死んでいるか
    if(IsDead()){
        return;
    }
    //ダメージ
    mHP -= damage;

    //死亡アニメーション
    if (mHP <= 0){
        mHP = 0;
        StartDeadAnimation();
        return;
    }

    //ノックバックがある場合、開始処理
    if(strength > 0.0f){
        AddKnockBack(direction,strength,knockBackTime);
    }

    //ダメージアニメーション
    DamageAnimation();
}

void PlayerCharacter::StartLevelUp(int levelUpCount)
{
    //レベルアップの報酬選択開始
    SCENE_M.OnLevelUpEvent(levelUpCount);
}

void PlayerCharacter::LevelUp()
{
    //ステータス設定
    auto status = STATUS.GetCurrentPlayerStatus();
    mLevel = status.level;
    mHP = status.hp;
    mMaxHP = status.hp;
    mMoveSpeed = status.moveSpeed;
}

void PlayerCharacter::WeaponLevelUp(const std::string componentName)
{
    //武器ステータス更新
    auto comp = GetComponent(componentName);
    if(comp){
        comp->SetStatus(STATUS.GetCurrentWeaponStatus(componentName));
    }
}

void PlayerCharacter::ResetExp()
{
    //経験値初期化
    mAllExp = 0.0f;
    mCurrentExp = 0.0f;
}

void PlayerCharacter::TakeExp(float exp)
{
    //レベルが最大レベルに達しているか
    if(mLevel >= STATUS.GetPlayerMaxLevel()){
        return;
    }
    
    //経験値取得
    mAllExp += exp;
    mCurrentExp += exp;
    
    // レベルアップ処理
    bool isLevelUp = false;
    int levelUpCount = 0;
    //必要な経験値を超えた分だけレベルアップ処理を行う
    while (mCurrentExp >= STATUS.GetNeedExp(mLevel + levelUpCount)) {
        isLevelUp = true;
        //現在の取得経験値から必要な経験値を引く
        mCurrentExp -= STATUS.GetNeedExp(mLevel + levelUpCount);
        //レベルアップの回数記録
        levelUpCount++;
        //レベルが最大になったら、取得経験値を0にする
        if (mLevel+levelUpCount >= STATUS.GetPlayerMaxLevel()) {
            mCurrentExp = 0.0f;
            break;
        }
    }
    //レベルアップフラグが経っているなら、レベルアップイベント開始
    if(isLevelUp){
        StartLevelUp(levelUpCount);
    }
}

void PlayerCharacter::TakeHeal(float heal)
{
    //体力がすでに最大ならスキップ
    if(GetHP()==GetMaxHP()){
        return;
    }
    //最大体力を超えないように、現在の体力を回復させる
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
    //コンポーネントデータ定義
    struct ComponentInfo {
        std::shared_ptr<Component> component;
        int index;
        bool isEnable;
    };
    //各コンポーネント作成
    std::vector<ComponentInfo> components = {
        {std::make_shared<PlayerMovementComponent>(self), 0,true},
        {std::make_shared<PlayerMeleeComponent>(self), 1,true},
        {std::make_shared<PlayerGunComponent>(self), 2,true},
        {std::make_shared<PlayerThrowBombComponent>(self), 3,false},
        {std::make_shared<PlayerReflectionGunComponent>(self), 4,false}
    };
    //各コンポーネントに対応した強化、入手報酬などを追加、更新
    for (auto& info : components) {
        info.component->SetEnable(info.isEnable);
        //初めから使用可能かどうか
        if(info.isEnable){
            STATUS.UpdateRewards(info.component->GetName());
            //初期武器リストに追加
            mStartEnableComponentList.push_back(info.index);
        }
        //コンポーネント追加
        AddComponent(info.component, info.index);
    }
}

void PlayerCharacter::ResetPlayerComponents()
{
    auto components = GetComponents();
    for (auto& component : components) {
        component->SetEnable(false); // 全てのコンポーネントを一旦無効化
    }
    
    // 必要なコンポーネントのみを有効化
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

    //すでに追加済み
    if(comp->IsEnable()){
        return;
    }

    comp->SetEnable(true);
    STATUS.UpdateRewards(componentName);
}
