#include "BulletObjectPool.h"
#include "BulletFactory.h"

void BulletObjectPool::Init()
{
	OnCreate("PlayerBullet", playerbulletPoolSize);

	OnCreate("PlayerReflectionBullet", playerReflectionbulletPoolSize);

	OnCreate("EnemyBullet", bulletPoolSize);

	OnCreate("BossBullet", bossBulletPoolSize);
}

void BulletObjectPool::OnCreate(const char* name, int count)
{
	for (int i = 0; i < count; i++)
	{
		auto enemy = Singleton<BulletFactory>::get_instance().CreateObject(name);
		enemy->SetActive(false);
		pool[name].push(enemy);
	}
}

std::shared_ptr<Bullet> BulletObjectPool::OnGet(const char* name)
{
    auto bullet = OnGet(name);

	if(!bullet) return nullptr;

	return std::static_pointer_cast<Bullet>(bullet);
}
