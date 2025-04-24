#pragma once
#include "Singleton.h"
#include <unordered_map> 
#include <queue>
#include <vector>
#include "Vector2D.h"
#include "ItemBase.h"
#include "Bullet.h"
#include "Bomb.h"
#include "Laser.h"
#include "Actor.h"
#include "CharacterBase.h"
#include "BossBase.h"
#include "CharacterFactory.h"
#include "PlayerFactory.h"
#include "EnemyObjectPool.h"
#include "BulletObjectPool.h"

#define ACTOR_M Singleton<ActorManager>::get_instance()

class BossBase;
class ActorFactory;

class ActorManager
{
friend class Singleton<ActorManager>;

public:
	//�������A�I���A���Z�b�g����
	bool Init();
	bool End();
	bool Reset();

	//�v���C���[�A�{�X�̕ϐ��擾
	std::shared_ptr<PlayerCharacter>GetCurrentPlayer();
	std::shared_ptr<BossBase>GetCurrentBossEnemy();

	bool IsSpawnBoss();//�{�X���X�|�[�����Ă��邩
	void PlayerLevelUp();//���x���A�b�v
	void PlayerWeaponLevelUp(std::string name);//����̋���
	void PlayerTakeExp(float exp);//�o���l�擾
	void PlayerTakeHeal(float heal);//�񕜃A�C�e���擾

	//�I�u�W�F�N�g�v�[���������Ă���
	std::shared_ptr<Actor>Get(CharacterType ct,const char* typeName);

	//�e�L�����̃X�|�[������
	std::shared_ptr<CharacterBase>GetPlayer(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<CharacterBase> GetEnemy(const char* typeName,const Vector2D<float>&localPosition,const Vector2D<float>&spawnWorldPosition,const float rotation, bool isVisible);
	std::shared_ptr<CharacterBase> GetBossEnemy(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<ItemBase> GetItem(const char* typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<Bullet> GetBullet(const char* typeName,CharacterType ownerType,const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<Laser> GetLaser(const char* typeName, std::shared_ptr<CharacterBase>owner, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<Bomb> GetBomb(const char* typeName, CharacterType ownerType, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);

	//�h���b�v�A�C�e����ݒ肳�ꂽ�����烉���_���ɒ��I����
	std::string ChoiceDropItem(const std::string& enemyTypeName);

	//�X�V�A�`��
	void Update(const float deltaTime);
	void Draw(const float deltaTime);

	//���݃A�N�e�B�u�̃L�����̍��v��
	int GetActorCount();
	//���݃A�N�e�B�u�̃A�C�e���i�q�[���A�C�e���A�o���l�j�̍��v��
	int GetItemCount();
	//���݃A�N�e�B�u�̒e�ۂ̍��v��
	int GetBulletCount();

	const std::vector<std::shared_ptr<CharacterBase>>GetActiveEnemies(){
		return mActiveEnemies;
	}

private:
	//�A�N�e�B�u�����A�N�e�B�u�ɂȂ����e�I�u�W�F�N�g���A�N�e�B�u��p�̃f�[�^�Ɋi�[����
	void ReturnPool(std::shared_ptr<Actor>obj);

	const char* NameToCharacterType(CharacterType ct);

	ActorManager();// �O���ł̃C���X�^���X�쐬�͋֎~
	~ActorManager();

	//�v���C���[�ƃ{�X�݈̂�̕ϐ��ŊǗ�
	std::shared_ptr<PlayerCharacter>mPlayer;
	std::shared_ptr<BossBase>mBoss;

	//�A�N�e�B�u��Ԃ̊e�I�u�W�F�N�g���i�[����f�[�^
	std::vector<std::shared_ptr<CharacterBase>> mActiveEnemies;
	std::vector<std::shared_ptr<ItemBase>> mActiveItems;
	std::vector<std::shared_ptr<Bullet>>mActiveBullets;
	std::vector<std::shared_ptr<Laser>>mActiveLasers;
	std::vector<std::shared_ptr<Bomb>>mActiveBombs;

	//�������t���O
	bool mInitialize = false;

	//���݂̃��x��
	int mPlayerLevel = 1;
	int mEnemyLevel = 1;

	//�v���C���[�A�{�X�̃X�|�[���t���O
	bool mIsSpawnPlayer = false;
	bool mIsSpawnBoss = false;
};

