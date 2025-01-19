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
	//初期化、終了、リセット処理
	bool Init();
	bool End();
	bool Reset();

	//プレイヤー、ボスの変数取得
	std::shared_ptr<PlayerCharacter>GetCurrentPlayer();
	std::shared_ptr<BossBase>GetCurrentBossEnemy();

	bool IsSpawnBoss();//ボスがスポーンしているか
	void PlayerLevelUp();//レベルアップ
	void PlayerWeaponLevelUp(std::string name);//武器の強化
	void PlayerTakeExp(float exp);//経験値取得
	void PlayerTakeHeal(float heal);//回復アイテム取得

	//キャラが作成されているかどうか
	bool ContainsEnemyTypeName(const std::string& typeName);
	bool ContainsItemTypeName(const std::string& typeName);
	bool ContainsBulletTypeName(const std::string& typeName);

	//各キャラのスポーン処理
	std::shared_ptr<CharacterBase>GetPlayer(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<CharacterBase> GetEnemy(const std::string& typeName,const Vector2D<float>&localPosition,const Vector2D<float>&spawnWorldPosition,const float rotation, bool isVisible);
	std::shared_ptr<CharacterBase> GetBossEnemy(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<ItemBase> GetItem(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<Bullet> GetBullet(const std::string& typeName,CharacterType ownerType,const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<Laser> GetLaser(const std::string& typeName, std::shared_ptr<CharacterBase>owner, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<Bomb> GetBomb(const std::string& typeName, CharacterType ownerType, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);

	//ドロップアイテムを設定された中からランダムに抽選する
	std::string ChoiceDropItem(const std::string& enemyTypeName);

	//更新、描画
	void Update(const float deltaTime);
	void Draw(const float deltaTime);

	//アクティブのオブジェクトを登録する
	bool RegistObject(std::shared_ptr<CharacterBase> chara);
	bool RegistObject(std::shared_ptr<ItemBase> item);
	bool RegistObject(std::shared_ptr<Bullet> bullet);
	bool RegistObject(std::shared_ptr<Laser> laser);
	bool RegistObject(std::shared_ptr<Bomb> bomb);

	//現在アクティブのキャラの合計数
	int GetActorCount();
	//現在アクティブのアイテム（ヒールアイテム、経験値）の合計数
	int GetItemCount();
	//現在アクティブの弾丸の合計数
	int GetBulletCount();
private:
	//各キャラのカテゴリごとに格納配列を作成する
	void initializePool(CharacterType ct,const std::string typeName,int size);

	//非アクティブのプレイヤーや敵のデータを取得
	std::shared_ptr<CharacterBase> getFromPool(CharacterType ct,std::string typeName,std::queue<std::shared_ptr<CharacterBase>>& pool,const Vector2D<float>& localPosition,const Vector2D<float>& spawnWorldPosition,const float rotation,bool isVisible);

	//非アクティブのアイテムのデータを取得
	std::shared_ptr<ItemBase> getFromPool(CharacterType ct, std::string typeName, std::queue<std::shared_ptr<ItemBase>>& pool, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);

	//非アクティブの弾丸のデータを取得
	std::shared_ptr<Bullet> getFromPool(CharacterType ct,CharacterType ownerType,std::string typeName, std::queue<std::shared_ptr<Bullet>>& pool, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);

	//非アクティブのレーザーのデータを取得
	std::shared_ptr<Laser> getFromPool(CharacterType ct, std::shared_ptr<CharacterBase> owner, std::string typeName, std::queue<std::shared_ptr<Laser>>& pool, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);

	//非アクティブの爆弾のデータを取得
	std::shared_ptr<Bomb> getFromPool(CharacterType ct, CharacterType ownerType, std::string typeName, std::queue<std::shared_ptr<Bomb>>& pool, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);

	//アクティブから非アクティブになった各オブジェクトを非アクティブ専用のデータに格納する
	void ReturnEnemy(std::shared_ptr<CharacterBase>enemy);
	void ReturnItem(std::shared_ptr<ItemBase>item);
	void ReturnBullet(std::shared_ptr<Bullet>bullet);
	void ReturnLaser(std::shared_ptr<Laser>laser);
	void ReturnBomb(std::shared_ptr<Bomb>bomb);

	ActorManager();// 外部でのインスタンス作成は禁止
	~ActorManager();

	//プレイヤーとボスのみ一つの変数で管理
	std::shared_ptr<PlayerCharacter>mPlayer;
	std::shared_ptr<BossBase>mBoss;

	//非アクティブの各オブジェクトを格納するデータ
	std::unordered_map<std::string, std::queue<std::shared_ptr<CharacterBase>>> mEnemyPools; 
	std::unordered_map<std::string, std::queue<std::shared_ptr<ItemBase>>> mItemPools;
	std::unordered_map<std::string, std::queue<std::shared_ptr<Bullet>>> mBulletPools;
	std::unordered_map<std::string, std::queue<std::shared_ptr<Laser>>> mLaserPools;
	std::unordered_map<std::string, std::queue<std::shared_ptr<Bomb>>> mBombPools;

	//アクティブ状態の各オブジェクトを格納するデータ
	std::vector<std::shared_ptr<CharacterBase>> mActiveEnemies;
	std::vector<std::shared_ptr<ItemBase>> mActiveItems;
	std::vector<std::shared_ptr<Bullet>>mActiveBullets;
	std::vector<std::shared_ptr<Laser>>mActiveLasers;
	std::vector<std::shared_ptr<Bomb>>mActiveBombs;

	//非アクティブデータの最大容量
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

	//初期化フラグ
	bool mInitialize = false;

	//現在のレベル
	int mPlayerLevel = 1;
	int mEnemyLevel = 1;

	//プレイヤー、ボスのスポーンフラグ
	bool mIsSpawnPlayer = false;
	bool mIsSpawnBoss = false;
};

