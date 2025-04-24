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
	//�O�̂��߁Anull,���ꂼ��̔z��̒��g����ɂ���
	mPlayer = nullptr;
	mBoss = nullptr;

	mActiveEnemies.clear();
	mActiveItems.clear();
	mActiveBullets.clear();
	return true;
}

bool ActorManager::Reset()
{
	//���ꂼ��̔z��̃I�u�W�F�N�g���A�N�e�B�u�ɂ���
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

	//�S�ċ�ɂ���
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
	//�쐬����ĂȂ��ꍇ�A���̏�ō쐬����
	if(!mPlayer) 
	{
		auto player = std::dynamic_pointer_cast<PlayerCharacter>(Singleton<PlayerFactory>::get_instance().CreateObject(typeName));
		if (!player) return nullptr;
		player->SetActive(false);
		mPlayer = player;
	}

	//ID���s
	int id = Singleton<PlayerFactory>::get_instance().CreateID(typeName);
	//�������A���Z�b�g����
	if(!mPlayer->IsInitialize()){
		mPlayer->Init(CharacterType::PLAYER, typeName, id, localPosition, spawnWorldPosition, 90, true);
	}else{
		mPlayer->Reset(id,localPosition,spawnWorldPosition,rotation,isVisible);
	}

	mIsSpawnPlayer = true;

	//���݂̃R���W�����̍��W���Փ˔���p�̃O���b�h�ɓo�^����
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
	//���݂̃R���W�����̍��W���Փ˔���p�̃O���b�h�ɓo�^����
	COLLISION_M.AddCharacter(spawnWorldPosition, enemy);
	enemy->SetActive(true);

	return enemy;
}

std::shared_ptr<CharacterBase> ActorManager::GetBossEnemy(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	//�쐬����ĂȂ��ꍇ�A���̏�ō쐬����
	if (!mBoss) 
	{
		auto boss = std::dynamic_pointer_cast<BossBase>(Singleton<BossFactory>::get_instance().CreateObject(typeName));
		if (!boss) return nullptr;
		
		boss->SetActive(false);
		mBoss = boss;
	}

	//ID���s
	int id = Singleton<BossFactory>::get_instance().CreateID(typeName);

	//�������A���Z�b�g����
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

	//�A�N�e�B�u��Ԃ̃f�[�^�Ɋi�[,�A�N�e�B�u�ɐݒ�
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

	//�A�N�e�B�u��Ԃ̃f�[�^�Ɋi�[,�A�N�e�B�u�ɐݒ�
	mActiveBullets.push_back(bullet);
	bullet->SetActive(true);
	return bullet;
}

std::shared_ptr<Laser> ActorManager::GetLaser(const char* typeName, std::shared_ptr<CharacterBase>owner, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible)
{
	auto laser = std::dynamic_pointer_cast<Laser>(Get(CharacterType::LASER, typeName));

	if (!laser) return nullptr;

	//ID���s
	int id = Singleton<LaserFactory>::get_instance().CreateID(typeName);
	if (!laser->IsInitialize()) {
		laser->Init(CharacterType::LASER, owner, typeName, id, localPosition, spawnWorldPosition, rotation, isVisible);
	}
	else {
		laser->Reset(owner, id, localPosition, spawnWorldPosition, rotation, isVisible);
	}

	//�A�N�e�B�u��Ԃ̃f�[�^�Ɋi�[,�A�N�e�B�u�ɐݒ�
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

	//�A�N�e�B�u��Ԃ̃f�[�^�Ɋi�[,�A�N�e�B�u�ɐݒ�
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

	// �ݐϊm�����v�Z
	std::vector<float> cumulativeWeights;
	float totalWeight = 0.0f;
	for (auto item : dropItemList) {
		totalWeight += item.dropWeight;
		cumulativeWeights.push_back(totalWeight);
	}
	
	// �����_����������������� 
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, totalWeight);
	
	// �����𐶐� 
	float randomValue = static_cast<float>(dis(gen));
	
	// �d�݂��g���ėv�f��I�� 
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

	//�v���C���[�����
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
	
	//�G�̍X�V
	for (auto itr = mActiveEnemies.begin(); itr != mActiveEnemies.end();) {
		if ((*itr)->IsActive()) {
			if(DEBUG_HELPER.IsDebugActor((*itr)->GetName())){
				bool isDebug = true;
			}
			(*itr)->SetOldWorldPosition2D((*itr)->GetWorldPosition2D());
			(*itr)->Update(deltaTime);

			//����ł���ꍇ�A�R���W�������X�g���珜���A�X�L�b�v
			if((*itr)->IsDead()){
				COLLISION_M.RemoveIndex((*itr)->GetGridIndex(), *itr);
				itr++;
				continue;
			}

			//���݂̃R���W�����̃O���b�h�X�V
			COLLISION_M.UpdateGridIndex((*itr));

			itr++;
		}else { 
			//����ł���ꍇ�A�A�C�e�����m���Ńh���b�v
			if((*itr)->IsDead()){
				auto itemName = ChoiceDropItem((*itr)->GetTypeName());
				if(itemName!="None"){
					auto item = GetItem(itemName.c_str(), (*itr)->GetLocalPosition2D(), (*itr)->GetWorldPosition2D(), (*itr)->GetRotation(), true);
					if (item) {
						//�e�A�C�e������������
						if (item->GetTypeName() == "LevelUpItem") {//�o���l
							std::static_pointer_cast<LevelUpItem>(item)->SetExp((*itr)->GetLevel(), (*itr)->GetTypeName());
						}
						if (item->GetTypeName() == "HealItem") {//�񕜃A�C�e��
							std::static_pointer_cast<HealItem>(item)->SetHeal((*itr)->GetLevel(), (*itr)->GetTypeName());
						}
					}
				}
				//�v���C���[�ɃX�R�A���Z
				STATUS.AddScore((int)(*itr)->GetScore());
				STATUS.AddCount((*itr)->GetActorType());
			}

			//�O�̂��߁A�R���W�����̌��ݍ��W����
			COLLISION_M.RemoveIndex((*itr)->GetGridIndex(), *itr);

			//�O�̂��߈ړ�
			(*itr)->SetWorldPosition2D(Vector2D<float>(0,0));

			// ��A�N�e�B�u�ɂȂ����G���v�[���ɖ߂�
			ReturnPool(*itr);
			itr = mActiveEnemies.erase(itr); 
		}
	}

	//�{�X�̍X�V
	if (mBoss && mIsSpawnBoss) {
		//�A�N�e�B�u�Ȃ�A���W�A�R���|�[�l���g�X�V
		if (mBoss->IsActive()) {
			mBoss->SetOldWorldPosition2D(mBoss->GetWorldPosition2D());
			mBoss->Update(deltaTime);
		}
		else {
			//����ł��邩
			if (mBoss->IsDead()) {
				//�v���C���[�ɃX�R�A���Z
				STATUS.AddScore((int)mBoss->GetScore());
				STATUS.AddCount(mBoss->GetActorType());
			}
			mIsSpawnBoss = false;
		}
	}

	//�Q�[����̃A�C�e���X�V
	for (auto itr = mActiveItems.begin(); itr != mActiveItems.end();) {
		if ((*itr)->IsActive()) {
			(*itr)->Update(deltaTime);//���W�X�V

			itr++;
		}else{
			//��A�N�e�B�u�Ȃ��p�̃f�[�^�Ɋi�[
			ReturnPool(*itr);
			itr = mActiveItems.erase(itr);
		}
	}

	//�Q�[����̒e�X�V
	for (auto itr = mActiveBullets.begin(); itr != mActiveBullets.end();) {
		if ((*itr)->IsActive()) {
			(*itr)->Update(deltaTime);//���W�X�V

			itr++;
		}
		else {
			//��A�N�e�B�u�Ȃ��p�̃f�[�^�Ɋi�[
			ReturnPool(*itr);
			itr = mActiveBullets.erase(itr);
		}
	}

	//�Q�[����̃��[�U�[�X�V
	for (auto itr = mActiveLasers.begin(); itr != mActiveLasers.end();) {
		if ((*itr)->IsActive()) {
			(*itr)->Update(deltaTime);//���W�X�V

			itr++;
		}
		else {
			//��A�N�e�B�u�Ȃ��p�̃f�[�^�Ɋi�[
			ReturnPool(*itr);
			itr = mActiveLasers.erase(itr);
		}
	}

	//�Q�[����̔��e�X�V
	for (auto itr = mActiveBombs.begin(); itr != mActiveBombs.end();) {
		if ((*itr)->IsActive()) {
			(*itr)->Update(deltaTime);//���W�X�V

			itr++;
		}
		else {
			//��A�N�e�B�u�Ȃ��p�̃f�[�^�Ɋi�[
			ReturnPool(*itr);
			itr = mActiveBombs.erase(itr);
		}
	}
}

void ActorManager::Draw(const float deltaTime)
{
	//�v���C���[�����
	if(mPlayer && mIsSpawnPlayer){
		if (mPlayer->IsActive()) {
			mPlayer->Draw(deltaTime);
		}
	}

	//�G�`��
	for (auto&e : mActiveEnemies) {
		if(e->IsActive()){
			e->Draw(deltaTime);
		}
	}

	//�{�X�̍X�V
	if (mBoss && mIsSpawnBoss) {
		if (mBoss->IsActive()) {
			mBoss->Draw(deltaTime);
		}
	}

	//�A�C�e���`��
	for(auto&i : mActiveItems){
		if(i->IsActive()){
			i->Draw(deltaTime);
		}
	}

	//�e�X�V
	for(auto &b : mActiveBullets){
		if(b->IsActive()){
			b->Draw(deltaTime);
		}
	}

	//���[�U�[�X�V
	for (auto& l : mActiveLasers) {
		if (l->IsActive()) {
			l->Draw(deltaTime);
		}
	}

	//���e�X�V
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


