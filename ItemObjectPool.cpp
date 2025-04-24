#include "ItemObjectPool.h"
#include "ItemFactory.h"

void ItemObjectPool::Init()
{
	OnCreate("LevelUpItem", itemPoolSize);

	OnCreate("HealItem", itemPoolSize);
}

void ItemObjectPool::OnCreate(const char* name, int count)
{
	for (int i = 0; i < count; i++)
	{
		auto enemy = Singleton<ItemFactory>::get_instance().CreateObject(name);
		enemy->SetActive(false);
		pool[name].push(enemy);
	}
}

std::shared_ptr<ItemBase> ItemObjectPool::OnGet(const char* name)
{
	auto item = OnGet(name);

	if (!item) return nullptr;

	return std::static_pointer_cast<ItemBase>(item);
}
