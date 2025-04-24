#pragma once
#include "ObjectPool.h"

class Laser;

class LaserObjectPool :
    public ObjectPool
{
public:
	void Init();

	void OnCreate(const char* name, int count) override;

	std::shared_ptr<Laser>OnGet(const char* name);

private:
	const int laserPoolSize = 10;
};

