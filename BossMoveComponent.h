#pragma once
#include "Component.h"
#include "Vector2D.h"

class PlayerCharacter;
class Map;
class BossBase;
class CharacterBase;

class BossMoveComponent :
    public Component
{
public:
    BossMoveComponent(std::shared_ptr<CharacterBase>owner);

    void Update(const float deltaTime) override;

private:
    bool mIsMove =false;

    Vector2D<float> GetRandomMovePosition(const Vector2D<float>center,const float radius);
    void Move(float deltaTime, std::shared_ptr<BossBase>owner);

    bool IsArrived(std::shared_ptr<BossBase>owner)const;

    std::weak_ptr<BossBase>mBossOwner;

    std::shared_ptr<Map>mMap;

    Vector2D<float>mNextPosition;

    float mCurrentTime = 0.0f;
    float mWaitMaxTime = 3.0f;

    float mDistanceToArrived = 10.0f;
};

