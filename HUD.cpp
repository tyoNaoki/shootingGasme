#include "HUD.h"
#include "UITime.h"
#include "HPBase.h"
#include "BossHPBase.h"
#include "UIExperienceBar.h"
#include "UIText.h"
#include "UILevelText.h"
#include "UIScoreText.h"
#include "PlayerCharacter.h"
#include "ActorManager.h"
#include "StatusManager.h"

HUD::HUD(const int width,const int height)
{
	//ゲーム時間、HPバー、経験値バー作成
	auto gameTime = std::make_shared<UITime>(CreateFontToHandle("Arial", 30, -1, DX_FONTTYPE_ANTIALIASING), UI::white,true);
	gameTime->SetLocalPosition(Vector2D<float>(width / 2 - 40, 40));
	auto hpBar = std::make_shared<HPBase>(ACTOR_M.GetCurrentPlayer());
	auto experienceBar = std::make_shared<UIExperienceBar>(width,30,0,width,UI::blue,UI::white);
	experienceBar->SetLocalPosition(Vector2D<float>(0, height - 30));

	//現在のレベル表記
	auto levelText = std::make_shared<UILevelText>("Lv0",CreateFontToHandle("ＭＳゴシック",40, 2), UI::white);
	levelText->SetLocalPosition(Vector2D<float>(0 + 30, height - 80));

	//現在のスコア表記
	auto scoreText = std::make_shared<UIScoreText>(CreateFontToHandle("ＭＳゴシック", 25, 2), UI::white);
	scoreText->SetLocalPosition(Vector2D<float>(width-170, 40));

	AddUIElement("GameTime",gameTime);
	AddUIElement("HpBar", hpBar);
	AddUIElement("ExperienceBar", experienceBar);
	AddUIElement("LevelText",levelText);
	AddUIElement("ScoreText", scoreText);
}

HUD::~HUD()
{
	mUIList.clear();
}

void HUD::Update(float deltaTime)
{
	if (!IsVisible()) { return; }

	if(IsSpawnBossHUD){
		auto boss = ACTOR_M.GetCurrentBossEnemy();
		if(!boss||boss->IsDead()){
			RemoveUIElement("BossHPBar");
			RemoveUIElement("BossText");
		}
	}

	for (auto& x : mUIList) {
		x.second->Update(deltaTime);
	}
}

void HUD::Draw(float deltaTime)
{
	if (!IsVisible()) { return; }

	for (auto& x : mUIList) {
		x.second->Draw(deltaTime);
	}
}

std::shared_ptr<UIBase> HUD::GetElement(std::string name)
{
	auto it = mUIList.find(name);
	if(it!=mUIList.end()){
		return it->second;
	}

	return nullptr;
}

void HUD::SpawnBossHUD()
{
	//画面サイズ取得
	int width,height;
	GetWindowSize(&width,&height);
	
	//HPの場所を設定
	auto boss = ACTOR_M.GetCurrentBossEnemy();
	auto hpBar = std::make_shared<BossHPBase>(boss,width-100,30);
	hpBar->SetLocalPosition(Vector2D<float>(100,1));
	//ボス名前表示
	auto bossText = std::make_shared<UIText>(STATUS.GetCurrentBossName(), CreateFontToHandle("ＭＳゴシック", 25, 2), UI::white);
	bossText->SetLocalPosition(Vector2D<float>(10,1));

	AddUIElement("BossHPBar", hpBar);
	AddUIElement("BossText", bossText);
}

void HUD::AddUIElement(std::string name,std::shared_ptr<UIBase> ui)
{
	mUIList[name] = ui;
}

void HUD::RemoveUIElement(std::string name)
{
	auto itr = mUIList.find(name);
	if(itr!=mUIList.end()){
		mUIList.erase(itr);
	}
}
