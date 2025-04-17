#include "BombObjectPool.h"
#include "BombFactory.h"

void BombObjectPool::Init()
{
	OnCreate("PlayerBomb", bombPoolSize);
}

void BombObjectPool::OnCreate(const char* name, int count)
{
	for (int i = 0; i < count; i++)
	{
		auto bomb = Singleton<BombFactory>::get_instance().CreateObject(name);
		bomb->SetActive(false);
		pool[name].push(bomb);
	}
}

std::shared_ptr<Bomb> BombObjectPool::OnGet(const char* name)
{
	auto bomb = OnGet(name);

	if (!bomb) return nullptr;

	return std::static_pointer_cast<Bomb>(bomb);
}
