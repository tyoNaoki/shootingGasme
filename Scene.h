#pragma once
#include <memory>
#include "Map.h"
#include <unordered_map>
#include "DxLib.h"
#include <string>

class Map;
enum class SceneState { PLAYING, NEXTSCENE, FINISH };

// �V�[���N���X�̒�`
class Scene {
public:
    virtual ~Scene() = default;

    // �������֐��i�������z�֐��j
    virtual bool Initialize() = 0;

    // �X�V�֐��i�������z�֐��j
    virtual void Update(const float deltaTime) = 0;

    // �`��֐��i�������z�֐��j
    virtual void Render(const float deltaTime) = 0;

    // ���݂̃V�[����Ԃ��擾����֐�
    SceneState GetCurrentState() const;

    // �V�[����ύX����֐�
    void ChangeScene(SceneState newScene);

    // �}�b�v���擾����֐��i�������z�֐��j
    virtual std::shared_ptr<Map> GetMap() const = 0;

    // �V�[�������擾����֐�
    std::string GetSceneName() const { return mSceneName; }

    // �|�[�Y��Ԃ�ݒ肷��֐�
    void SetPaused(bool isPause) { mIsPaused = isPause; }

    // �|�[�Y��Ԃ��擾����֐�
    bool IsPaused() { return mIsPaused; }

    // �|�[�Y���L�����ǂ������m�F����֐�
    bool IsPauseEnabled() const { return mPauseEnabled; }

    // ���x���A�b�v��Ԃ�ݒ肷��֐�
    void SetIsLevelUp(bool isLevelUp) { mIsLevelUp = isLevelUp; }

    // ���x���A�b�v��Ԃ��m�F����֐�
    bool IsLevelUp() { return mIsLevelUp; }

    // ���x���A�b�v�C�x���g���J�n����֐��i�I�[�o�[���C�h�\�j
    virtual void StartLevelUpEvent(int count) {}

    // ���x���A�b�v�C�x���g�֐��i�I�[�o�[���C�h�\�j
    virtual void LevelUpEvent() {}

    // ���x���A�b�v��V���I�����ꂽ�ۂ̏���
    void LevelUpRewardSelected();

    // ���x���A�b�v���I�������ۂ̏���
    void FinishLevelUp();

    // �I�������֐��i�������z�֐��j
    virtual void End() = 0;

protected:
    std::string mSceneName = "EMPTY"; // �V�[����
    bool mIsPaused = false; // �|�[�Y���
    bool mPauseEnabled = true; // �|�[�Y���L����
    bool mIsLevelUp = false; // ���x���A�b�v���
    int mLevelUpCount = 0; // ���x���A�b�v��

private:
    SceneState mCurrentState; // ���݂̃V�[�����
};


