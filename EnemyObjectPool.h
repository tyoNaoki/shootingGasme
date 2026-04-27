#pragma once
#include "ObjectPoolManager.h"
#include "ObjectPool.h"

class CharacterBase;

class EnemyObjectPool : public ObjectPool
{
public:
	void Init();

	void OnCreate(const char* name, int count) override;

	std::shared_ptr<CharacterBase>OnGet(const char* enemyName);
	
private:
	const int enemyPoolSize = 1000;
	const int enemyBPoolSize = 100;
	const int enemyCPoolSize = 10;
};

