#pragma once
#include "BossBase.h"

class BossEnemyA :
    public BossBase
{
public:
    BossEnemyA() = delete;
    BossEnemyA(Collision::Rect rect);
    ~BossEnemyA() = default;

    void Init(CharacterType ct, std::string name, int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;

    void Update(float deltaTime)override;

    void FinishState()override;

private:
    bool IsChooseLeft(float leftWeight);

    std::shared_ptr<Component>mAttackComponent;
};

