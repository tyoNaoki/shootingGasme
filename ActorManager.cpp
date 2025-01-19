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

bool ActorManager::Init()
{
	//登録
	initializePool(CharacterType::PLAYER, "PlayerCharacter", 1);

	initializePool(CharacterType::BOSS, "BossEnemyA", 1);

	initializePool(CharacterType::ENEMY,"NormalEnemyA", enemyPoolSize);

	initializePool(CharacterType::ENEMY, "NormalEnemyB", enemyBPoolSize);

	initializePool(CharacterType::ENEMY, "NormalEnemyC", enemyCPoolSize);

	initializePool(CharacterType::ITEM, "LevelUpItem",itemPoolSize);

	initializePool(CharacterType::ITEM, "HealItem", itemPoolSize);

	initializePool(CharacterType::BULLET, "PlayerBullet", playerbulletPoolSize);

	initializePool(CharacterType::BULLET, "PlayerReflectionBullet", playerReflectionbulletPoolSize);

	initializePool(CharacterType::BULLET, "EnemyBullet", bulletPoolSize);

	initializePool(CharacterType::BULLET, "BossBullet", bossBulletPoolSize);

	initializePool(CharacterType::LASER,"Laser",laserPoolSize);

	initializePool(CharacterType::BOMB,"PlayerBomb",bombPoolSize);

	mInitialize = true;
	return true;
}

bool ActorManager::End()
{
	mPlayer = nullptr;
	mBoss = nullptr;

	mActiveEnemies.clear();

	for (auto& pair : mEnemyPools) {
		while (!pair.second.empty()) {
			pair.second.pop();
		}
	}

	mEnemyPools.clear();

	for (auto& pair : mItemPools) {
		while (!pair.second.empty()) {
			pair.second.pop();
		}
	}

	mItemPools.clear();

	for (auto& pair : mBulletPools) {
		while (!pair.second.empty()) {
			pair.second.pop();
		}
	}

	mBulletPools.clear();

	mActiveItems.clear();
	mActiveBullets.clear();
	return true;
}

bool ActorManager::Reset()
{
	for(auto &x:mActiveEnemies){
		x->SetActive(false);
		mEnemyPools[x->GetTypeName()].push(x);
	}

	for (auto& x : mActiveItems) {
		x->SetActive(false);
		mItemPools[x->GetTypeName()].push(x);
	}

	for (auto& x : mActiveBullets) {
		x->SetActive(false);
		mBulletPools[x->GetTypeName()].push(x);
	}

	mActiveEnemies.clear();
	mActiveItems.clear();
	mActiveBullets.clear();
	mIsSpawnPlayer = false;
	mIsSpawnBoss = false;

	return true;
}

void ActorManager::initializePool(const CharacterType ct,const std::string typeName, int size)
{
	std::shared_ptr<PlayerCharacter> player = nullptr;
	std::shared_ptr<CharacterBase>enemy = nullptr;
	std::shared_ptr<ItemBase>item = nullptr;
	std::shared_ptr<Bullet>bullet = nullptr;
	std::shared_ptr<Laser>laser = nullptr;
	std::shared_ptr<Bomb>bomb = nullptr;
	std::shared_ptr<BossBase>boss = nullptr;
	switch (ct)
	{
		case CharacterType::ENEMY:
			for (int i = 0; i < size; ++i) {
				enemy = Singleton<EnemyFactory>::get_instance().CreateObject(typeName);
				enemy->SetActive(false);
				mEnemyPools[typeName].push(enemy);
			}
			break;
		case CharacterType::ITEM:
			for (int i = 0; i < size; ++i) {
				item = std::static_pointer_cast<ItemBase>(Singleton<ItemFactory>::get_instance().CreateObject(typeName));
				item->SetActive(false);
				mItemPools[typeName].push(item);
			}
			break;
		case CharacterType::BULLET:
			for (int i = 0; i < size; ++i) {
				bullet = std::static_pointer_cast<Bullet>(Singleton<BulletFactory>::get_instance().CreateObject(typeName));
				bullet->SetActive(false);
				mBulletPools[typeName].push(bullet);
			}
			break;
		case CharacterType::LASER:
			for (int i = 0; i < size; ++i) {
				laser = std::static_pointer_cast<Laser>(Singleton<LaserFactory>::get_instance().CreateObject(typeName));
				laser->SetActive(false);
				mLaserPools[typeName].push(laser);
			}
			break;
		case CharacterType::BOMB:
			for (int i = 0; i < size; ++i) {
				bomb = std::static_pointer_cast<Bomb>(Singleton<BombFactory>::get_instance().CreateObject(typeName));
				bomb->SetActive(false);
				mBombPools[typeName].push(bomb);
			}
			break;
		case CharacterType::PLAYER:
			player = std::dynamic_pointer_cast<PlayerCharacter>(Singleton<PlayerFactory>::get_instance().CreateObject(typeName));
			if (player) {
				player->SetActive(false);
				mPlayer = player;
			}
			break;
		case CharacterType::BOSS:
			boss = std::dynamic_pointer_cast<BossBase>(Singleton<BossFactory>::get_instance().CreateObject(typeName));
			if (boss) {
				boss->SetActive(false);
				mBoss = boss;
			}
			break;
		default:
			break;
	}
}

std::shared_ptr<CharacterBase> ActorManager::getFromPool(CharacterType ct,std::string typeName, std::queue<std::shared_ptr<CharacterBase>>& pool,const Vector2D<float>&localPosition,const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	auto& f = Singleton<EnemyFactory>::get_instance();

	if (pool.empty()) {
		DEBUG_HELPER.Add("CharacterBase Pool is Empty", 5.0f,GetColor(255,0,0));
		auto enemy = f.CreateObject(typeName);
		if(!enemy){return nullptr;}
		int id = f.CreateID(typeName);
		enemy->Init(ct,typeName,id,localPosition,spawnWorldPosition,rotation,isVisible);
		return enemy;
	}else {
		auto enemy = pool.front();
		pool.pop();
		int id = f.CreateID(typeName);
		if(!enemy->IsInitialize()){
			enemy->Init(ct, typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
		}else{
			enemy->Reset(id, localPosition, spawnWorldPosition, rotation, isVisible);
		}

		return enemy;
	}
}

std::shared_ptr<ItemBase> ActorManager::getFromPool(CharacterType ct, std::string typeName, std::queue<std::shared_ptr<ItemBase>>& pool, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	auto& f = Singleton<ItemFactory>::get_instance();

	if (pool.empty()) {
		DEBUG_HELPER.Add("ItemBase Pool is Empty", 5.0f, GetColor(255, 0, 0));
		auto item = std::dynamic_pointer_cast<ItemBase>(f.CreateObject(typeName));
		if(!item){
			return nullptr;
		}
		int id = f.CreateID(typeName);
		item->Init(ct, typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
		return item;
	}
	else {
		auto item = pool.front();
		pool.pop();
		int id = f.CreateID(typeName);
		if (!item->IsInitialize()) {
			item->Init(ct, typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
		}
		else {
			item->Reset(id, localPosition, spawnWorldPosition, rotation, isVisible);
		}

		return item;
	}

	return std::shared_ptr<ItemBase>();
}

std::shared_ptr<Bullet> ActorManager::getFromPool(CharacterType ct,CharacterType ownerType,std::string typeName, std::queue<std::shared_ptr<Bullet>>& pool, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	auto& f = Singleton<BulletFactory>::get_instance();

	if (pool.empty()) {
		DEBUG_HELPER.Add("Bullet Pool is Empty",5.0f, GetColor(255, 0, 0));
		auto bullet = std::dynamic_pointer_cast<Bullet>(f.CreateObject(typeName));
		if(!bullet){
			return nullptr;
		}
		int id = f.CreateID(typeName);
		bullet->Init(ct,ownerType,typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
		return bullet;
	}
	else {
		auto item = pool.front();
		pool.pop();
		int id = f.CreateID(typeName);
		if (!item->IsInitialize()) {
			item->Init(ct,ownerType,typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
		}
		else {
			item->Reset(ownerType,id, localPosition, spawnWorldPosition, rotation, isVisible);
		}

		return item;
	}

	return std::shared_ptr<Bullet>();
}

std::shared_ptr<Laser> ActorManager::getFromPool(CharacterType ct, std::shared_ptr<CharacterBase> owner, std::string typeName, std::queue<std::shared_ptr<Laser>>& pool, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	auto& f = Singleton<LaserFactory>::get_instance();

	if (pool.empty()) {
		DEBUG_HELPER.Add("Laser Pool is Empty", 5.0f, GetColor(255, 0, 0));
		auto laser = std::dynamic_pointer_cast<Laser>(f.CreateObject(typeName));
		if (!laser) {
			return nullptr;
		}
		int id = f.CreateID(typeName);
		laser->Init(ct, owner, typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
		return laser;
	}
	else {
		auto laser = pool.front();
		pool.pop();
		int id = f.CreateID(typeName);
		if (!laser->IsInitialize()) {
			laser->Init(ct, owner, typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
		}
		else {
			laser->Reset(owner, id, localPosition, spawnWorldPosition, rotation, isVisible);
		}
		return laser;
	}

	return std::shared_ptr<Laser>();
}

std::shared_ptr<Bomb> ActorManager::getFromPool(CharacterType ct, CharacterType ownerType, std::string typeName, std::queue<std::shared_ptr<Bomb>>& pool, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	auto& f = Singleton<BombFactory>::get_instance();

	if (pool.empty()) {
		DEBUG_HELPER.Add("Bomb Pool is Empty", 5.0f, GetColor(255, 0, 0));
		auto bomb = std::dynamic_pointer_cast<Bomb>(f.CreateObject(typeName));
		if (!bomb) {
			return nullptr;
		}
		int id = f.CreateID(typeName);
		bomb->Init(ct, ownerType, typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
		return bomb;
	}
	else {
		auto bomb = pool.front();
		pool.pop();
		int id = f.CreateID(typeName);
		if (!bomb->IsInitialize()) {
			bomb->Init(ct, ownerType, typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
		}
		else {
			bomb->Reset(ownerType, id, localPosition, spawnWorldPosition, rotation, isVisible);
		}
		return bomb;
	}

	return std::shared_ptr<Bomb>();
}

std::shared_ptr<CharacterBase> ActorManager::GetPlayer(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	if (!mPlayer) { return nullptr; }

	int id = Singleton<PlayerFactory>::get_instance().CreateID(typeName);
	if(!mPlayer->IsInitialize()){
		mPlayer->Init(CharacterType::PLAYER, typeName, id, localPosition, spawnWorldPosition, 90, true);
	}else{
		mPlayer->Reset(id,localPosition,spawnWorldPosition,rotation,isVisible);
	}
	
	return mPlayer;
}

std::shared_ptr<CharacterBase> ActorManager::GetEnemy(const std::string& typeName,const Vector2D<float>&localPosition,const Vector2D<float>&spawnWorldPosition, const float rotation, bool isVisible)
{
	return getFromPool(CharacterType::ENEMY,typeName,mEnemyPools[typeName],localPosition,spawnWorldPosition,rotation,isVisible);
}

std::shared_ptr<CharacterBase> ActorManager::GetBossEnemy(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	if (!mBoss) { return nullptr; }

	int id = Singleton<BossFactory>::get_instance().CreateID(typeName);

	if (!mBoss->IsInitialize()) {
		mBoss->Init(CharacterType::BOSS, typeName, id, localPosition, spawnWorldPosition, 90, true);
	}else {
		mBoss->Reset(id, localPosition, spawnWorldPosition, rotation, isVisible);
	}
	
	return mBoss;
}

std::shared_ptr<ItemBase> ActorManager::GetItem(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	return getFromPool(CharacterType::ITEM, typeName, mItemPools[typeName], localPosition, spawnWorldPosition, rotation, isVisible);
}

std::shared_ptr<Bullet> ActorManager::GetBullet(const std::string& typeName,CharacterType ownerType,const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	return getFromPool(CharacterType::BULLET,ownerType,typeName, mBulletPools[typeName], localPosition, spawnWorldPosition, rotation, isVisible);
}

std::shared_ptr<Laser> ActorManager::GetLaser(const std::string& typeName, std::shared_ptr<CharacterBase>owner, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	return getFromPool(CharacterType::LASER, owner, typeName, mLaserPools[typeName], localPosition, spawnWorldPosition, rotation, isVisible);
}

std::shared_ptr<Bomb> ActorManager::GetBomb(const std::string& typeName, CharacterType ownerType, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	return getFromPool(CharacterType::BOMB, ownerType, typeName, mBombPools[typeName], localPosition, spawnWorldPosition, rotation, isVisible);
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

void ActorManager::ReturnEnemy(std::shared_ptr<CharacterBase> enemy)
{
	enemy->SetActive(false);
	mEnemyPools[enemy->GetTypeName()].push(enemy);
}

void ActorManager::ReturnItem(std::shared_ptr<ItemBase> item)
{
	item->SetActive(false);
	mItemPools[item->GetTypeName()].push(item);
}

void ActorManager::ReturnBullet(std::shared_ptr<Bullet> bullet)
{
	bullet->SetActive(false);
	mBulletPools[bullet->GetTypeName()].push(bullet);
}

void ActorManager::ReturnLaser(std::shared_ptr<Laser> laser)
{
	laser->SetActive(false);
	mLaserPools[laser->GetTypeName()].push(laser);
}

void ActorManager::ReturnBomb(std::shared_ptr<Bomb> bomb)
{
	bomb->SetActive(false);
	mBombPools[bomb->GetTypeName()].push(bomb);
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

			//現在のコリジョンの現在グリッドの更新
			COLLISION_M.UpdateGridIndex((*itr));

			itr++;
		}else { 
			//死んでいる場合、アイテムを確率でドロップ
			if((*itr)->IsDead()){
				auto itemName = ChoiceDropItem((*itr)->GetTypeName());
				if(itemName!="None"){
					auto item = GetItem(itemName, (*itr)->GetLocalPosition2D(), (*itr)->GetWorldPosition2D(), (*itr)->GetRotation(), true);
					if (item) {
						//各アイテム初期化処理
						if (item->GetTypeName() == "LevelUpItem") {//経験値
							std::static_pointer_cast<LevelUpItem>(item)->SetExp((*itr)->GetLevel(), (*itr)->GetTypeName());
						}
						if (item->GetTypeName() == "HealItem") {//回復アイテム
							std::static_pointer_cast<HealItem>(item)->SetHeal((*itr)->GetLevel(), (*itr)->GetTypeName());
						}
						//ドロップアイテム登録でアクティブ状態にする
						RegistObject(item);
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
			ReturnEnemy(*itr);
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
			ReturnItem(*itr);
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
			ReturnBullet(*itr);
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
			ReturnLaser(*itr);
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
			ReturnBomb(*itr);
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

bool ActorManager::ContainsEnemyTypeName(const std::string& typeName)
{
	auto it = mEnemyPools.find(typeName);
	return it!=mEnemyPools.end();
}

bool ActorManager::ContainsItemTypeName(const std::string& typeName)
{
	auto it = mItemPools.find(typeName);
	return it != mItemPools.end();
}

bool ActorManager::ContainsBulletTypeName(const std::string& typeName)
{
	auto it = mBulletPools.find(typeName);
	return it != mBulletPools.end();
}

bool ActorManager::RegistObject(std::shared_ptr<CharacterBase> chara)
{
	if (!chara) { return false; }

	

	//初期化終わっているか
	if(!chara->IsInitialize()){
		return false;
	}

	//プレイヤーならスポーンフラグを立てる
	auto charaType = chara->GetActorType();
	if(charaType == CharacterType::PLAYER){
		mIsSpawnPlayer = true;
	}
	//敵なら、アクティブのデータに追加
	if(charaType == CharacterType::ENEMY){
		mActiveEnemies.push_back(chara);
	}

	//ボスなら、スポーンフラグを立てて、アクティブ状態に設定
	if(charaType == CharacterType::BOSS){
		mIsSpawnBoss = true;
		chara->SetActive(true);
		return true;
	}

	//現在のコリジョンの座標を衝突判定用のグリッドに登録する
	COLLISION_M.AddCharacter(chara->GetWorldPosition2D(), chara);

	chara->SetActive(true);
	return true;
}
bool ActorManager::RegistObject(std::shared_ptr<ItemBase> item)
{
	if (!item) { return false; }

	//初期化終わっているか
	if (!item->IsInitialize()) {
		return false;
	}

	//弾丸以外なら
	if (item->GetActorType() != CharacterType::ITEM) {
		return false;
	}
	//アクティブ状態のデータに格納,アクティブに設定
	mActiveItems.push_back(item);
	item->SetActive(true);

	return true;
}
bool ActorManager::RegistObject(std::shared_ptr<Bullet> bullet)
{
	if(!bullet){return false;}

	//初期化終わっているか
	if (!bullet->IsInitialize()) {
		return false;
	}
	//弾以外なら
	if (bullet->GetActorType() != CharacterType::BULLET) {
		return false;
	}
	//アクティブ状態のデータに格納,アクティブに設定
	mActiveBullets.push_back(bullet);
	bullet->SetActive(true);
	return true;
}

bool ActorManager::RegistObject(std::shared_ptr<Laser> laser)
{
	if (!laser) { return false; }

	//初期化終わっているか
	if (!laser->IsInitialize()) {
		return false;
	}
	//レーザー以外なら
	if (laser->GetActorType() != CharacterType::LASER) {
		return false;
	}

	//アクティブ状態のデータに格納,アクティブに設定
	mActiveLasers.push_back(laser);
	laser->SetActive(true);

	return true;
}

bool ActorManager::RegistObject(std::shared_ptr<Bomb> bomb)
{
	if (!bomb) { return false; }

	//初期化終わっているか
	if (!bomb->IsInitialize()) {
		return false;
	}
	//爆弾以外なら
	if (bomb->GetActorType() != CharacterType::BOMB) {
		return false;
	}

	//アクティブ状態のデータに格納,アクティブに設定
	mActiveBombs.push_back(bomb);
	bomb->SetActive(true);

	return true;
}


int ActorManager::GetActorCount()
{
	return mActiveEnemies.size();
}

int ActorManager::GetItemCount()
{
	return mActiveItems.size();
}

int ActorManager::GetBulletCount()
{
	return mActiveBullets.size();
}

ActorManager::ActorManager()
{
}

ActorManager::~ActorManager()
{
	End();
}

