#include "EffectBase.h"
#include "DxLib.h"
#include "EffekseerForDXLib.h"

static int effectResourceHandle; //エフェクトリソース
static int grBackgroundHandle; //背面画像ソース
static int grFrontHandle; //前面画像ソース
static int playtime; //エフェクトの再生時間
static float position_x; //エフェクトのx座標
static float position_y; //エフェクトのy座標
static int playingEffectHandle; //再生するエフェクトのハンドル

int Effect_Initialize()
{
    // DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
  // Effekseerを使用するには必ず設定する。
    SetUseDirect3DVersion(DX_DIRECT3D_11);
    //Effekseerを初期化する
    if (Effekseer_Init(8000) == -1)
    {
        return -1;
    }
    // フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
    // Effekseerを使用する場合は必ず設定する。
    SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
    // DXライブラリのデバイスロストした時のコールバックを設定する。
    // ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
    // ただし、DirectX11を使用する場合は実行する必要はない。
    Effekseer_SetGraphicsDeviceLostCallbackFunctions();
    // エフェクトリソースを読み込む。
    // 読み込む時に大きさを指定する。
    effectResourceHandle = LoadEffekseerEffect("Laser01.efkefc", 1.0f);
    // 何でもいいので画像を読み込む
    grBackgroundHandle = LoadGraph(_T("Texture/Background.png"));
    grFrontHandle = LoadGraph(_T("Texture/Front.png"));
    // 時間を初期化する(定期的にエフェクトを再生するため)
    playtime = 0;
    // エフェクトの表示する位置を設定する。
    position_x = 0.0f;
    position_y = 0.0f;
    // 再生中のエフェクトのハンドルを初期化する。
    playingEffectHandle = -1;
    // Zバッファを有効にする。
    // Effekseerを使用する場合、2DゲームでもZバッファを使用する。
    SetUseZBuffer3D(TRUE);
    // Zバッファへの書き込みを有効にする。
    // Effekseerを使用する場合、2DゲームでもZバッファを使用する。
    SetWriteZBuffer3D(TRUE);
    return 0;
}

void Effect_Update()
{
    // DXライブラリのカメラを設定する。
    SetCameraPositionAndTarget_UpVecY(VGet(10, 10, -20), VGet(0, 0, 0));
    SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
    SetCameraNearFar(1.0f, 150.0f);
    // DXライブラリのカメラとEffekseerのカメラを同期する。
    Effekseer_Sync3DSetting();
    // 定期的にエフェクトを再生する
    if (playtime % 60 == 0)
    {
        // エフェクトを再生する。
        playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);
        // エフェクトの位置をリセットする。
        position_x = 0.0f;
    }
    // 再生中のエフェクトを移動する。
    SetPosPlayingEffekseer3DEffect(playingEffectHandle, position_x, position_y, 0);
    position_x += 0.2f;
    // Effekseerにより再生中のエフェクトを更新する。
    UpdateEffekseer3D();
    // 時間を経過させる。
    playtime++;
}

void Effect_Draw()
{
    // 何でもいいので画像を描画する。
    // こうして描画した後でないと、Effekseerは描画できない。
    DrawGraph(0, 0, grBackgroundHandle, TRUE);
    // 3Dを表示する。
    DrawCapsule3D(
        VGet(0.0f, 100.0f, 0.0f), VGet(0.0f, -100.0f, 0.0f), 6.0f, 16, GetColor(100, 100, 100), GetColor(255, 255, 255), TRUE);
    // Effekseerにより再生中のエフェクトを描画する。
    DrawEffekseer3D();
    // エフェクトの上にも画像を描画できる。
    DrawGraph(0, 0, grFrontHandle, TRUE);
}

void Effect_Finalize()
{
    // エフェクトリソースを削除する。(Effekseer終了時に破棄されるので削除しなくてもいい)
    DeleteEffekseerEffect(effectResourceHandle);
    // Effekseerを終了する。
    Effkseer_End();
}
