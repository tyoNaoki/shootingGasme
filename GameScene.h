#pragma once
#include "Scene.h"

class GameScene :
    public Scene
{
public:
    //mapWidth,mapHeight,
    GameScene(Vector2D<int>mapWorldSize,Vector2D<float>cell);

    ~GameScene();

    //������
    bool Initialize() override;

    //�X�V
    void Update(const float deltaTime) override;

    //�`��
    void Render(const float deltaTime) override;

    //�I������
    void End() override;

    //���݂̃}�b�v�擾
    std::shared_ptr<Map>GetMap() const override;
    
    //�Q�[�������Ԏ擾
    float GetCurrentGameTime();

    //�N���A�܂ł̎��Ԏ擾
    float GetClearGameTime();

    //���x���A�b�v�C�x���g�J�n����
    void StartLevelUpEvent(int count)override;

    //���x���A�b�v��ʕ\���A���x���A�b�v���̕�V�X�V�Ȃ�
    void LevelUpEvent()override;

    void GameOver();

    void GameClear();

private:
    //�Q�[�������ԍX�V
    void UpdateTimer(float deltaTime);
    //�{�X�o��C�x���g
    void SpawnBoss();
    //�������t���O
    bool mInitialze = false;
    //�}�b�v�̕�
    Vector2D<float>mMapSize;
    //�}�b�v�̈�̃Z���T�C�Y
    Vector2D<float>mCellSize;

    std::shared_ptr<Map> mGameMap;

    //�Q�[�����̃}�b�v�ɑ��݂���G�̍ő吔
    const int mMaxSpawnEnemyCount = 500;

    //��x�ɉ��̃X�|�[�������邩
    int mCurrentSpawnEnemyCount = 0;
    //�ŏ��̈�x�̃X�|�[����
    int mDefaultSpawnCount = 5;
    //���Ԃ��ƂɃX�|�[�����𑫂��Z�ő��₷
    int mLevelUpSpawnCount = 2;
    
    //�G�l�~�[A�̉��̂Ɉ��X�|�[�������邩
    int mCurrentSpawnEnemyBToSpawnCount = 0;
    const int mSpawnEnemyBToSpawnCount = 10;
    //��x�̍ő�GB�X�|�[����
    const int mMaxSpawnEnemyBCount = 1;

    //�G���x���A�����{�[�i�X�̎��ԊǗ�
    int mPreLevelUpTime = -1;
    int mPreLevelUpBonusTime = -1;

    //���݂̒��{�X�̃X�|�[�������Ԗڂ�
    int mEnemyCspawnIndex = 0;

    //���̎��Ԃ��ƂɓG��1���x���A�b�v����
    int mEnemyLevelUpTime = 20;

    //�G�̃X�|�[���Ԋu
    float mCurrentNextSpawnTime = 0.0f;
    const float mDefaultSpawnTime = 3.0f;
    float mCurrentEnemySpawnTime = 0.0f;
    //float mLevelUpSpawnTime = 1.8f;
    float mLevelUpSpawnTime = 0.1f;

    //�����o���l�{�[�i�X����
    const float mStartLevelUpBonusTime = 180.0f;
    const int mDefaultBonusTime = 20;
    //float mCurrentBonusTime = 0.0f;

    //�Q�[��������
    float mCurrentGameTime = 0.0f;
    //�Q�[���N���A����
    const float mGameClearTime = 300.0f;
    //�{�X�o�ꎞ��
    const float mGameBossSpawnTime = 210.0f;

    //�G�X�|�[����
    std::string mSpawnEnemyName = "NormalEnemyA";
    std::string mSpawnEnemyNameB = "NormalEnemyB";
    std::string mSpawnEnemyNameC = "NormalEnemyC";

    //�������x��
    const int mPlayerStartLevel = 1;
    const int mEnemyStartLevel = 1;

    //�V�[���J�ڎ���
    float mChangeSceneTime = 5.0f;

    //�t���O
    bool mGameOver = false;
    bool mGameClear = false;
};
