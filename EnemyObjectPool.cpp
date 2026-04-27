#include "EnemyObjectPool.h"
#include "EnemyFactory.h"


void EnemyObjectPool::Init()
{
	OnCreate("NormalEnemyA", enemyPoolSize);
	OnCreate("NormalEnemyB", enemyBPoolSize);
	OnCreate("NormalEnemyC", enemyCPoolSize);
}

void EnemyObjectPool::OnCreate(const char* enemyName,int count)
{
	for (int i=0;i<count;i++)
	{
		auto enemy = Singleton<EnemyFactory>::get_instance().CreateObject(enemyName);
		enemy->SetActive(false);
		pool[enemyName].push(enemy);
	}
}

std::shared_ptr<CharacterBase> EnemyObjectPool::OnGet(const char* enemyName)
{
	auto enemy = ObjectPool::OnGet(enemyName);
	if(!enemy) return nullptr;

	return std::static_pointer_cast<CharacterBase>(enemy);
}
