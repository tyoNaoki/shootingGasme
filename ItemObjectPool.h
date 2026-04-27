#pragma once
#include "ObjectPool.h"

class ItemBase;

class ItemObjectPool :
    public ObjectPool
{
public:
	void Init();

	void OnCreate(const char* name, int count) override;

	std::shared_ptr<ItemBase>OnGet(const char* name);

private:
	const int itemPoolSize = 200;
};

