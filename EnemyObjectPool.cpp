#include "EnemyObjectPool.h"
#include "EnemyFactory.h"


void EnemyObjectPool::Init()
{
	OnCreate("NormalEnemyA", enemyPoolSize);
	OnCreate("NormalEnemyB", enemyBPoolSize);
	OnCreate("NormalEnemyC", enemyCPoolSize);
}

void EnemyObjectPool::OnCreate(const char* enemyName,int count)
{
	for (int i=0;i<count;i++)
	{
		auto enemy = Singleton<EnemyFactory>::get_instance().CreateObject(enemyName);
		
		enemy->GetCollision()->SetCollisionCallback([weakEnemy = std::weak_ptr<Actor>(enemy)](std::shared_ptr<Actor>actor)
		{
			if(auto rect2 = actor->GetCollision<Collision::Rect>())
			{
				auto enemy = weakEnemy.lock();
				//押出処理
				auto rect1 = enemy->GetCollision<Collision::Rect>();
				Collision::PushBackRect(*rect1,*rect2);
				auto newPosi = rect1->mLeftTop + (rect1->mSize / 2);
				auto localPosi = SCENE_M.GetCurrentScene()->GetMap()->GetLocalFromWorldPosition(newPosi);

				//座標更新
				enemy->SetLocalPosition2D(localPosi);
				enemy->SetWorldPosition2D(newPosi);
			}
		});

		enemy->SetActive(false);
		pool[enemyName].push(enemy);
	}
}

std::shared_ptr<CharacterBase> EnemyObjectPool::OnGet(const char* enemyName)
{
	auto enemy = ObjectPool::OnGet(enemyName);
	if(!enemy) return nullptr;

	return std::static_pointer_cast<CharacterBase>(enemy);
}
