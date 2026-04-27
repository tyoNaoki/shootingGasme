#pragma once
#include "NormalEnemyBase.h"

class NormalEnemyB :
    public NormalEnemyBase
{
public:
    NormalEnemyB();
    ~NormalEnemyB();
    //基礎データの初期化
    void Init(CharacterType ct, std::string name, int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    //コンポーネントや座標、衝突検知の更新
    void Update(const float deltaTime)override;
};

