#pragma once
#include "UIBase.h"
#include <unordered_map>

class SelectUIRect;
class UIControllDescription;
class UISoundSetting;

// メニューパネルクラス
class MenuPanel : public UIBase {
public:
    //メニューUIやテキストなど各種作成処理
    MenuPanel(int width, int height);
    //UIの削除
    ~MenuPanel();

    // 更新関数
    virtual void Update(float deltaTime) override;
    // 描画関数
    virtual void Draw(float deltaTime) override;

private:
    // デフォルトコンストラクタを削除
    MenuPanel() = default;

    // UI要素追加関数
    void AddUIElement(std::shared_ptr<UIBase> ui);
    void AddUIElement(std::shared_ptr<SelectUIRect> ui);
    void AddRect(UI::Rect rect);

    // 操作説明を表示する関数
    void ViewControllDescription(bool IsOpen);
    // 音量設定を表示する関数
    void ViewSoundSetting(bool IsOpen);

    std::vector<std::shared_ptr<UIBase>> mUIList; // UI要素のリスト
    std::vector<UI::Rect> rects; // 矩形のリスト
    std::vector<std::shared_ptr<SelectUIRect>> mSelectRect; // 選択可能な矩形のリスト

    std::shared_ptr<UIControllDescription> mControllDescription; // コントロール説明オブジェクト
    std::shared_ptr<UISoundSetting> mSoundSetting; // サウンド設定オブジェクト

    int mSelectIndex = 0; // 選択インデックス
    int mFont; // フォント
};


