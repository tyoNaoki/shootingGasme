#pragma once
#include "UIBase.h"
#include <vector>
#include <unordered_map>

class BossBase;

// HUDクラス
class HUD : public UIBase {
public:
    //ゲーム時間、HPバー、経験値バー作成処理
    HUD(const int width, const int height);
    // デストラクタ
    ~HUD();

    // 更新関数
    virtual void Update(float deltaTime) override;
    // 描画関数
    virtual void Draw(float deltaTime) override;
    // ボスHUD生成関数
    void SpawnBossHUD();

private:
    // デフォルトコンストラクタを削除
    HUD() = default;

    bool IsSpawnBossHUD = false;

    // UI要素取得関数
    std::shared_ptr<UIBase> GetElement(std::string name);
    // UI要素追加関数
    void AddUIElement(std::string name, std::shared_ptr<UIBase> ui);

    // UI要素削除関数
    void RemoveUIElement(std::string name);

    std::unordered_map<std::string, std::shared_ptr<UIBase>> mUIList; // UI要素のリスト
};




