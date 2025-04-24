#pragma once
#include "Singleton.h"
#include <unordered_map>
#include"DxLib.h"
#include <string>

#define SOUND_M Singleton<SoundManager>::get_instance()

struct SoundInfo {
	int defaultVolume = 0;
	int handle = -1;
};

class SoundManager
{
friend class Singleton<SoundManager>;

public:
	//初期化、終了、リセット処理
	void Init();
	void End();
	void Reset();

	//IDのサウンド再生
	int PlaySoundByID(const std::string&id,bool isLoop = false); 
	//サウンド停止
	int StopSoundPlay(int handle);

	bool CheckSoundPlaying(int handle)const; //サウンド再生中か確認

	void ClearSoundPlayHandle(); //サウンド、ミュージック全消去

	//音楽
	int PlayBGMOnSound(const std::string&id);
	int PlayMusicByID(const std::string& id, bool isLoop= true); //IDでミュージック再生開始。ループするか指定。(成功で0,失敗で-1)
	int GetBGMVolume();
	int GetActionSoundVolume();
	bool CheckMusicPlaying()const; //ミュージック再生中か確認
	bool CheckBGMPlaying()const; //ミュージック再生中か確認
	bool CheckAssignedMusicPlaying(const std::string& id)const; //ミュージック再生中か確認
	void StopMusic(); //音楽終了
	void StopBGM(); //BGM終了
	void ChangeSoundVolume(const std::string& id,int volume);
	void ChangeMusicVolume(const std::string& id, int volume);
	void ChangeBGMVolume(int volume);
	void ChangeActionSoundVolume(int volume);

private:
	SoundManager();
	~SoundManager();

	SoundInfo GetSoundInfo(const std::string&)const; //IDでサウンドハンドルを取得(ないときは-1)
	int GetMusicHandle(const std::string&); //IDでミュー軸ハンドルを取得(ないときは-1)

	std::unordered_map<std::string, SoundInfo> mSoundHandles; //サウンドハンドルリスト
	std::vector<int>mPlaySoundHandles;

	std::unordered_map<std::string, int> mMusicHandles; //ミュージックハンドルリスト
	std::string mCurrentMusic_ID = "EMPTY"; //再生中の音楽
	std::string mCurrentBGM_ID = "EMPTY"; //再生中のBGM
	int mCurrentBGMVolume = 100;
	int mCurrentActionSoundVolume = 100;
	int mCurrentMusicHandle = -1;
	SoundInfo mCurrentBGMInfo;
};

