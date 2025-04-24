#include "GraphicManager.h"
#include "DebugHelper.h"
#include "DxLib.h"

bool GraphicManager::Init()
{
    if(mInitialize) return true;

    imageLoader = std::make_unique<ImageLoader>();

    mGraphicFiles["Title"] = {
    {"Title","Video/DXSurvivePicture.png",{1,1,1,1920,1080}}
    };

    //Player
    mGraphicFiles["PlayerCharacter"] = {
    {"Idle","image/Hero/Idle/HeroKnight_Idle_.png",{8,1,1,100,55}},
    {"Walking","image/Hero/Run/HeroKnight_Run_.png",{10,1,1,100,55}},
    {"Dead","image/Hero/Dead/HeroKnight_Death_.png",{10,1,1,100,55},1.0f,false}
    };

    mGraphicFiles["NormalEnemyA"] = {
    {"Idle","image/Goblin/Idle/Idle.png",{4,4,1,150,150}},
    {"Walking","image/Goblin/Walking/Run.png",{8,8,1,150,150},0.5f},
    {"Dead","image/Goblin/Dead/Death.png",{4,4,1,150,150},1.5f,false}
    };

    mGraphicFiles["NormalEnemyB"] = {
    {"Flight","image/FlyingEye/Flight/Flight.png",{8,8,1,150,150}},
    {"Dead","image/FlyingEye/Dead/Death.png",{4,4,1,150,150},1.5f,false}
    };

    mGraphicFiles["NormalEnemyC"] = {
    {"Idle","image/Mushroom/Idle/Idle.png",{4,4,1,150,150}},
    {"Walking","image/Mushroom/Walking/Run.png",{8,8,1,150,150},0.4f},
    {"Dead","image/Mushroom/Dead/Death.png",{4,4,1,150,150},1.5f,false}
    };

    mGraphicFiles["BossEnemyA"] = {
    {"BossIdle","image/Boss/Idle/BossIdle.png",{4,4,1,100,100}},
    {"BossGunComponent","image/Boss/LaserAttack/BossLaser.png",{7,7,1,100,100},2.0f,false},
    {"BossLaserComponent","image/Boss/LaserAttack/BossLaser.png",{7,7,1,100,100},0.15f,false},
    {"Dead","image/Boss/Dead/BossDead.png",{14,10,2,100,100},1.0f,false}
    };

    mGraphicFiles["LevelUpItem"] = {
    {"Flight","image/Item/ExpItemSprites.png",{1,1,1,16,16}}
    };

    mGraphicFiles["HealItem"] = {
    {"Flight","image/Item/HealItemSprites.png",{1,1,1,16,16}}
    };

    mGraphicFiles["PlayerBullet"] =  {
    {"Flight","image/Bullet/Player/SwordSprite.png",{3,3,1,92,102},1.5f}
    };

    mGraphicFiles["PlayerReflectionBullet"] = {
    //{"Flight","image/Bullet/Player/SwordSprite.png",{3,3,1,92,102},1.5f}
    };

    mGraphicFiles["EnemyBullet"] = {
    {"Flight","image/Bullet/Enemy/NormalEnemyB/projectileSprite.png",{3,3,1,48,48}}
    };

    mGraphicFiles["Laser"] = {
    {"LaserStart","image/Laser/LaserStart.png",{6,1,6,86,100},1.0,false},
    {"Laser","image/Laser/Laser.png",{1,1,1,14,100},1.0,false}
    }; 

    mGraphicFiles["PlayerBomb"] = {
    {"Bomb","image/Bomb/BombSprite.png",{10,10,1,100,100},1.0,false},
    {"Explosion","image/Bomb/Explosion.png",{7,7,1,34,40},6.0,false}
    };

    mGraphicFiles["BossBullet"] = {
    {"Flight","image/Bullet/Enemy/Boss/ProjectileSprite.png",{4,4,1,50,50}},
    {"Dead","image/Bullet/Enemy/Boss/ProjectileSprite.png",{8,8,1,50,50},1.0f,false}
    };

    //マップ画像
    mGraphicFiles["GameMap"] = {
    {"GROUND","Texture/Grass_05-128x128.png",{1,1,1,128,128}},
    {"WALL","Texture/Bricks_01-128x128.png",{1,1,1,128,128}}
    };

    //エフェクト
    mGraphicFiles["Effect"] = {
    {"Melee","image/Melee/Slash/slash.png",{1,1,1,300,141},1.0f,false},
    {"Spore","image/Melee/Spore/sporeWave.png",{4,4,1,50,50},0.7f,false}
    };
    
    mInitialize = true;
    return true;
}

void GraphicManager::CreateGraphic(const std::string name)
{
    auto itr = mGraphicFiles.find(name);
    if(itr !=mGraphicFiles.end() && mGraphics.find(name) == mGraphics.end()){
        //画像読み込み
        mGraphics.emplace(name,imageLoader->loadImages(itr->second));
    }
}

bool GraphicManager::ReadHandle(int& gHandle,const std::string& name, const std::string& actionName)
{
    auto charaGraphic = mGraphics.find(name);

    //見つからない場合、空画像用のアニメーションに差し替え
    if (charaGraphic == mGraphics.end() || charaGraphic->second->isDummy() || !charaGraphic->second->IsLoadGraphic(actionName)) {
        gHandle = -2;
        return false;
    }

    int frame = charaGraphic->second->GetStartFrame(actionName);
    gHandle = charaGraphic->second->GetGHandle(frame);
    return gHandle != -1;
}

bool GraphicManager::ReadHandle(int& gHandle, const std::string& name, const std::string& actionName, const int interval)
{
    auto charaGraphic = mGraphics.find(name);

    //見つからない場合、空画像用のアニメーションに差し替え
    if (charaGraphic == mGraphics.end() || charaGraphic->second->isDummy() || !charaGraphic->second->IsLoadGraphic(actionName)) {
        gHandle = -2;
        return false;
    }

    int frame = charaGraphic->second->GetStartFrame(actionName);

    if(interval == -1){
        int maxInterval = charaGraphic->second->GetInterval(actionName);
        gHandle = charaGraphic->second->GetGHandle(frame + maxInterval);
        return gHandle != -1;
    }

    gHandle = charaGraphic->second->GetGHandle(frame + interval);
    return gHandle != -1;
}

void GraphicManager::initializeHandle(int& gHandle, int& cur_frame,float& cur_frameAdjust,Vector2D<float>&size,const std::string& name,const std::string& actionName)
{
    auto charaGraphic = mGraphics.find(name);

    //見つからない場合、空画像用のアニメーションに差し替え
    if(charaGraphic == mGraphics.end()||charaGraphic->second->isDummy() || !charaGraphic->second->IsLoadGraphic(actionName)){
        cur_frame = -1;
        cur_frameAdjust = -1;
        gHandle = -2;
        return;
    }
    
    int frame = charaGraphic->second->GetStartFrame(actionName);
    cur_frame = frame;
    cur_frameAdjust = 0.0f;
    size = charaGraphic->second->GetSize(actionName);
    gHandle = charaGraphic->second->GetGHandle(frame);
}

void GraphicManager::updateHandle(const float deltaTime,int& currentHandle,int& currentframe,float& currentframeAdjust,const std::string& name,std::string actionName)
{
    //空画像の場合、スキップ
    if(currentHandle == -2){
        return;
    }

    auto charaGraphic = mGraphics.find(name);

    //見つからない場合、空画像用のアニメーションに差し替え
    if(charaGraphic==mGraphics.end()||charaGraphic->second->isDummy()){
        currentframe = -1;
        currentframeAdjust = -1;
        currentHandle = -2;
        return;
    }
    
    int maxFrame = 0;
    int startFrame = charaGraphic->second->GetStartFrame(actionName);
    int interval = charaGraphic->second->GetInterval(actionName);
    int frame = currentframe;
    float frame_adjust = currentframeAdjust;
    float speed = charaGraphic->second->GetPlaySpeed(actionName);
    bool isLoop = charaGraphic->second->IsLoop(actionName);

    DebugHelper& dh = Singleton<DebugHelper>::get_instance();

    //一枚絵の場合、更新せず終了
    if(interval==0){
        return;
    }

    if(frame < startFrame){
        frame = startFrame;
    }

     if(frame_adjust > 5.0f){
        frame++;
        frame_adjust = 0;
     }

    if(frame>startFrame+interval){
        if(isLoop){
            frame = startFrame;
        }else{
            frame = startFrame + interval;
        }
    }else{
        frame_adjust += deltaTime * 100.0f * speed;
    }
    
    currentframe = frame;
    currentframeAdjust = frame_adjust;
    currentHandle = charaGraphic->second->GetGHandle(frame);
}

void GraphicManager::updateHandle(const float deltaTime, int& currentHandle, int& currentframe, float& currentframeAdjust, const std::string& name, std::string actionName, const float playSpeed)
{
    //空画像の場合、スキップ
    if (currentHandle == -2) {
        return;
    }

    auto charaGraphic = mGraphics.find(name);

    //見つからない場合、空画像用のアニメーションに差し替え
    if (charaGraphic == mGraphics.end() || charaGraphic->second->isDummy()) {
        currentframe = -1;
        currentframeAdjust = -1;
        currentHandle = -2;
        return;
    }

    int maxFrame = 0;
    int startFrame = charaGraphic->second->GetStartFrame(actionName);
    int interval = charaGraphic->second->GetInterval(actionName);
    int frame = currentframe;
    float frame_adjust = currentframeAdjust;
    float speed = playSpeed;
    bool isLoop = charaGraphic->second->IsLoop(actionName);

    DebugHelper& dh = Singleton<DebugHelper>::get_instance();

    //一枚絵の場合、更新せず終了
    if (interval == 0) {
        return;
    }

    if (frame < startFrame) {
        frame = startFrame;
    }

    if (frame_adjust > 5.0f) {
        frame++;
        frame_adjust = 0;
    }

    if (frame > startFrame + interval) {
        if (isLoop) {
            frame = startFrame;
        }
        else {
            frame = startFrame + interval;
        }
    }
    else {
        frame_adjust += deltaTime * 100.0f * speed;
    }

    currentframe = frame;
    currentframeAdjust = frame_adjust;
    currentHandle = charaGraphic->second->GetGHandle(frame);
}

void GraphicManager::TransitionToAction(int& currentHandle, int& currentframe, float& currentFrameAdjust, Vector2D<float>& size, const std::string name, const std::string newAction)
{
    auto charaGraphic = mGraphics.find(name);

    //見つからない場合、空画像用のアニメーションに差し替え
    if (charaGraphic == mGraphics.end() || charaGraphic->second->isDummy()||!charaGraphic->second->IsLoadGraphic(newAction)) {
        currentframe = -1;
        currentFrameAdjust = -1;
        currentHandle = -2;
        return;
    }

    currentframe = charaGraphic->second->GetStartFrame(newAction);
    currentFrameAdjust = 0;
    size = charaGraphic->second->GetSize(newAction);
    currentHandle = charaGraphic->second->GetGHandle(currentframe);
}

bool GraphicManager::IsReadGraphic(std::string name)
{
    return mGraphics.find(name) != mGraphics.end();
}

bool GraphicManager::IsReadAction(const std::string name, const std::string actionName)const
{
    auto charaGraphic = mGraphics.find(name);

    //見つからない場合、空画像用のアニメーションに差し替え
    return (charaGraphic != mGraphics.end() && !charaGraphic->second->isDummy() && charaGraphic->second->IsLoadGraphic(actionName));
}

bool GraphicManager::IsFinishAction(int currentframe,float currentframeAdjust, std::string name,std::string actionName) const
{
    auto charaGraphic = mGraphics.find(name);
    if(charaGraphic==mGraphics.end()){
        return false;
    }

    int startFrame = charaGraphic->second->GetStartFrame(actionName);
    int interval = charaGraphic->second->GetInterval(actionName);

    //現在のアニメーションが描画フレームを超えているか
    return (currentframe >= (startFrame + interval));
}

GraphicManager::~GraphicManager()
{
    mGraphicFiles.clear();
    mGraphics.clear();
    imageLoader = nullptr;
}
