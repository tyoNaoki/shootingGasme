#pragma once
#include <map>
#include <queue>
#include "Actor.h"

class ObjectPool
{
public:
	ObjectPool() = default;
	virtual ~ObjectPool() = default;

	virtual void OnCreate(const char* name, int count) = 0;
	
	std::shared_ptr<Actor>OnGet(const char* name);

	void OnRelease(const char* name, std::shared_ptr<Actor>obj);

protected:
	std::map<std::string, std::queue<std::shared_ptr<Actor>>>pool;
};

