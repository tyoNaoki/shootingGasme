#pragma once
#include "Scene.h"

// メニューシーンのクラス
class MenuScene : public Scene {
public:
    // コンストラクタ
    MenuScene();

    // デストラクタ
    ~MenuScene();

    // 初期化関数
    bool Initialize() override;

    // 更新関数
    void Update(const float deltaTime) override;

    // 描画関数
    void Render(const float deltaTime) override;

    // マップ取得関数
    std::shared_ptr<Map> GetMap() const override;

    // 終了処理関数
    void End() override;

private:
    int mSelectIndex = 0; // 選択インデックス
    bool mInitialze = false; // 初期化フラグ
    int mHandle = -1; // ハンドル
};


