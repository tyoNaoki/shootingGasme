#pragma once
#include "CharacterFactory.h"

class PlayerCharacter;

// プレイヤーキャラクターを生成するファクトリクラス
class PlayerFactory : public CharacterFactory {
    friend class Singleton<PlayerFactory>;

public:
    // オブジェクト生成関数
    std::shared_ptr<CharacterBase> CreateObject(std::string name) override;
    // ID生成関数
    int CreateID(std::string name) override;

private:
    PlayerFactory() = default; // コンストラクタ
    ~PlayerFactory() = default; // デストラクタ

    int mChara_Index = 0; // キャラクターインデックス
};


