#include "ActorManager.h"
#include <memory>
#include "SceneManager.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "PlayerFactory.h"
#include "ItemFactory.h"
#include "BulletFactory.h"
#include "LaserFactory.h"
#include "BombFactory.h"
#include "BossFactory.h"
#include "PlayerCharacter.h"
#include "StatusManager.h"
#include "LevelUpItem.h"
#include "HealItem.h"
#include <random>
#include "EnemyFactory.h"
#include <functional>
#include "ObjectPoolManager.h"

bool ActorManager::Init()
{
	Singleton<ObjectPoolManager>().get_instance().Init();

	mInitialize = true;
	return true;
}

bool ActorManager::End()
{
	//念のため、null,それぞれの配列の中身を空にする
	mPlayer = nullptr;
	mBoss = nullptr;

	mActiveEnemies.clear();
	mActiveItems.clear();
	mActiveBullets.clear();
	return true;
}

bool ActorManager::Reset()
{
	//それぞれの配列のオブジェクトを非アクティブにする
	auto &obj_M = Singleton<ObjectPoolManager>().get_instance();

	auto pool = obj_M.GetPool(NameToCharacterType(CharacterType::ENEMY));
	for(auto &x:mActiveEnemies){
		
		pool->OnRelease(x->GetTypeName().c_str(),x);
	}

	pool = obj_M.GetPool(NameToCharacterType(CharacterType::ITEM));
	for (auto& x : mActiveItems) {
		pool->OnRelease(x->GetTypeName().c_str(),x);
	}

	pool = obj_M.GetPool(NameToCharacterType(CharacterType::BULLET));
	for (auto& x : mActiveBullets) {
		pool->OnRelease(x->GetTypeName().c_str(), x);
	}

	//全て空にする
	mActiveEnemies.clear();
	mActiveItems.clear();
	mActiveBullets.clear();
	mIsSpawnPlayer = false;
	mIsSpawnBoss = false;

	return true;
}

std::shared_ptr<Actor> ActorManager::Get(CharacterType ct,const char* typeName)
{
	auto pool = Singleton<ObjectPoolManager>::get_instance().GetPool(NameToCharacterType(ct));

	if (!pool) return nullptr;

	return pool->OnGet(typeName);
}

std::shared_ptr<CharacterBase> ActorManager::GetPlayer(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	//作成されてない場合、その場で作成する
	if(!mPlayer) 
	{
		auto player = std::dynamic_pointer_cast<PlayerCharacter>(Singleton<PlayerFactory>::get_instance().CreateObject(typeName));
		if (!player) return nullptr;
		player->SetActive(false);
		mPlayer = player;
	}

	//ID発行
	int id = Singleton<PlayerFactory>::get_instance().CreateID(typeName);
	//初期化、リセット処理
	if(!mPlayer->IsInitialize()){
		mPlayer->Init(CharacterType::PLAYER, typeName, id, localPosition, spawnWorldPosition, 90, true);
	}else{
		mPlayer->Reset(id,localPosition,spawnWorldPosition,rotation,isVisible);
	}

	mIsSpawnPlayer = true;

	//現在のコリジョンの座標を衝突判定用のグリッドに登録する
	COLLISION_M.AddCharacter(spawnWorldPosition, mPlayer);
	mPlayer->SetActive(true);
	return mPlayer;
}

std::shared_ptr<CharacterBase> ActorManager::GetEnemy(const char* typeName,const Vector2D<float>&localPosition,const Vector2D<float>&spawnWorldPosition, const float rotation, bool isVisible)
{
	auto enemy = std::dynamic_pointer_cast<CharacterBase>(Get(CharacterType::ENEMY,typeName));

	int id = Singleton<EnemyFactory>::get_instance().CreateID(typeName);

	if (!enemy->IsInitialize()) {
		enemy->Init(CharacterType::ENEMY, typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
	}
	else {
		enemy->Reset(id, localPosition, spawnWorldPosition, rotation, isVisible);
	}

	mActiveEnemies.push_back(enemy);
	//現在のコリジョンの座標を衝突判定用のグリッドに登録する
	COLLISION_M.AddCharacter(spawnWorldPosition, enemy);
	enemy->SetActive(true);

	return enemy;
}

std::shared_ptr<CharacterBase> ActorManager::GetBossEnemy(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	//作成されてない場合、その場で作成する
	if (!mBoss) 
	{
		auto boss = std::dynamic_pointer_cast<BossBase>(Singleton<BossFactory>::get_instance().CreateObject(typeName));
		if (!boss) return nullptr;
		
		boss->SetActive(false);
		mBoss = boss;
	}

	//ID発行
	int id = Singleton<BossFactory>::get_instance().CreateID(typeName);

	//初期化、リセット処理
	if (!mBoss->IsInitialize()) {
		mBoss->Init(CharacterType::BOSS, typeName, id, localPosition, spawnWorldPosition, 90, true);
	}else {
		mBoss->Reset(id, localPosition, spawnWorldPosition, rotation, isVisible);
	}

	mIsSpawnBoss = true;
	mBoss->SetActive(true);
	return mBoss;
}

std::shared_ptr<ItemBase> ActorManager::GetItem(const char* typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	auto item = std::dynamic_pointer_cast<ItemBase>(Get(CharacterType::ITEM, typeName));

	if(!item) return nullptr;

	int id = Singleton<ItemFactory>::get_instance().CreateID(typeName);

	if (!item->IsInitialize()) {
		item->Init(CharacterType::ITEM, typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
	}
	else {
		item->Reset(id, localPosition, spawnWorldPosition, rotation, isVisible);
	}

	//アクティブ状態のデータに格納,アクティブに設定
	mActiveItems.push_back(item);
	item->SetActive(true);
	return item;
}

std::shared_ptr<Bullet> ActorManager::GetBullet(const char* typeName,CharacterType ownerType,const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	auto bullet = std::dynamic_pointer_cast<Bullet>(Get(CharacterType::BULLET, typeName));

	if (!bullet) return nullptr;

	int id = Singleton<BulletFactory>::get_instance().CreateID(typeName);

	if (!bullet->IsInitialize()) {
		bullet->Init(CharacterType::BULLET,ownerType, typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
	}
	else {
		bullet->Reset(ownerType,id, localPosition, spawnWorldPosition, rotation, isVisible);
	}

	//アクティブ状態のデータに格納,アクティブに設定
	mActiveBullets.push_back(bullet);
	bullet->SetActive(true);
	return bullet;
}

std::shared_ptr<Laser> ActorManager::GetLaser(const char* typeName, std::shared_ptr<CharacterBase>owner, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	auto laser = std::dynamic_pointer_cast<Laser>(Get(CharacterType::LASER, typeName));

	if (!laser) return nullptr;

	//ID発行
	int id = Singleton<LaserFactory>::get_instance().CreateID(typeName);
	if (!laser->IsInitialize()) {
		laser->Init(CharacterType::LASER, owner, typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
	}
	else {
		laser->Reset(owner, id, localPosition, spawnWorldPosition, rotation, isVisible);
	}

	//アクティブ状態のデータに格納,アクティブに設定
	mActiveLasers.push_back(laser);
	laser->SetActive(true);
	return laser;
}

std::shared_ptr<Bomb> ActorManager::GetBomb(const char* typeName, CharacterType ownerType, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	auto bomb = std::dynamic_pointer_cast<Bomb>(Get(CharacterType::BOMB, typeName));

	if (!bomb) return nullptr;

	int id = Singleton<BombFactory>::get_instance().CreateID(typeName);
	if (!bomb->IsInitialize()) {
		bomb->Init(CharacterType::BOMB, ownerType, typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
	}
	else {
		bomb->Reset(ownerType, id, localPosition, spawnWorldPosition, rotation, isVisible);
	}

	//アクティブ状態のデータに格納,アクティブに設定
	mActiveBombs.push_back(bomb);
	bomb->SetActive(true);
	return bomb;
}

std::string ActorManager::ChoiceDropItem(const std::string& enemyTypeName)
{
	auto dropItemList = STATUS.GetDropItems(enemyTypeName);
	if(dropItemList.size()==0){
		return std::string();
	}

	// 累積確率を計算
	std::vector<float> cumulativeWeights;
	float totalWeight = 0.0f;
	for (auto item : dropItemList) {
		totalWeight += item.dropWeight;
		cumulativeWeights.push_back(totalWeight);
	}
	
	// ランダム数生成器を初期化 
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, totalWeight);
	
	// 乱数を生成 
	float randomValue = static_cast<float>(dis(gen));
	
	// 重みを使って要素を選択 
	for (int i = 0; i < cumulativeWeights.size(); ++i) {
		if (randomValue < cumulativeWeights[i]) {
			return dropItemList[i].itemName;
		}
	}
	
	return std::string();
}

void ActorManager::ReturnPool(std::shared_ptr<Actor> obj)
{
	auto pool = Singleton<ObjectPoolManager>().get_instance().GetPool(NameToCharacterType(obj->GetActorType()));
	pool->OnRelease(obj->GetTypeName().c_str(),obj);
}

const char* ActorManager::NameToCharacterType(CharacterType ct)
{
	switch (ct)
	{
	case CharacterType::PLAYER:
		return "Player";
	case CharacterType::ENEMY:
		return "Enemy";
	case CharacterType::BULLET:
		return "Bullet";
	case CharacterType::LASER:
		return "Laser";
	case CharacterType::BOMB:
		return "Bomb";
	case CharacterType::ITEM:
		return "Item";
	case CharacterType::BOSS:
		return "Boss";
	default:
		break;
	}
	return "";
}

void ActorManager::Update(const float deltaTime)
{
	auto scene = SCENE_M.GetCurrentScene();
	if(!scene){return;}

	auto map = scene->GetMap();

	//プレイヤーから先
	if(mPlayer&&mIsSpawnPlayer){
		if (mPlayer->IsActive()) {
			mPlayer->SetOldWorldPosition2D(mPlayer->GetWorldPosition2D());
			mPlayer->Update(deltaTime);

			COLLISION_M.UpdateGridIndex(mPlayer);
		}else{
			if(mPlayer->IsDead()){
				
			}
			auto position = mPlayer->GetWorldPosition2D();
			//COLLISION_M.RemoveActor(position, mPlayer);
			COLLISION_M.RemoveIndex(mPlayer->GetGridIndex(), mPlayer);
			mIsSpawnPlayer = false;
			//mPlayer = nullptr;
		}
	}
	
	//敵の更新
	for (auto itr = mActiveEnemies.begin(); itr != mActiveEnemies.end();) {
		if ((*itr)->IsActive()) {
			if(DEBUG_HELPER.IsDebugActor((*itr)->GetName())){
				bool isDebug = true;
			}
			(*itr)->SetOldWorldPosition2D((*itr)->GetWorldPosition2D());
			(*itr)->Update(deltaTime);

			//死んでいる場合、コリジョンリストから除き、スキップ
			if((*itr)->IsDead()){
				COLLISION_M.RemoveIndex((*itr)->GetGridIndex(), *itr);
				itr++;
				continue;
			}

			//現在のコリジョンのグリッド更新
			COLLISION_M.UpdateGridIndex((*itr));

			itr++;
		}else { 
			//死んでいる場合、アイテムを確率でドロップ
			if((*itr)->IsDead()){
				auto itemName = ChoiceDropItem((*itr)->GetTypeName());
				if(itemName!="None"){
					auto item = GetItem(itemName.c_str(), (*itr)->GetLocalPosition2D(), (*itr)->GetWorldPosition2D(), (*itr)->GetRotation(), true);
					if (item) {
						//各アイテム初期化処理
						if (item->GetTypeName() == "LevelUpItem") {//経験値
							std::static_pointer_cast<LevelUpItem>(item)->SetExp((*itr)->GetLevel(), (*itr)->GetTypeName());
						}
						if (item->GetTypeName() == "HealItem") {//回復アイテム
							std::static_pointer_cast<HealItem>(item)->SetHeal((*itr)->GetLevel(), (*itr)->GetTypeName());
						}
					}
				}
				//プレイヤーにスコア加算
				STATUS.AddScore((int)(*itr)->GetScore());
				STATUS.AddCount((*itr)->GetActorType());
			}

			//念のため、コリジョンの現在座標消去
			COLLISION_M.RemoveIndex((*itr)->GetGridIndex(), *itr);

			//念のため移動
			(*itr)->SetWorldPosition2D(Vector2D<float>(0,0));

			// 非アクティブになった敵をプールに戻す
			ReturnPool(*itr);
			itr = mActiveEnemies.erase(itr); 
		}
	}

	//ボスの更新
	if (mBoss && mIsSpawnBoss) {
		//アクティブなら、座標、コンポーネント更新
		if (mBoss->IsActive()) {
			mBoss->SetOldWorldPosition2D(mBoss->GetWorldPosition2D());
			mBoss->Update(deltaTime);
		}
		else {
			//死んでいるか
			if (mBoss->IsDead()) {
				//プレイヤーにスコア加算
				STATUS.AddScore((int)mBoss->GetScore());
				STATUS.AddCount(mBoss->GetActorType());
			}
			mIsSpawnBoss = false;
		}
	}

	//ゲーム上のアイテム更新
	for (auto itr = mActiveItems.begin(); itr != mActiveItems.end();) {
		if ((*itr)->IsActive()) {
			(*itr)->Update(deltaTime);//座標更新

			itr++;
		}else{
			//非アクティブなら専用のデータに格納
			ReturnPool(*itr);
			itr = mActiveItems.erase(itr);
		}
	}

	//ゲーム上の弾更新
	for (auto itr = mActiveBullets.begin(); itr != mActiveBullets.end();) {
		if ((*itr)->IsActive()) {
			(*itr)->Update(deltaTime);//座標更新

			itr++;
		}
		else {
			//非アクティブなら専用のデータに格納
			ReturnPool(*itr);
			itr = mActiveBullets.erase(itr);
		}
	}

	//ゲーム上のレーザー更新
	for (auto itr = mActiveLasers.begin(); itr != mActiveLasers.end();) {
		if ((*itr)->IsActive()) {
			(*itr)->Update(deltaTime);//座標更新

			itr++;
		}
		else {
			//非アクティブなら専用のデータに格納
			ReturnPool(*itr);
			itr = mActiveLasers.erase(itr);
		}
	}

	//ゲーム上の爆弾更新
	for (auto itr = mActiveBombs.begin(); itr != mActiveBombs.end();) {
		if ((*itr)->IsActive()) {
			(*itr)->Update(deltaTime);//座標更新

			itr++;
		}
		else {
			//非アクティブなら専用のデータに格納
			ReturnPool(*itr);
			itr = mActiveBombs.erase(itr);
		}
	}
}

void ActorManager::Draw(const float deltaTime)
{
	//プレイヤーから先
	if(mPlayer && mIsSpawnPlayer){
		if (mPlayer->IsActive()) {
			mPlayer->Draw(deltaTime);
		}
	}

	//敵描画
	for (auto&e : mActiveEnemies) {
		if(e->IsActive()){
			e->Draw(deltaTime);
		}
	}

	//ボスの更新
	if (mBoss && mIsSpawnBoss) {
		if (mBoss->IsActive()) {
			mBoss->Draw(deltaTime);
		}
	}

	//アイテム描画
	for(auto&i : mActiveItems){
		if(i->IsActive()){
			i->Draw(deltaTime);
		}
	}

	//弾更新
	for(auto &b : mActiveBullets){
		if(b->IsActive()){
			b->Draw(deltaTime);
		}
	}

	//レーザー更新
	for (auto& l : mActiveLasers) {
		if (l->IsActive()) {
			l->Draw(deltaTime);
		}
	}

	//爆弾更新
	for (auto& b : mActiveBombs) {
		if (b->IsActive()) {
			b->Draw(deltaTime);
		}
	}
}

std::shared_ptr<PlayerCharacter> ActorManager::GetCurrentPlayer()
{
	return mPlayer;
}

std::shared_ptr<BossBase> ActorManager::GetCurrentBossEnemy()
{
	return mBoss;
}

bool ActorManager::IsSpawnBoss()
{
	return mIsSpawnBoss;
}

void ActorManager::PlayerLevelUp()
{
	mPlayer->LevelUp();
}

void ActorManager::PlayerWeaponLevelUp(std::string name)
{
	mPlayer->WeaponLevelUp(name);
}

void ActorManager::PlayerTakeExp(float exp)
{
	float currentExp = exp*STATUS.GetSurvivalBonus();
	mPlayer->TakeExp(currentExp);
}

void ActorManager::PlayerTakeHeal(float heal)
{
	mPlayer->TakeHeal(heal);
}

int ActorManager::GetActorCount()
{
	return (int)mActiveEnemies.size();
}

int ActorManager::GetItemCount()
{
	return (int)mActiveItems.size();
}

int ActorManager::GetBulletCount()
{
	return (int)mActiveBullets.size();
}

ActorManager::ActorManager()
{
}

ActorManager::~ActorManager()
{
	End();
}


