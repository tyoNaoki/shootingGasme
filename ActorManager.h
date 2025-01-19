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

#define ACTOR_M Singleton<ActorManager>::get_instance()

class BossBase;

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

	//�L�������쐬����Ă��邩�ǂ���
	bool ContainsEnemyTypeName(const std::string& typeName);
	bool ContainsItemTypeName(const std::string& typeName);
	bool ContainsBulletTypeName(const std::string& typeName);

	//�e�L�����̃X�|�[������
	std::shared_ptr<CharacterBase>GetPlayer(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<CharacterBase> GetEnemy(const std::string& typeName,const Vector2D<float>&localPosition,const Vector2D<float>&spawnWorldPosition,const float rotation, bool isVisible);
	std::shared_ptr<CharacterBase> GetBossEnemy(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<ItemBase> GetItem(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<Bullet> GetBullet(const std::string& typeName,CharacterType ownerType,const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<Laser> GetLaser(const std::string& typeName, std::shared_ptr<CharacterBase>owner, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<Bomb> GetBomb(const std::string& typeName, CharacterType ownerType, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);

	//�h���b�v�A�C�e����ݒ肳�ꂽ�����烉���_���ɒ��I����
	std::string ChoiceDropItem(const std::string& enemyTypeName);

	//�X�V�A�`��
	void Update(const float deltaTime);
	void Draw(const float deltaTime);

	//�A�N�e�B�u�̃I�u�W�F�N�g��o�^����
	bool RegistObject(std::shared_ptr<CharacterBase> chara);
	bool RegistObject(std::shared_ptr<ItemBase> item);
	bool RegistObject(std::shared_ptr<Bullet> bullet);
	bool RegistObject(std::shared_ptr<Laser> laser);
	bool RegistObject(std::shared_ptr<Bomb> bomb);

	//���݃A�N�e�B�u�̃L�����̍��v��
	int GetActorCount();
	//���݃A�N�e�B�u�̃A�C�e���i�q�[���A�C�e���A�o���l�j�̍��v��
	int GetItemCount();
	//���݃A�N�e�B�u�̒e�ۂ̍��v��
	int GetBulletCount();
private:
	//�e�L�����̃J�e�S�����ƂɊi�[�z����쐬����
	void initializePool(CharacterType ct,const std::string typeName,int size);

	//��A�N�e�B�u�̃v���C���[��G�̃f�[�^���擾
	std::shared_ptr<CharacterBase> getFromPool(CharacterType ct,std::string typeName,std::queue<std::shared_ptr<CharacterBase>>& pool,const Vector2D<float>& localPosition,const Vector2D<float>& spawnWorldPosition,const float rotation,bool isVisible);

	//��A�N�e�B�u�̃A�C�e���̃f�[�^���擾
	std::shared_ptr<ItemBase> getFromPool(CharacterType ct, std::string typeName, std::queue<std::shared_ptr<ItemBase>>& pool, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);

	//��A�N�e�B�u�̒e�ۂ̃f�[�^���擾
	std::shared_ptr<Bullet> getFromPool(CharacterType ct,CharacterType ownerType,std::string typeName, std::queue<std::shared_ptr<Bullet>>& pool, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);

	//��A�N�e�B�u�̃��[�U�[�̃f�[�^���擾
	std::shared_ptr<Laser> getFromPool(CharacterType ct, std::shared_ptr<CharacterBase> owner, std::string typeName, std::queue<std::shared_ptr<Laser>>& pool, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);

	//��A�N�e�B�u�̔��e�̃f�[�^���擾
	std::shared_ptr<Bomb> getFromPool(CharacterType ct, CharacterType ownerType, std::string typeName, std::queue<std::shared_ptr<Bomb>>& pool, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);

	//�A�N�e�B�u�����A�N�e�B�u�ɂȂ����e�I�u�W�F�N�g���A�N�e�B�u��p�̃f�[�^�Ɋi�[����
	void ReturnEnemy(std::shared_ptr<CharacterBase>enemy);
	void ReturnItem(std::shared_ptr<ItemBase>item);
	void ReturnBullet(std::shared_ptr<Bullet>bullet);
	void ReturnLaser(std::shared_ptr<Laser>laser);
	void ReturnBomb(std::shared_ptr<Bomb>bomb);

	ActorManager();// �O���ł̃C���X�^���X�쐬�͋֎~
	~ActorManager();

	//�v���C���[�ƃ{�X�݈̂�̕ϐ��ŊǗ�
	std::shared_ptr<PlayerCharacter>mPlayer;
	std::shared_ptr<BossBase>mBoss;

	//��A�N�e�B�u�̊e�I�u�W�F�N�g���i�[����f�[�^
	std::unordered_map<std::string, std::queue<std::shared_ptr<CharacterBase>>> mEnemyPools; 
	std::unordered_map<std::string, std::queue<std::shared_ptr<ItemBase>>> mItemPools;
	std::unordered_map<std::string, std::queue<std::shared_ptr<Bullet>>> mBulletPools;
	std::unordered_map<std::string, std::queue<std::shared_ptr<Laser>>> mLaserPools;
	std::unordered_map<std::string, std::queue<std::shared_ptr<Bomb>>> mBombPools;

	//�A�N�e�B�u��Ԃ̊e�I�u�W�F�N�g���i�[����f�[�^
	std::vector<std::shared_ptr<CharacterBase>> mActiveEnemies;
	std::vector<std::shared_ptr<ItemBase>> mActiveItems;
	std::vector<std::shared_ptr<Bullet>>mActiveBullets;
	std::vector<std::shared_ptr<Laser>>mActiveLasers;
	std::vector<std::shared_ptr<Bomb>>mActiveBombs;

	//��A�N�e�B�u�f�[�^�̍ő�e��
	const int enemyPoolSize = 1000;
	const int enemyBPoolSize = 100;
	const int enemyCPoolSize = 10;
	const int itemPoolSize = 200;
	const int playerbulletPoolSize = 100;
	const int playerReflectionbulletPoolSize = 50;
	const int bulletPoolSize = 200;
	const int laserPoolSize = 10;
	const int bombPoolSize = 20;
	const int bossBulletPoolSize = 10;

	//�������t���O
	bool mInitialize = false;

	//���݂̃��x��
	int mPlayerLevel = 1;
	int mEnemyLevel = 1;

	//�v���C���[�A�{�X�̃X�|�[���t���O
	bool mIsSpawnPlayer = false;
	bool mIsSpawnBoss = false;
};

