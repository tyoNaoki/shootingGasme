#pragma once
#include "ObjectPool.h"

class Bomb;

class BombObjectPool :
    public ObjectPool
{
public:
	void Init();

	void OnCreate(const char* name, int count) override;

	std::shared_ptr<Bomb>OnGet(const char* name);

private:
	const int bombPoolSize = 20;
};

