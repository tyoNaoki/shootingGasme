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

	//���Ƃ��Ĉ�U����,���
	//Singleton<ActorManager>::get_instance().Reset();
	
	//�e�L�������Ƃ̍쐬�N���X����
	mGameMap = nullptr;
}

bool GameScene::Initialize()
{
	if (mInitialze){
		return true;
	}

	mCurrentGameTime = 0.0f;

	//�v���C���[
	STATUS.ResetPlayerStatus(mPlayerStartLevel);
	STATUS.ResetWeaponStatus("PlayerMeleeComponent");
	STATUS.ResetWeaponStatus("PlayerGunComponent");
	STATUS.ResetWeaponStatus("PlayerReflectionGunComponent");
	STATUS.ResetWeaponStatus("PlayerThrowBombComponent");

	//�G
	STATUS.ResetEnemyStatus(mEnemyStartLevel,mSpawnEnemyName);
	STATUS.ResetEnemyStatus(mEnemyStartLevel, mSpawnEnemyNameB);
	STATUS.ResetEnemyStatus(mEnemyStartLevel, mSpawnEnemyNameC);
	STATUS.ResetBossStatus();
	STATUS.ResetWeaponStatus("NormalEnemyAttackComponent");
	STATUS.ResetWeaponStatus("NormalEnemyMeleeComponent");

	//��V���Z�b�g
	STATUS.ResetRewardS();
	STATUS.ResetSurvivalBonus();

	//�X�R�A�֌W���Z�b�g
	STATUS.ResetScore();
	STATUS.ResetCount();

	//�e�L�������Ƃ̃N���X����

	//�����ʒu���}�b�v�̒����ɐݒ�
	auto startCenter = (mGameMap->GetMapSize() * mGameMap->GetCellSize())/2.0;
	mGameMap->Initialize(startCenter);

	//�Փ˔���̂��߂ɃO���b�h�̕����T�C�Y�ݒ�
	Singleton<CollisionManager>::get_instance().Initialize(50,mGameMap);

	int height,width;
	GetWindowSize(&width,&height);

	//�v���C���[�̏���������
	auto localStart = Vector2D<float>(static_cast<float>(width / 2), static_cast<float>(height / 2));
	auto player = ACTOR_M.GetPlayer("PlayerCharacter", localStart, startCenter, 90, true);
	//ActorManager�Ƀv���C���[��ǉ�
	if (!ACTOR_M.RegistObject(player)) {
		return false;
	}

	auto& dh = DEBUG_HELPER;

	//std::string message = "spawnPosition is " + enemyStart.ToString();
	//dh.Add(message, 5.0f);
	
	//�G�̃X�|�[�� & �o�^
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

	//�f�o�b�N�p�̃}�b�v�ݒ�
	Singleton<DebugHelper>::get_instance().SetMap(mGameMap);

	Singleton<EffectManager>::get_instance().Initialize();

	//UI�ݒ�
	//
	//�Q�[���I�[�o�[
	auto gameOverCanvas = std::make_shared<Canvas>();
	auto gameOverUI = std::make_shared<GameOverPanel>(width,height);
	gameOverCanvas->AddElement(gameOverUI);
	gameOverCanvas->SetVisibilty(false);
	UI_M.AddCanvas("GameOver",gameOverCanvas);

	//�Q�[���N���A
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

	//�|�[�Y���
	auto pauseCanvas = std::make_shared<Canvas>();
	auto menuPanel = std::make_shared<MenuPanel>(width,height);
	pauseCanvas->AddElement(menuPanel);
	pauseCanvas->SetVisibilty(false);
	UI_M.AddCanvas("PauseMenu", pauseCanvas);
	
	//���x���A�b�v���
	auto levelUpCanvas = std::make_shared<Canvas>();
	auto levelUpPanel = std::make_shared<LevelUpPanel>(width, height,STATUS.GetRewardMaxCount());

	//��U�m�F�p�ɕ�V�Z�b�g
	//levelUpPanel->SetLevelUpReward(STATUS.GetRandomSelectRewards());

	levelUpCanvas->AddElement(levelUpPanel);
	levelUpCanvas->SetVisibilty(false);
	UI_M.AddCanvas("LevelUpMenu", levelUpCanvas);

	//�d�v�C�x���g�ݒ�
	//�Q�[���I�[�o�[�C�x���g�Z�b�g
	EVENT_M.addEvent([this](){
		this->GameOver();
	},[this](){
		return Singleton<ActorManager>::get_instance().GetCurrentPlayer()->GetHP()<=0.0f;
	});

	//�Q�[���N���A�C�x���g�Z�b�g
	EVENT_M.addEvent([this]() {
		this->GameClear();
		}, [this]() {
			//300�b��ɃQ�[���N���A
			return mCurrentGameTime >= mGameClearTime;
		});

	//�G���ݒ肵���b�����Ƀ��x���A�b�v
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

	//���{�X���ݒ肵���b�����ɃX�|�[��
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
			//180�b���3��
			if (mCurrentGameTime >= 180 && mEnemyCspawnIndex < 3) {
				mEnemyCspawnIndex = 3;
				return true;
			}
			//120�b��ɂP��
			if (mCurrentGameTime >= 120 && mEnemyCspawnIndex < 2) {
				mEnemyCspawnIndex = 2;
				return true;
			}
			//60�b��ɂP��
			if (mCurrentGameTime>=60 && mEnemyCspawnIndex < 1) {
				mEnemyCspawnIndex = 1;
				return true;
			}
			return false;
		}, 0.0f, true);

	//��莞�Ԍ�ɐݒ肵���b�����Ɍo���l����ʃA�b�v
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

	//�{�X�o��
	EVENT_M.addEvent([this]() {
		this->SpawnBoss();
		}, [this]() {
			return mCurrentGameTime >= mGameBossSpawnTime;
	});

	//�T�E���h�}�l�[�W���[������
	SOUND_M.Init();

	//�o�b�N�O���E���h�~���[�W�b�N
	if (SOUND_M.PlayBGMOnSound("BGM") == -1) {
		return false;
	}

	//�G�t�F�N�g���[�h
	EFFECT_M.LoadEffects();

	//�Q�[���X�^�[�g
	ChangeScene(SceneState::PLAYING);

	//�e�ϐ�������
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

	EVENT_M.Update(deltaTime);//�C�x���g��������

	UI_M.Update(deltaTime);//UI�X�V

	//�Q�[���I�[�o�[���N���A���Ă��邩
	if (mGameOver || mGameClear){
		return;
	}

	//�|�[�Y
	if (IsPaused()) {
		UI_M.SetVisibiltyToCanvas("PauseMenu", true);
		if(IsLevelUp()){
			UI_M.SetVisibiltyToCanvas("LevelUpMenu", false);
		}
		return;
	}else { //�|�[�Y�����
		UI_M.SetVisibiltyToCanvas("PauseMenu", false);
	}

	//���x���A�b�v��ʂŕ�V�I��
	if(IsLevelUp()){
		UI_M.SetVisibiltyToCanvas("LevelUpMenu", true);
		return;
	}else{//�I���I��
		UI_M.SetVisibiltyToCanvas("LevelUpMenu", false);
	}
	
	//�G���G�X�|�[������
	ActorManager& am = ACTOR_M;
	if (am.GetActorCount() < mMaxSpawnEnemyCount && mCurrentNextSpawnTime<=0.0f){
		for(int i=0;i<mCurrentSpawnEnemyCount;i++){

			//�GA�̃X�|�[��
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
				//�GB�̃X�|�[��
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
	//�f�o�b�O�֌W
	DEBUG_HELPER.Add("PlayerLevel is " + std::to_string(STATUS.GetCurrentPlayerStatus().level));
	DEBUG_HELPER.Add("EnemyLevel is " + std::to_string(STATUS.GetCurrentEnemyStatus("NormalEnemyA").level));
	auto debugHp = ACTOR_M.GetCurrentPlayer()->GetHP();
	DEBUG_HELPER.Add("PlayerCurrentHP is " + std::to_string(debugHp));
	debugHp = STATUS.GetCurrentEnemyStatus("NormalEnemyA").hp;
	DEBUG_HELPER.Add("EnemyHP is " + std::to_string(debugHp));

	//�e�I�u�W�F�N�g�̍��W��e�R���|�[�l���g�̍X�V
	am.Update(deltaTime); 

	//�Q�[�������ԍX�V
	mCurrentGameTime += deltaTime; 

	//�G�t�F�N�g�摜�X�V
	EFFECT_M.Update(deltaTime);
}

void GameScene::Render(const float deltaTime)
{
	mGameMap->Draw(); //�}�b�v�̊e�Z���̕`��

	ACTOR_M.Draw(deltaTime);//�I�u�W�F�N�g�̕`��
	EFFECT_M.Draw(deltaTime);//�G�t�F�N�g�̕`��
	UI_M.Draw(deltaTime);//UI�`��
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
	//���x���A�b�v������ꍇ�A���x���A�b�v��ʂ̕\���A��V�̒��I
	if(mLevelUpCount > 0){
		auto element = std::dynamic_pointer_cast<LevelUpPanel>(UI_M.GetCanvas("LevelUpMenu")->GetElements()[0]);
		if(!element){
			mLevelUpCount = 0;
			return;
		}

		//��V�������_���ɒ��I
		element->SetLevelUpReward(STATUS.GetRandomSelectRewards());

		//�K�v�ȕ\�����i���݂̃X�e�[�^�X�Ȃǁj��ݒ�
		element->SetStatusInfoPanel();

		//���x���A�b�v��ʃt���O�I��
		mIsLevelUp = true;
	}else{//�Ȃ��ꍇ�A���x���A�b�v��ʂ̏I���������s
		mLevelUpCount = 0;
		FinishLevelUp();
	}
}

void GameScene::GameOver()
{
	//�Q�[���I�[�o�[��pUI�ɐ؂�ւ�
	UI_M.SetVisibiltyToCanvas("GameOver", true);
	UI_M.SetVisibiltyToCanvas("HUD", false);
	mGameOver = true;
	//�T�b��Ƀ^�C�g���ɖ߂�
	EVENT_M.addEvent([this]() {this->ChangeScene(SceneState::NEXTSCENE);
		}, mChangeSceneTime);
}

void GameScene::GameClear()
{
	//�N���A��pUI�ɐ؂�ւ�
	UI_M.SetVisibiltyToCanvas("GameClear", true);
	UI_M.SetVisibiltyToCanvas("HUD", false);
	mGameClear = true;
	//�T�b��Ƀ^�C�g���ɖ߂�
	EVENT_M.addEvent([this]() {this->ChangeScene(SceneState::NEXTSCENE);
		},mChangeSceneTime);
}

void GameScene::UpdateTimer(float deltaTime)
{
	mCurrentGameTime +=deltaTime;
}

void GameScene::SpawnBoss()
{
	//�{�X���v���C���[�̉�ʂ̋��ŃX�|�[��������
	Vector2D<float>bossStart = Singleton<EnemyFactory>::get_instance().GetRandomSpawnPosition(0, mGameMap);
	auto boss = ACTOR_M.GetBossEnemy("BossEnemyA", mGameMap->GetLocalFromWorldPosition(bossStart), bossStart, 90, true);

	if (!ACTOR_M.RegistObject(boss)) {//�{�X�o�^
		return;
	}
	//�{�X��HP��\��
	auto hudCanvas = UI_M.GetCanvas("HUD");
	auto hud = std::dynamic_pointer_cast<HUD>(hudCanvas->GetElements()[0]);
	if(hud){
		hud->SpawnBossHUD();
	}
}

void GameScene::End()
{
	//UI��S������
	UI_M.ClearCanvas();
	//�I�u�W�F�N�g��S�Ĕ�A�N�e�B�u�ɖ߂�
	Singleton<ActorManager>::get_instance().Reset();
	//�e�I�u�W�F�N�g�̃R���W�������W����
	COLLISION_M.RemoveAllCharacter();
	//���ݕ`�悵�Ă���G�t�F�N�g������
	EFFECT_M.Finitialze();
	//BGM��SFX���~
	SOUND_M.Reset();
	//�C�x���g����
	EVENT_M.Reset();
	mInitialze = false;
}
