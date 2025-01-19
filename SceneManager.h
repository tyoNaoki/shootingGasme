#pragma once
#include <vector>
#include <unordered_map>
#include "Singleton.h"
#include "Scene.h"
#include "GameSystemManager.h"

#define SCENE_M Singleton<SceneManager>::get_instance()

class Scene;

// �V�[���}�l�[�W���N���X
class SceneManager
{
    friend class Singleton<SceneManager>;

public:
    // �������֐�
    bool Initialize();

    // ���s�֐�
    bool Run(const float deltaTime);

    // �X�V�֐�
    void Update(const float deltaTime) const;

    // �`��֐�
    void Render(const float deltaTime) const;

    // �V�[���ǉ��֐�
    void AddScene(std::shared_ptr<Scene> scene, int idx);

    // ���݂̃V�[���擾�֐�
    std::shared_ptr<Scene> GetCurrentScene() const;

    // �V�[���C���f�b�N�X�擾�֐�
    int GetSceneIndex(std::string sceneName) const;

    // ���x���A�b�v�C�x���g�����֐�
    void OnLevelUpEvent(int count);

    // ���x���A�b�v��V�I�����̏����֐�
    void OnLevelUpRewardSelected();

    // �V�[���ύX�֐�
    void ChangeScene(SceneState newScene);

    // �Q�[���I���֐�
    void ExitGame();

private:
    // �R���X�g���N�^
    SceneManager();
    // �f�X�g���N�^
    ~SceneManager();

    // �|�[�Y��ԃ`�F�b�N�֐�
    void PauseCheck(SceneState& state, float deltaTime);

    std::vector<std::shared_ptr<Scene>> mSceneList; // �V�[�����X�g

    int mCurrent_SceneIndex = 0; // ���݂̃V�[���C���f�b�N�X

    int mNext_SceneIndex = -1; // ���̃V�[���C���f�b�N�X

    bool mInitialized = false; // �������t���O

    float mCoolTime_pause = 0.0f; // �|�[�Y�̃N�[���^�C��

    bool mIsExitGame = false; // �Q�[���I���t���O
};


