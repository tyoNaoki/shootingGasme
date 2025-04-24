#include "Singleton.h"

namespace {

    constexpr int kMaxFinalizersSize = 256; // 最大ファイナライザ数
    std::mutex gMutex; // 排他制御用のミューテックス
    int gNumFinalizersSize = 0; // 現在のファイナライザ数
    SingletonFinalizer::FinalizerFunc gFinalizers[kMaxFinalizersSize]; // ファイナライザの配列

}

void SingletonFinalizer::addFinalizer(FinalizerFunc func) {
    std::lock_guard<std::mutex> lock(gMutex); // ミューテックスでロック
    assert(gNumFinalizersSize < kMaxFinalizersSize); // 最大数を超えないか確認
    gFinalizers[gNumFinalizersSize++] = func; // ファイナライザを追加
}

void SingletonFinalizer::finalize() {
    std::lock_guard<std::mutex> lock(gMutex); // ミューテックスでロック
    for (int i = gNumFinalizersSize - 1; i >= 0; --i) {
        (*gFinalizers[i])(); // 逆順でファイナライザを呼び出す
    }
    gNumFinalizersSize = 0; // ファイナライザ数をリセット
}
