#include "GameScene.h"
#include "Game.h"
#include "ActorManager.h"
#include "PlayerFactory.h"
#include "PlayerCharacter.h"
#include "DebugHelper.h"
#include "EnemyFactory.h"
#include "CharacterBase.h"
#include "CollisionManager.h"
#include "EffectManager.h"
#include "UIManager.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "Map.h"
#include "Actor.h"
#include "BossEnemyA.h"

GameScene::GameScene(Vector2D<int>mapWorldSize,Vector2D<float>cell) : mCellSize(cell)
{
	mSceneName = "Game";
	mMapSize = mapWorldSize/mCellSize;
	mGameMap = std::make_shared<Map>(mMapSize, mCellSize);
}

GameScene::~GameScene()
{

	//仮として一旦消去,後で
	//Singleton<ActorManager>::get_instance().Reset();
	
	//各キャラごとの作成クラス消去
	mGameMap = nullptr;
}

bool GameScene::Initialize()
{
	if (mInitialze){
		return true;
	}

	mCurrentGameTime = 0.0f;

	//プレイヤー
	STATUS.ResetPlayerStatus(mPlayerStartLevel);
	STATUS.ResetWeaponStatus("PlayerMeleeComponent");
	STATUS.ResetWeaponStatus("PlayerGunComponent");
	STATUS.ResetWeaponStatus("PlayerReflectionGunComponent");
	STATUS.ResetWeaponStatus("PlayerThrowBombComponent");

	//敵
	STATUS.ResetEnemyStatus(mEnemyStartLevel,mSpawnEnemyName);
	STATUS.ResetEnemyStatus(mEnemyStartLevel, mSpawnEnemyNameB);
	STATUS.ResetEnemyStatus(mEnemyStartLevel, mSpawnEnemyNameC);
	STATUS.ResetBossStatus();
	STATUS.ResetWeaponStatus("NormalEnemyAttackComponent");
	STATUS.ResetWeaponStatus("NormalEnemyMeleeComponent");

	//報酬リセット
	STATUS.ResetRewardS();
	STATUS.ResetSurvivalBonus();

	//スコア関係リセット
	STATUS.ResetScore();
	STATUS.ResetCount();

	//各キャラごとのクラス生成

	//初期位置をマップの中央に設定
	auto startCenter = (mGameMap->GetMapSize() * mGameMap->GetCellSize())/2.0;
	mGameMap->Initialize(startCenter);

	//衝突判定のためにグリッドの分割サイズ設定
	Singleton<CollisionManager>::get_instance().Initialize(50,mGameMap);

	int height,width;
	GetWindowSize(&width,&height);

	//プレイヤーの初期化処理
	auto localStart = Vector2D<float>(static_cast<float>(width / 2), static_cast<float>(height / 2));
	auto player = ACTOR_M.GetPlayer("PlayerCharacter", localStart, startCenter, 90, true);
	//ActorManagerにプレイヤーを追加
	if (!ACTOR_M.RegistObject(player)) {
		return false;
	}

	auto& dh = DEBUG_HELPER;

	//std::string message = "spawnPosition is " + enemyStart.ToString();
	//dh.Add(message, 5.0f);
	
	//敵のスポーン & 登録
	//NormaLEnemyA
	for(int i = 0;i<10;i++){
		Vector2D<float>enemyStart = Singleton<EnemyFactory>::get_instance().GetRandomSpawnPosition(0, mGameMap);
		auto enemy = ACTOR_M.GetEnemy(mSpawnEnemyName, mGameMap->GetLocalFromWorldPosition(enemyStart), enemyStart, 90, true);
		
		if (!ACTOR_M.RegistObject(enemy)) {
			return false;
		}
	}

	//debug
	Vector2D<float>enemyCStart = Singleton<EnemyFactory>::get_instance().GetRandomSpawnPosition(0, mGameMap);
	auto enemyC = Singleton<ActorManager>::get_instance().GetEnemy(mSpawnEnemyNameC, mGameMap->GetLocalFromWorldPosition(enemyCStart), enemyCStart, 90, true);

	if (!Singleton<ActorManager>::get_instance().RegistObject(enemyC)) {
		return false;
	}

	//デバック用のマップ設定
	Singleton<DebugHelper>::get_instance().SetMap(mGameMap);

	Singleton<EffectManager>::get_instance().Initialize();

	//UI設定
	//
	//ゲームオーバー
	auto gameOverCanvas = std::make_shared<Canvas>();
	auto gameOverUI = std::make_shared<GameOverPanel>(width,height);
	gameOverCanvas->AddElement(gameOverUI);
	gameOverCanvas->SetVisibilty(false);
	UI_M.AddCanvas("GameOver",gameOverCanvas);

	//ゲームクリア
	auto gameClearCanvas = std::make_shared<Canvas>();
	auto gameClearUI = std::make_shared<GameClearPanel>(width,height);
	gameClearCanvas->AddElement(gameClearUI);
	gameClearCanvas->SetVisibilty(false);
	UI_M.AddCanvas("GameClear", gameClearCanvas);

	//HUD
	auto hudCanvas = std::make_shared<Canvas>();
	auto hud = std::make_shared<HUD>(width,height);
	hudCanvas->AddElement(hud);
	hudCanvas->SetVisibilty(true);
	UI_M.AddCanvas("HUD", hudCanvas);

	//ポーズ画面
	auto pauseCanvas = std::make_shared<Canvas>();
	auto menuPanel = std::make_shared<MenuPanel>(width,height);
	pauseCanvas->AddElement(menuPanel);
	pauseCanvas->SetVisibilty(false);
	UI_M.AddCanvas("PauseMenu", pauseCanvas);
	
	//レベルアップ画面
	auto levelUpCanvas = std::make_shared<Canvas>();
	auto levelUpPanel = std::make_shared<LevelUpPanel>(width, height,STATUS.GetRewardMaxCount());

	//一旦確認用に報酬セット
	//levelUpPanel->SetLevelUpReward(STATUS.GetRandomSelectRewards());

	levelUpCanvas->AddElement(levelUpPanel);
	levelUpCanvas->SetVisibilty(false);
	UI_M.AddCanvas("LevelUpMenu", levelUpCanvas);

	//重要イベント設定
	//ゲームオーバーイベントセット
	EVENT_M.addEvent([this](){
		this->GameOver();
	},[this](){
		return Singleton<ActorManager>::get_instance().GetCurrentPlayer()->GetHP()<=0.0f;
	});

	//ゲームクリアイベントセット
	EVENT_M.addEvent([this]() {
		this->GameClear();
		}, [this]() {
			//300秒後にゲームクリア
			return mCurrentGameTime >= mGameClearTime;
		});

	//敵が設定した秒数毎にレベルアップ
	EVENT_M.addEvent([this](){
		if (STATUS.GetEnemyMaxLevel(mSpawnEnemyName) > STATUS.GetCurrentEnemyStatus(mSpawnEnemyName).level){
			STATUS.UpdateEnemyStatus(mSpawnEnemyName);
			STATUS.UpdateEnemyStatus(mSpawnEnemyNameB);
			STATUS.UpdateEnemyWeaponStatus("NormalEnemyAttackComponent",mSpawnEnemyName);
			mCurrentSpawnEnemyCount+=mLevelUpSpawnCount;
		}
	}, [this]() {
		int integerValue = static_cast<int>(std::floor(mCurrentGameTime));
		if (integerValue != 0&&integerValue % mEnemyLevelUpTime == 0 && integerValue != mPreLevelUpTime) {
			mPreLevelUpTime = integerValue;
			return true;
		}
		return false;
	},0.0f,true);

	//中ボスが設定した秒数毎にスポーン
	EVENT_M.addEvent([this]() {
		if (mEnemyCspawnIndex <= 2) {
			Vector2D<float>enemyStart = Singleton<EnemyFactory>::get_instance().GetRandomSpawnPosition(0, mGameMap);
			auto enemy = Singleton<ActorManager>::get_instance().GetEnemy(mSpawnEnemyNameC, mGameMap->GetLocalFromWorldPosition(enemyStart), enemyStart, 90, true);

			if (!Singleton<ActorManager>::get_instance().RegistObject(enemy)) {
				return;
			}
		}else{
			for(int i =0;i<3;i++){
				Vector2D<float>enemyStart = Singleton<EnemyFactory>::get_instance().GetRandomSpawnPosition(0, mGameMap);
				auto enemy = Singleton<ActorManager>::get_instance().GetEnemy(mSpawnEnemyNameC, mGameMap->GetLocalFromWorldPosition(enemyStart), enemyStart, 90, true);

				if (!Singleton<ActorManager>::get_instance().RegistObject(enemy)) {
					return;
				}
			}
		}
		}, [this]() {
			//180秒後に3体
			if (mCurrentGameTime >= 180 && mEnemyCspawnIndex < 3) {
				mEnemyCspawnIndex = 3;
				return true;
			}
			//120秒後に１体
			if (mCurrentGameTime >= 120 && mEnemyCspawnIndex < 2) {
				mEnemyCspawnIndex = 2;
				return true;
			}
			//60秒後に１体
			if (mCurrentGameTime>=60 && mEnemyCspawnIndex < 1) {
				mEnemyCspawnIndex = 1;
				return true;
			}
			return false;
		}, 0.0f, true);

	//一定時間後に設定した秒数毎に経験値入手量アップ
	EVENT_M.addEvent([this]() {
		STATUS.UpdateSurvivalBonus();
	}, [this]() {
		if(mCurrentGameTime<mStartLevelUpBonusTime){
			return false;
		}
		int integerValue = static_cast<int>(std::floor(mCurrentGameTime));
		if (integerValue != 0 && integerValue % mDefaultBonusTime == 0 && integerValue != mPreLevelUpBonusTime) {
			mPreLevelUpBonusTime = integerValue;
			return true;
		}
		return false;
	},0.0f, true);

	//ボス登場
	EVENT_M.addEvent([this]() {
		this->SpawnBoss();
		}, [this]() {
			return mCurrentGameTime >= mGameBossSpawnTime;
	});

	//サウンドマネージャー初期化
	SOUND_M.Init();

	//バックグラウンドミュージック
	if (SOUND_M.PlayBGMOnSound("BGM") == -1) {
		return false;
	}

	//エフェクトロード
	EFFECT_M.LoadEffects();

	//ゲームスタート
	ChangeScene(SceneState::PLAYING);

	//各変数初期化
	mCurrentEnemySpawnTime = mDefaultSpawnTime;
	mCurrentSpawnEnemyCount = mDefaultSpawnCount;
	mCurrentSpawnEnemyBToSpawnCount = mSpawnEnemyBToSpawnCount;
	mGameOver = false;
	mGameClear = false;
	mInitialze = true;
	return true;
}

void GameScene::Update(const float deltaTime)
{
	if(!mInitialze){return;}

	EVENT_M.Update(deltaTime);//イベント条件判定

	UI_M.Update(deltaTime);//UI更新

	//ゲームオーバーかクリアしているか
	if (mGameOver || mGameClear){
		return;
	}

	//ポーズ
	if (IsPaused()) {
		UI_M.SetVisibiltyToCanvas("PauseMenu", true);
		if(IsLevelUp()){
			UI_M.SetVisibiltyToCanvas("LevelUpMenu", false);
		}
		return;
	}else { //ポーズを閉じる
		UI_M.SetVisibiltyToCanvas("PauseMenu", false);
	}

	//レベルアップ画面で報酬選択中
	if(IsLevelUp()){
		UI_M.SetVisibiltyToCanvas("LevelUpMenu", true);
		return;
	}else{//選択終了
		UI_M.SetVisibiltyToCanvas("LevelUpMenu", false);
	}
	
	//雑魚敵スポーン処理
	ActorManager& am = ACTOR_M;
	if (am.GetActorCount() < mMaxSpawnEnemyCount && mCurrentNextSpawnTime<=0.0f){
		for(int i=0;i<mCurrentSpawnEnemyCount;i++){

			//敵Aのスポーン
			Vector2D<float>enemyStart = Singleton<EnemyFactory>::get_instance().GetRandomSpawnPosition(20, mGameMap);
			auto enemy = Singleton<ActorManager>::get_instance().GetEnemy(mSpawnEnemyName, mGameMap->GetLocalFromWorldPosition(enemyStart), enemyStart, 90, true);

			if (!Singleton<ActorManager>::get_instance().RegistObject(enemy)) {
				DEBUG_HELPER.Add("enemyA spawn faild", 1.0f, GetColor(200, 0, 0));
			}

			mCurrentSpawnEnemyBToSpawnCount--;

			if(mCurrentSpawnEnemyBToSpawnCount>0){
				continue;
			}

			for(int j = 0;j<mMaxSpawnEnemyBCount;j++){
				//敵Bのスポーン
				enemyStart = Singleton<EnemyFactory>::get_instance().GetRandomSpawnPosition(0, mGameMap);
				enemy = Singleton<ActorManager>::get_instance().GetEnemy(mSpawnEnemyNameB, mGameMap->GetLocalFromWorldPosition(enemyStart), enemyStart, 90, true);

				if (!Singleton<ActorManager>::get_instance().RegistObject(enemy)) {
					DEBUG_HELPER.Add("enemyB spawn faild", 1.0f, GetColor(200, 0, 0));
				}
			}
			
			mCurrentSpawnEnemyBToSpawnCount = mSpawnEnemyBToSpawnCount;
		}

		mCurrentNextSpawnTime = mCurrentEnemySpawnTime;
	}else{
		mCurrentNextSpawnTime-=deltaTime;
	}
	//デバッグ関係
	DEBUG_HELPER.Add("PlayerLevel is " + std::to_string(STATUS.GetCurrentPlayerStatus().level));
	DEBUG_HELPER.Add("EnemyLevel is " + std::to_string(STATUS.GetCurrentEnemyStatus("NormalEnemyA").level));
	auto debugHp = ACTOR_M.GetCurrentPlayer()->GetHP();
	DEBUG_HELPER.Add("PlayerCurrentHP is " + std::to_string(debugHp));
	debugHp = STATUS.GetCurrentEnemyStatus("NormalEnemyA").hp;
	DEBUG_HELPER.Add("EnemyHP is " + std::to_string(debugHp));

	//各オブジェクトの座標や各コンポーネントの更新
	am.Update(deltaTime); 

	//ゲーム内時間更新
	mCurrentGameTime += deltaTime; 

	//エフェクト画像更新
	EFFECT_M.Update(deltaTime);
}

void GameScene::Render(const float deltaTime)
{
	mGameMap->Draw(); //マップの各セルの描画

	ACTOR_M.Draw(deltaTime);//オブジェクトの描画
	EFFECT_M.Draw(deltaTime);//エフェクトの描画
	UI_M.Draw(deltaTime);//UI描画
}

std::shared_ptr<Map> GameScene::GetMap() const
{
	return mGameMap;
}

float GameScene::GetCurrentGameTime()
{
	return mCurrentGameTime;
}

float GameScene::GetClearGameTime()
{
	return mGameClearTime;
}

void GameScene::StartLevelUpEvent(int count)
{
	mLevelUpCount = count;
	LevelUpEvent();
}

void GameScene::LevelUpEvent()
{
	//レベルアップがある場合、レベルアップ画面の表示、報酬の抽選
	if(mLevelUpCount > 0){
		auto element = std::dynamic_pointer_cast<LevelUpPanel>(UI_M.GetCanvas("LevelUpMenu")->GetElements()[0]);
		if(!element){
			mLevelUpCount = 0;
			return;
		}

		//報酬をランダムに抽選
		element->SetLevelUpReward(STATUS.GetRandomSelectRewards());

		//必要な表示情報（現在のステータスなど）を設定
		element->SetStatusInfoPanel();

		//レベルアップ画面フラグオン
		mIsLevelUp = true;
	}else{//ない場合、レベルアップ画面の終了処理実行
		mLevelUpCount = 0;
		FinishLevelUp();
	}
}

void GameScene::GameOver()
{
	//ゲームオーバー専用UIに切り替え
	UI_M.SetVisibiltyToCanvas("GameOver", true);
	UI_M.SetVisibiltyToCanvas("HUD", false);
	mGameOver = true;
	//５秒後にタイトルに戻る
	EVENT_M.addEvent([this]() {this->ChangeScene(SceneState::NEXTSCENE);
		}, mChangeSceneTime);
}

void GameScene::GameClear()
{
	//クリア専用UIに切り替え
	UI_M.SetVisibiltyToCanvas("GameClear", true);
	UI_M.SetVisibiltyToCanvas("HUD", false);
	mGameClear = true;
	//５秒後にタイトルに戻る
	EVENT_M.addEvent([this]() {this->ChangeScene(SceneState::NEXTSCENE);
		},mChangeSceneTime);
}

void GameScene::UpdateTimer(float deltaTime)
{
	mCurrentGameTime +=deltaTime;
}

void GameScene::SpawnBoss()
{
	//ボスをプレイヤーの画面の隅でスポーンさせる
	Vector2D<float>bossStart = Singleton<EnemyFactory>::get_instance().GetRandomSpawnPosition(0, mGameMap);
	auto boss = ACTOR_M.GetBossEnemy("BossEnemyA", mGameMap->GetLocalFromWorldPosition(bossStart), bossStart, 90, true);

	if (!ACTOR_M.RegistObject(boss)) {//ボス登録
		return;
	}
	//ボスのHPを表示
	auto hudCanvas = UI_M.GetCanvas("HUD");
	auto hud = std::dynamic_pointer_cast<HUD>(hudCanvas->GetElements()[0]);
	if(hud){
		hud->SpawnBossHUD();
	}
}

void GameScene::End()
{
	//UIを全部消去
	UI_M.ClearCanvas();
	//オブジェクトを全て非アクティブに戻す
	Singleton<ActorManager>::get_instance().Reset();
	//各オブジェクトのコリジョン座標消去
	COLLISION_M.RemoveAllCharacter();
	//現在描画しているエフェクトを消去
	EFFECT_M.Finitialze();
	//BGMやSFXを停止
	SOUND_M.Reset();
	//イベント消去
	EVENT_M.Reset();
	mInitialze = false;
}
