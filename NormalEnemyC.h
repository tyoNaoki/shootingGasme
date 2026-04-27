#pragma once
#include "NormalEnemyBase.h"

class NormalEnemyC :
    public NormalEnemyBase
{
public:
    //デフォルトコンストラクタ削除
    NormalEnemyC();
    ~NormalEnemyC();
  //基礎データの初期化
    void Init(CharacterType ct, std::string name, int id, Vector2D<float>localPosition, Vector2D<float> worldPosition, float rotation, bool isVisible) override;
    //更新
    void Update(const float deltaTime)override;
};

