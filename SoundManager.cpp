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

    //�e������
    //�e�X�g�p
    int soundHandle = LoadSoundMem("Sound/test.wav"); // �����t�@�C���̓ǂݍ���
    mSoundHandles["testSound"] = {30,soundHandle };

    //�v���C���[�̋ߐډ�
    soundHandle = LoadSoundMem("Sound/SwordAttack1.wav");
    mSoundHandles["swordAttack"] = {80, soundHandle};

    //�U���q�b�g��
    soundHandle = LoadSoundMem("Sound/SwordImpactHit1.wav");
    mSoundHandles["Hit"] = {70,soundHandle};

    //�ˌ���
    soundHandle = LoadSoundMem("Sound/SwordAttack3.wav");
    mSoundHandles["FireBulletSound"] = {80, soundHandle};

    //���@���ˉ�
    soundHandle = LoadSoundMem("Sound/Fireball3.wav");
    mSoundHandles["MagicBulletSound"] = { 50, soundHandle };

    //������
    soundHandle = LoadSoundMem("Sound/explosion.wav");
    mSoundHandles["ExplosionSound"] = { 100, soundHandle };

    //�{�X�̃��[�U�[�A���[�g��
    soundHandle = LoadSoundMem("Sound/Warning-Siren05-04(Fast-Single).mp3");
    mSoundHandles["laserAlertSound"] = {70,soundHandle};

    //�{�X�̒e�۔��ˉ�
    soundHandle = LoadSoundMem("Sound/Fireball3.wav");
    mSoundHandles["BossFireBulletSound"] = {80, soundHandle};

    //�{�X�̃��[�U�[���ˉ�
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
    //���ʉ��̏��擾
    auto sInfo = GetSoundInfo(id);
    if (sInfo.handle == -1) { return -1; }
    //���ʉ��Đ�
    PlaySoundMem(sInfo.handle, isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK, true);
    //���ʉ��̉��ʐݒ�
    float actionSoundVomue = mCurrentActionSoundVolume>0 ? (float)mCurrentActionSoundVolume/100.0 : 0;
    int volume = sInfo.defaultVolume * actionSoundVomue;
    ChangeSoundVolume(id,volume);

    //���݂̖炵�Ă�����ʉ��ɒǉ�
    mPlaySoundHandles.push_back(sInfo.handle);

    return 0;
}

int SoundManager::StopSoundPlay(int handle)
{
    //���݂��̌��ʉ���炵�Ă��邩
    auto it = std::find(mPlaySoundHandles.begin(),mPlaySoundHandles.end(),handle);
    if (it == mPlaySoundHandles.end()) { return -1; }
    //�����~
    StopSoundMem((*it));
    //���݂̖炵�Ă��鉹����폜
    mPlaySoundHandles.erase(it);
    return 0;
}

bool SoundManager::CheckSoundPlaying(int handle) const
{
    //���ݖ炵�Ă��邩�ǂ���
    auto it = std::find(mPlaySoundHandles.begin(), mPlaySoundHandles.end(), handle);
    if (it == mPlaySoundHandles.end()) { return false; }
    if (CheckSoundMem((*it))) { return true; }
    else { return false; }
}

void SoundManager::ClearSoundPlayHandle()
{
    //���ʉ�������
    for(auto &s:mSoundHandles){
        //���ݖ炵�Ă���Ȃ特���X�g�b�v
        if(CheckSoundPlaying(s.second.handle)){
            StopSoundPlay(s.second.handle);
        }
        DeleteSoundMem(s.second.handle);
    }
    mSoundHandles.clear();
    //BGM������
    StopMusic();
    for(auto &m: mMusicHandles){
        DeleteMusicMem(m.second);
    }
    mMusicHandles.clear();
    
}

int SoundManager::PlayBGMOnSound(const std::string& id)
{
    //���ʉ��̏��擾
    auto sInfo = GetSoundInfo(id);
    if (sInfo.handle == -1) { return -1; }
    PlaySoundMem(sInfo.handle, DX_PLAYTYPE_LOOP, true);
    mPlaySoundHandles.push_back(sInfo.handle);
    mCurrentBGMInfo = sInfo;
    //���ʐݒ�
    float bgmVomue = mCurrentBGMVolume > 0 ? (float)mCurrentBGMVolume / 100.0 : 0;
    int volume = sInfo.defaultVolume * bgmVomue;
    ChangeSoundVolume(id,volume);
    mCurrentBGM_ID = id;
}

int SoundManager::PlayMusicByID(const std::string& id, bool isLoop)
{
    //���y�̏��擾
    int mh = GetMusicHandle(id);
    if (mh == -1) { return -1; }
    //�Đ�
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
    //���݉����炵�Ă��邩�ǂ���
    if(mCurrentMusic_ID != "EMPTY"){
        //��~
        StopMusicMem(mCurrentMusicHandle);
        //���ݖ炵�Ă���Ȃ̕ϐ���������
        mCurrentMusicHandle = -1;
        mCurrentMusic_ID = "EMPTY";
    }
}

void SoundManager::StopBGM()
{
    //���݉����炵�Ă��邩�ǂ���
    if (mCurrentBGM_ID != "EMPTY") {
        //��~
        StopSoundMem(mCurrentBGMInfo.handle);
        //���ݖ炵�Ă���Ȃ̕ϐ���������
        mCurrentBGMInfo = SoundInfo();
        mCurrentBGM_ID = "EMPTY";
    }
}

void SoundManager::ChangeSoundVolume(const std::string& id, int volume)
{
    //���ʉ��̏��擾
    auto sInfo = GetSoundInfo(id);
    if(sInfo.handle != -1&&volume>=0&&volume<=100){
        //���ʐݒ�
        ChangeVolumeSoundMem(255 * volume / 100, sInfo.handle);
    }
}

void SoundManager::ChangeMusicVolume(const std::string& id, int volume)
{
    //���y�̏��擾
    auto handle = GetMusicHandle(id);
    if (handle && volume >= 0 && volume <= 100) {
        //���ʐݒ�
        ChangeVolumeSoundMem(255 * volume / 100, handle);
    }
}

void SoundManager::ChangeBGMVolume(int volume)
{
    //����BGM������Ă��邩
    if (mCurrentBGM_ID != "EMPTY") {
        //���݂�BGM�̉��ʂ�ݒ�
        float temp = volume > 0 ? (float)volume / 100.0 : 0;
        int bgmVolume = mCurrentBGMInfo.defaultVolume * temp;
        ChangeSoundVolume(mCurrentBGM_ID, bgmVolume);
        mCurrentBGMVolume = volume;
    }
}

void SoundManager::ChangeActionSoundVolume(int volume)
{
    //���ʉ��̑S�Đ����ʂ�ݒ�
    if(volume>=0&&volume<=100){
        mCurrentActionSoundVolume = volume;
    }
}

SoundInfo SoundManager::GetSoundInfo(const std::string& id) const
{
    //���ʉ��̏��(�������ʁA�n���h��)���擾
    auto it = mSoundHandles.find(id);
    if (it == mSoundHandles.end()) { return SoundInfo(); }
    return (*it).second;
}

int SoundManager::GetMusicHandle(const std::string& id)
{
    //���y�̏��(�������ʁA�n���h��)���擾
    auto it = mMusicHandles.find(id);
    if (it == mMusicHandles.end()) {
        return -1;
    }

    return (*it).second;
}
