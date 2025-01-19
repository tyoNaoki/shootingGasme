#include "EffectBase.h"
#include "DxLib.h"
#include "EffekseerForDXLib.h"

static int effectResourceHandle; //�G�t�F�N�g���\�[�X
static int grBackgroundHandle; //�w�ʉ摜�\�[�X
static int grFrontHandle; //�O�ʉ摜�\�[�X
static int playtime; //�G�t�F�N�g�̍Đ�����
static float position_x; //�G�t�F�N�g��x���W
static float position_y; //�G�t�F�N�g��y���W
static int playingEffectHandle; //�Đ�����G�t�F�N�g�̃n���h��

int Effect_Initialize()
{
    // DirectX11���g�p����悤�ɂ���B(DirectX9���A�ꕔ�@�\�s��)
  // Effekseer���g�p����ɂ͕K���ݒ肷��B
    SetUseDirect3DVersion(DX_DIRECT3D_11);
    //Effekseer������������
    if (Effekseer_Init(8000) == -1)
    {
        return -1;
    }
    // �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
    // Effekseer���g�p����ꍇ�͕K���ݒ肷��B
    SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
    // DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
    // �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
    // �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
    Effekseer_SetGraphicsDeviceLostCallbackFunctions();
    // �G�t�F�N�g���\�[�X��ǂݍ��ށB
    // �ǂݍ��ގ��ɑ傫�����w�肷��B
    effectResourceHandle = LoadEffekseerEffect("Laser01.efkefc", 1.0f);
    // ���ł������̂ŉ摜��ǂݍ���
    grBackgroundHandle = LoadGraph(_T("Texture/Background.png"));
    grFrontHandle = LoadGraph(_T("Texture/Front.png"));
    // ���Ԃ�����������(����I�ɃG�t�F�N�g���Đ����邽��)
    playtime = 0;
    // �G�t�F�N�g�̕\������ʒu��ݒ肷��B
    position_x = 0.0f;
    position_y = 0.0f;
    // �Đ����̃G�t�F�N�g�̃n���h��������������B
    playingEffectHandle = -1;
    // Z�o�b�t�@��L���ɂ���B
    // Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
    SetUseZBuffer3D(TRUE);
    // Z�o�b�t�@�ւ̏������݂�L���ɂ���B
    // Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
    SetWriteZBuffer3D(TRUE);
    return 0;
}

void Effect_Update()
{
    // DX���C�u�����̃J������ݒ肷��B
    SetCameraPositionAndTarget_UpVecY(VGet(10, 10, -20), VGet(0, 0, 0));
    SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
    SetCameraNearFar(1.0f, 150.0f);
    // DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
    Effekseer_Sync3DSetting();
    // ����I�ɃG�t�F�N�g���Đ�����
    if (playtime % 60 == 0)
    {
        // �G�t�F�N�g���Đ�����B
        playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);
        // �G�t�F�N�g�̈ʒu�����Z�b�g����B
        position_x = 0.0f;
    }
    // �Đ����̃G�t�F�N�g���ړ�����B
    SetPosPlayingEffekseer3DEffect(playingEffectHandle, position_x, position_y, 0);
    position_x += 0.2f;
    // Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
    UpdateEffekseer3D();
    // ���Ԃ��o�߂�����B
    playtime++;
}

void Effect_Draw()
{
    // ���ł������̂ŉ摜��`�悷��B
    // �������ĕ`�悵����łȂ��ƁAEffekseer�͕`��ł��Ȃ��B
    DrawGraph(0, 0, grBackgroundHandle, TRUE);
    // 3D��\������B
    DrawCapsule3D(
        VGet(0.0f, 100.0f, 0.0f), VGet(0.0f, -100.0f, 0.0f), 6.0f, 16, GetColor(100, 100, 100), GetColor(255, 255, 255), TRUE);
    // Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
    DrawEffekseer3D();
    // �G�t�F�N�g�̏�ɂ��摜��`��ł���B
    DrawGraph(0, 0, grFrontHandle, TRUE);
}

void Effect_Finalize()
{
    // �G�t�F�N�g���\�[�X���폜����B(Effekseer�I�����ɔj�������̂ō폜���Ȃ��Ă�����)
    DeleteEffekseerEffect(effectResourceHandle);
    // Effekseer���I������B
    Effkseer_End();
}
