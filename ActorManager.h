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

	//オブジェクトプールから取ってくる
	std::shared_ptr<Actor>Get(CharacterType ct,const char* typeName);

	//各キャラのスポーン処理
	std::shared_ptr<CharacterBase>GetPlayer(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<CharacterBase> GetEnemy(const char* typeName,const Vector2D<float>&localPosition,const Vector2D<float>&spawnWorldPosition,const float rotation, bool isVisible);
	std::shared_ptr<CharacterBase> GetBossEnemy(const std::string& typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<ItemBase> GetItem(const char* typeName, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<Bullet> GetBullet(const char* typeName,CharacterType ownerType,const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<Laser> GetLaser(const char* typeName, std::shared_ptr<CharacterBase>owner, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);
	std::shared_ptr<Bomb> GetBomb(const char* typeName, CharacterType ownerType, const Vector2D<float>& localPosition, const Vector2D<float>& spawnWorldPosition, const float rotation, bool isVisible);

	//ドロップアイテムを設定された中からランダムに抽選する
	std::string ChoiceDropItem(const std::string& enemyTypeName);

	//更新、描画
	void Update(const float deltaTime);
	void Draw(const float deltaTime);

	//現在アクティブのキャラの合計数
	int GetActorCount();
	//現在アクティブのアイテム（ヒールアイテム、経験値）の合計数
	int GetItemCount();
	//現在アクティブの弾丸の合計数
	int GetBulletCount();

	const std::vector<std::shared_ptr<CharacterBase>>GetActiveEnemies(){
		return mActiveEnemies;
	}

private:
	//アクティブから非アクティブになった各オブジェクトを非アクティブ専用のデータに格納する
	void ReturnPool(std::shared_ptr<Actor>obj);

	const char* NameToCharacterType(CharacterType ct);

	ActorManager();// 外部でのインスタンス作成は禁止
	~ActorManager();

	//プレイヤーとボスのみ一つの変数で管理
	std::shared_ptr<PlayerCharacter>mPlayer;
	std::shared_ptr<BossBase>mBoss;

	//アクティブ状態の各オブジェクトを格納するデータ
	std::vector<std::shared_ptr<CharacterBase>> mActiveEnemies;
	std::vector<std::shared_ptr<ItemBase>> mActiveItems;
	std::vector<std::shared_ptr<Bullet>>mActiveBullets;
	std::vector<std::shared_ptr<Laser>>mActiveLasers;
	std::vector<std::shared_ptr<Bomb>>mActiveBombs;

	//初期化フラグ
	bool mInitialize = false;

	//現在のレベル
	int mPlayerLevel = 1;
	int mEnemyLevel = 1;

	//プレイヤー、ボスのスポーンフラグ
	bool mIsSpawnPlayer = false;
	bool mIsSpawnBoss = false;
};

