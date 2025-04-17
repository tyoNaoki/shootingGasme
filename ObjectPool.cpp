#include "ObjectPool.h"
#include "Actor.h"

std::shared_ptr<Actor>ObjectPool::OnGet(const char* name)
{
	auto it = pool.find(name);

	if (it == pool.end()) return nullptr;

	if (it->second.empty())
	{
		OnCreate(name, 1);
	}

	auto obj = it->second.front();

	if (!obj) return nullptr;

	obj->SetActive(true);
	it->second.pop();

	return obj;
}

void ObjectPool::OnRelease(const char* name, std::shared_ptr<Actor> obj)
{
	auto it = pool.find(name);

	if (it == pool.end()) return;

	if (obj)
	{
		obj->SetActive(false);
		it->second.push(obj);
	}
}
