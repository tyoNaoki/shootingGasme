#pragma once
#include "ObjectPool.h"

class Bullet;

class BulletObjectPool :
    public ObjectPool
{
public:
	void Init();

	void OnCreate(const char* name, int count) override;

	std::shared_ptr<Bullet>OnGet(const char* name);

private:
	const int playerbulletPoolSize = 100;
	const int playerReflectionbulletPoolSize = 50;
	const int bulletPoolSize = 200;
	const int bossBulletPoolSize = 10;
};

