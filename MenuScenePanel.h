#pragma once
#include "UIBase.h"

class SelectUIRect;
class UIControllDescription;
class UICredit;

// メニューシーンのパネルクラス
class MenuScenePanel : public UIBase {
public:
    // コンストラクタ：幅と高さを初期化
    MenuScenePanel(int width, int height);
    // デストラクタ
    ~MenuScenePanel();

    // 更新関数
    virtual void Update(float deltaTime) override;
    // 描画関数
    virtual void Draw(float deltaTime) override;

private:
    // デフォルトコンストラクタを削除
    MenuScenePanel() = default;

    // UI要素追加関数
    void AddUIElement(std::shared_ptr<UIBase> ui);
    void AddUIElement(std::shared_ptr<SelectUIRect> ui);
    void AddRect(UI::Rect rect);

    std::vector<std::shared_ptr<UIBase>> mUIList; // UI要素のリスト

    //操作説明を表示する関数
    void ViewControllDescription(bool IsOpen);
    // クレジットを表示する関数
    void ViewCredit(bool IsOpen);

    std::vector<UI::Rect> rects; // 項目の枠組みリスト
    std::vector<std::shared_ptr<SelectUIRect>> mSelectRect; // 選択可能な項目のリスト

    std::shared_ptr<UIControllDescription> mControllDescription; // 操作説明
    std::shared_ptr<UICredit> mCredit; // クレジット

    int mSelectIndex = 0; // 選択インデックス
    int mFont; // フォント
};


