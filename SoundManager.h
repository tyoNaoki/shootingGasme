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
	//�������A�I���A���Z�b�g����
	void Init();
	void End();
	void Reset();

	//ID�̃T�E���h�Đ�
	int PlaySoundByID(const std::string&id,bool isLoop = false); 
	//�T�E���h��~
	int StopSoundPlay(int handle);

	bool CheckSoundPlaying(int handle)const; //�T�E���h�Đ������m�F

	void ClearSoundPlayHandle(); //�T�E���h�A�~���[�W�b�N�S����

	//���y
	int PlayBGMOnSound(const std::string&id);
	int PlayMusicByID(const std::string& id, bool isLoop= true); //ID�Ń~���[�W�b�N�Đ��J�n�B���[�v���邩�w��B(������0,���s��-1)
	int GetBGMVolume();
	int GetActionSoundVolume();
	bool CheckMusicPlaying()const; //�~���[�W�b�N�Đ������m�F
	bool CheckBGMPlaying()const; //�~���[�W�b�N�Đ������m�F
	bool CheckAssignedMusicPlaying(const std::string& id)const; //�~���[�W�b�N�Đ������m�F
	void StopMusic(); //���y�I��
	void StopBGM(); //BGM�I��
	void ChangeSoundVolume(const std::string& id,int volume);
	void ChangeMusicVolume(const std::string& id, int volume);
	void ChangeBGMVolume(int volume);
	void ChangeActionSoundVolume(int volume);

private:
	SoundManager();
	~SoundManager();

	SoundInfo GetSoundInfo(const std::string&)const; //ID�ŃT�E���h�n���h�����擾(�Ȃ��Ƃ���-1)
	int GetMusicHandle(const std::string&); //ID�Ń~���[���n���h�����擾(�Ȃ��Ƃ���-1)

	std::unordered_map<std::string, SoundInfo> mSoundHandles; //�T�E���h�n���h�����X�g
	std::vector<int>mPlaySoundHandles;

	std::unordered_map<std::string, int> mMusicHandles; //�~���[�W�b�N�n���h�����X�g
	std::string mCurrentMusic_ID = "EMPTY"; //�Đ����̉��y
	std::string mCurrentBGM_ID = "EMPTY"; //�Đ�����BGM
	int mCurrentBGMVolume = 100;
	int mCurrentActionSoundVolume = 100;
	int mCurrentMusicHandle = -1;
	SoundInfo mCurrentBGMInfo;
};

