#include <math.h>
#include "DxLib.h"
#include "FpsControll.h"

static int mStartTime;      //測定開始時刻
static int mCount;          //カウンタ
static float mFps;          //fps
static const int N = 60;  //平均を取るサンプル数
static const int FPS = 60;  //設定したFPS
//初期化
void FpsControll_Initialize() {
    mStartTime = GetNowCount();
    mCount = 0;
    mFps = 0;
}
//FPS制御
bool FpsControll_Update() {
    if (mCount == 0) { //1フレーム目なら時刻を記憶
        mStartTime = GetNowCount();
    }
    if (mCount == N) { //60フレーム目なら平均を計算する
        int t = GetNowCount();
        mFps = 1000.f / ((t - mStartTime) / (float)N);
        mCount = 0;
        mStartTime = t;
    }
    mCount++;
    return true;
}
//FPS表示
void FpsControll_Draw() {
    int width,height;
    GetWindowSize(&width,&height);
    DrawFormatString(width-50, 0, GetColor(255, 255, 255), "%.1f", mFps);
}
void FpsControll_Wait() {
    int tookTime = GetNowCount() - mStartTime;  //かかった時間
    int waitTime = mCount * 1000 / FPS - tookTime;  //待つべき時間
    if (waitTime > 0) {
        Sleep(waitTime);  //待機
    }
}
