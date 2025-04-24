#include "SoundManager.h"
#include  "DxLib.h"

SoundManager::SoundManager(){
}

SoundManager::~SoundManager()
{
    End();
}

void SoundManager::Init()
{
    mCurrentMusicHandle = -1;
    mCurrentMusic_ID  = "EMPTY";

    //各交換音
    //テスト用
    int soundHandle = LoadSoundMem("Sound/test.wav"); // 音声ファイルの読み込み
    mSoundHandles["testSound"] = {30,soundHandle };

    //プレイヤーの近接音
    soundHandle = LoadSoundMem("Sound/SwordAttack1.wav");
    mSoundHandles["swordAttack"] = {80, soundHandle};

    //攻撃ヒット音
    soundHandle = LoadSoundMem("Sound/SwordImpactHit1.wav");
    mSoundHandles["Hit"] = {70,soundHandle};

    //射撃音
    soundHandle = LoadSoundMem("Sound/SwordAttack3.wav");
    mSoundHandles["FireBulletSound"] = {80, soundHandle};

    //魔法発射音
    soundHandle = LoadSoundMem("Sound/Fireball3.wav");
    mSoundHandles["MagicBulletSound"] = { 50, soundHandle };

    //爆発音
    soundHandle = LoadSoundMem("Sound/explosion.wav");
    mSoundHandles["ExplosionSound"] = { 100, soundHandle };

    //ボスのレーザーアラート音
    soundHandle = LoadSoundMem("Sound/Warning-Siren05-04(Fast-Single).mp3");
    mSoundHandles["laserAlertSound"] = {70,soundHandle};

    //ボスの弾丸発射音
    soundHandle = LoadSoundMem("Sound/Fireball3.wav");
    mSoundHandles["BossFireBulletSound"] = {80, soundHandle};

    //ボスのレーザー発射音
    soundHandle = LoadSoundMem("Sound/lightblast5.wav");
    mSoundHandles["laserFireSound"] = {60, soundHandle};

    //BGM
    soundHandle = LoadSoundMem("Sound/bgm-clashing-blades.wav");
    mSoundHandles["BGM"]= {50, soundHandle};
}

void SoundManager::End()
{
    ClearSoundPlayHandle();
}

void SoundManager::Reset()
{
    ClearSoundPlayHandle();
}

int SoundManager::PlaySoundByID(const std::string& id,bool isLoop)
{
    //効果音の情報取得
    auto sInfo = GetSoundInfo(id);
    if (sInfo.handle == -1) { return -1; }
    //効果音再生
    PlaySoundMem(sInfo.handle, isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK, true);
    //効果音の音量設定
    float actionSoundVomue = mCurrentActionSoundVolume>0 ? (float)mCurrentActionSoundVolume/100.0 : 0;
    int volume = sInfo.defaultVolume * actionSoundVomue;
    ChangeSoundVolume(id,volume);

    //現在の鳴らしている効果音に追加
    mPlaySoundHandles.push_back(sInfo.handle);

    return 0;
}

int SoundManager::StopSoundPlay(int handle)
{
    //現在その効果音を鳴らしているか
    auto it = std::find(mPlaySoundHandles.begin(),mPlaySoundHandles.end(),handle);
    if (it == mPlaySoundHandles.end()) { return -1; }
    //音を停止
    StopSoundMem((*it));
    //現在の鳴らしている音から削除
    mPlaySoundHandles.erase(it);
    return 0;
}

bool SoundManager::CheckSoundPlaying(int handle) const
{
    //現在鳴らしているかどうか
    auto it = std::find(mPlaySoundHandles.begin(), mPlaySoundHandles.end(), handle);
    if (it == mPlaySoundHandles.end()) { return false; }
    if (CheckSoundMem((*it))) { return true; }
    else { return false; }
}

void SoundManager::ClearSoundPlayHandle()
{
    //効果音を消去
    for(auto &s:mSoundHandles){
        //現在鳴らしているなら音をストップ
        if(CheckSoundPlaying(s.second.handle)){
            StopSoundPlay(s.second.handle);
        }
        DeleteSoundMem(s.second.handle);
    }
    mSoundHandles.clear();
    //BGMを消去
    StopMusic();
    for(auto &m: mMusicHandles){
        DeleteMusicMem(m.second);
    }
    mMusicHandles.clear();
    
}

int SoundManager::PlayBGMOnSound(const std::string& id)
{
    //効果音の情報取得
    auto sInfo = GetSoundInfo(id);
    if (sInfo.handle == -1) { return -1; }
    PlaySoundMem(sInfo.handle, DX_PLAYTYPE_LOOP, true);
    mPlaySoundHandles.push_back(sInfo.handle);
    mCurrentBGMInfo = sInfo;
    //音量設定
    float bgmVomue = mCurrentBGMVolume > 0 ? (float)mCurrentBGMVolume / 100.0 : 0;
    int volume = sInfo.defaultVolume * bgmVomue;
    ChangeSoundVolume(id,volume);
    mCurrentBGM_ID = id;
}

int SoundManager::PlayMusicByID(const std::string& id, bool isLoop)
{
    //音楽の情報取得
    int mh = GetMusicHandle(id);
    if (mh == -1) { return -1; }
    //再生
    PlayMusicMem(mh, isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
    mCurrentMusicHandle = mh;
    mCurrentMusic_ID = id ;
    return 0;
}

int SoundManager::GetBGMVolume()
{
    return mCurrentBGMVolume;
}

int SoundManager::GetActionSoundVolume()
{
    return mCurrentActionSoundVolume;
}

bool SoundManager::CheckMusicPlaying() const
{
    return mCurrentMusic_ID != "EMPTY";
}

bool SoundManager::CheckBGMPlaying() const
{
    return mCurrentBGM_ID != "EMPTY";
}

bool SoundManager::CheckAssignedMusicPlaying(const std::string& id) const
{
    return mCurrentMusic_ID == id;
}

void SoundManager::StopMusic()
{
    //現在何か鳴らしているかどうか
    if(mCurrentMusic_ID != "EMPTY"){
        //停止
        StopMusicMem(mCurrentMusicHandle);
        //現在鳴らしている曲の変数を初期化
        mCurrentMusicHandle = -1;
        mCurrentMusic_ID = "EMPTY";
    }
}

void SoundManager::StopBGM()
{
    //現在何か鳴らしているかどうか
    if (mCurrentBGM_ID != "EMPTY") {
        //停止
        StopSoundMem(mCurrentBGMInfo.handle);
        //現在鳴らしている曲の変数を初期化
        mCurrentBGMInfo = SoundInfo();
        mCurrentBGM_ID = "EMPTY";
    }
}

void SoundManager::ChangeSoundVolume(const std::string& id, int volume)
{
    //効果音の情報取得
    auto sInfo = GetSoundInfo(id);
    if(sInfo.handle != -1&&volume>=0&&volume<=100){
        //音量設定
        ChangeVolumeSoundMem(255 * volume / 100, sInfo.handle);
    }
}

void SoundManager::ChangeMusicVolume(const std::string& id, int volume)
{
    //音楽の情報取得
    auto handle = GetMusicHandle(id);
    if (handle && volume >= 0 && volume <= 100) {
        //音量設定
        ChangeVolumeSoundMem(255 * volume / 100, handle);
    }
}

void SoundManager::ChangeBGMVolume(int volume)
{
    //現在BGMが流れているか
    if (mCurrentBGM_ID != "EMPTY") {
        //現在のBGMの音量を設定
        float temp = volume > 0 ? (float)volume / 100.0 : 0;
        int bgmVolume = mCurrentBGMInfo.defaultVolume * temp;
        ChangeSoundVolume(mCurrentBGM_ID, bgmVolume);
        mCurrentBGMVolume = volume;
    }
}

void SoundManager::ChangeActionSoundVolume(int volume)
{
    //効果音の全再生音量を設定
    if(volume>=0&&volume<=100){
        mCurrentActionSoundVolume = volume;
    }
}

SoundInfo SoundManager::GetSoundInfo(const std::string& id) const
{
    //効果音の情報(初期音量、ハンドル)を取得
    auto it = mSoundHandles.find(id);
    if (it == mSoundHandles.end()) { return SoundInfo(); }
    return (*it).second;
}

int SoundManager::GetMusicHandle(const std::string& id)
{
    //音楽の情報(初期音量、ハンドル)を取得
    auto it = mMusicHandles.find(id);
    if (it == mMusicHandles.end()) {
        return -1;
    }

    return (*it).second;
}
