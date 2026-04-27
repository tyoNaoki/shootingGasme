#pragma once

#include "Singleton.h"
#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <functional>
#include <memory>
#include <iostream>
#include "ObjectPool.h"

class ObjectPoolManager
{
	friend class Singleton<ObjectPoolManager>;

public:
	void Init();

	std::shared_ptr<ObjectPool> GetPool(const std::string& poolName);
	
	void RegisterPool(const std::string& poolName, std::shared_ptr<ObjectPool> pool);

	void ClearPool();
private:
    std::map<std::string, std::shared_ptr<ObjectPool>> pools;

	ObjectPoolManager() = default;// 外部でのインスタンス作成は禁止
	~ObjectPoolManager();
};
