#include "ObjectPoolManager.h"
#include "EnemyObjectPool.h"
#include "BulletObjectPool.h"
#include "ItemObjectPool.h"
#include "BombObjectPool.h"
#include "LaserObjectPool.h"

void ObjectPoolManager::Init()
{
	auto enemyPool = std::make_shared<EnemyObjectPool>();
	auto bulletPool = std::make_shared<BulletObjectPool>();
	auto itemPool = std::make_shared<ItemObjectPool>();
	auto bombPool = std::make_shared<BombObjectPool>();
	auto laserPool = std::make_shared<LaserObjectPool>();

	enemyPool->Init();
	bulletPool->Init();
	itemPool->Init();
	bombPool->Init();
	laserPool->Init();

	RegisterPool("Enemy", enemyPool);
	RegisterPool("Bullet", bulletPool);
	RegisterPool("Item",itemPool);
	RegisterPool("Bomb",bombPool);
	RegisterPool("Laser",laserPool);
}

std::shared_ptr<ObjectPool> ObjectPoolManager::GetPool(const std::string& poolName)
{
	auto it = pools.find(poolName);
	return (it != pools.end()) ? it->second : nullptr;
}

void ObjectPoolManager::RegisterPool(const std::string& poolName, std::shared_ptr<ObjectPool> pool)
{
	pools[poolName] = pool;
}


void ObjectPoolManager::ClearPool()
{
	pools.clear();
}

ObjectPoolManager::~ObjectPoolManager()
{
    ClearPool();
}
