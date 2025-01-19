#pragma once
#include <cassert>
#include <mutex>

// シングルトンクラスの終了処理を行うためのクラス
class SingletonFinalizer {
public:
    // 終了処理関数の型定義
    using FinalizerFunc = void(*)();
    // 終了処理関数を追加するための静的関数
    static void addFinalizer(FinalizerFunc func);
    // 追加された終了処理関数を実行する静的関数
    static void finalize();
};

// シングルトンクラスの定義
template <typename T>
class Singleton final {
public:
    // シングルトンのインスタンスを取得する関数
    static T& get_instance() {
        // 初回呼び出し時に一度だけインスタンスを作成する
        std::call_once(initFlag, create);
        assert(instance); // インスタンスが正しく作成されたことを確認
        return *instance; // インスタンスを返す
    }

private:
    // シングルトンのインスタンスを作成する関数
    static void create() {
        instance = new T; // 新しいインスタンスを作成
        // インスタンスの終了処理をSingletonFinalizerに追加
        SingletonFinalizer::addFinalizer(&Singleton<T>::destroy);
    }

    // シングルトンのインスタンスを破棄する関数
    static void destroy() {
        delete instance; // インスタンスを削除
        instance = nullptr; // ポインタをリセット
    }

    static std::once_flag initFlag; // インスタンス作成のためのフラグ
    static T* instance; // シングルトンのインスタンスへのポインタ
};

// 静的メンバ変数の初期化
template <typename T> std::once_flag Singleton<T>::initFlag;
template <typename T> T* Singleton<T>::instance = nullptr;

