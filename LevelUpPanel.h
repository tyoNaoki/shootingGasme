#pragma once
#include "UIBase.h"

class UILevelUpReward;
class SelectUIRect;
class LevelUpBonus;
class StatusInfoPanel;

// レベルアップパネルクラス
class LevelUpPanel : public UIBase {
public:
    // コンストラクタ：報酬選択パネル、ステータス表示パネル、テキストなどの作成処理
    LevelUpPanel(const int width, const int height, const int rewardMaxCount);
    // デストラクタ
    ~LevelUpPanel() {};

    // ステータス情報パネル設定関数
    void SetStatusInfoPanel();
    // レベルアップ報酬設定関数
    void SetLevelUpReward(const std::vector<std::shared_ptr<LevelUpBonus>>& rewards);
    // 更新関数
    void Update(float deltaTime) override;
    // 描画関数
    void Draw(float deltaTime) override;

private:
    // デフォルトコンストラクタを削除
    LevelUpPanel() = default;
    // 報酬選択関数
    void SelectedReward() const;
    // UI要素追加関数
    void AddUIElement(std::shared_ptr<UIBase> ui);

    std::vector<std::shared_ptr<UIBase>> mUIList; // UI要素のリスト

    Vector2D<int> mCenter; // 中心位置
    Vector2D<int> mTextPosi; // テキスト位置
    Vector2D<int> mSelectRectSize; // 選択項目サイズ
    Vector2D<int> mLevelUpPanelSize; // レベルアップパネルサイズ

    std::shared_ptr<StatusInfoPanel> mStatusInfoPanel; // ステータス情報パネル
    std::vector<std::shared_ptr<UILevelUpReward>> mRewards; // レベルアップ報酬

    int rewardCount = 0; // 報酬数
    int selectIndex = 0; // 選択インデックス
    int mFont; // フォント
};


