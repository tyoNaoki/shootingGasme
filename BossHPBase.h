#pragma once
#include "UIBase.h"
#include "CharacterBase.h"

class BossHPBase :
    public UIBase
{
public:
    BossHPBase(std::shared_ptr<CharacterBase> owner,int width,int height) :mOwner(owner),mWidth(width),mHeight(height) {}

    void Update(float deltaTime) override;

    void Draw(float deltaTime) override;

    void SetHP(float hp);

    void SetMaxHP(float maxHp);

    void SetSize(int width, int height);

    std::shared_ptr<CharacterBase> GetOwner();

private:
    std::weak_ptr<CharacterBase> mOwner;

    float mMaxHP = 0;
    float mHP = 0;
    int mWidth = 60;
    int mHeight = 15;
};

