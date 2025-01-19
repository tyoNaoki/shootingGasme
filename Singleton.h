#pragma once
#include <cassert>
#include <mutex>

// �V���O���g���N���X�̏I���������s�����߂̃N���X
class SingletonFinalizer {
public:
    // �I�������֐��̌^��`
    using FinalizerFunc = void(*)();
    // �I�������֐���ǉ����邽�߂̐ÓI�֐�
    static void addFinalizer(FinalizerFunc func);
    // �ǉ����ꂽ�I�������֐������s����ÓI�֐�
    static void finalize();
};

// �V���O���g���N���X�̒�`
template <typename T>
class Singleton final {
public:
    // �V���O���g���̃C���X�^���X���擾����֐�
    static T& get_instance() {
        // ����Ăяo�����Ɉ�x�����C���X�^���X���쐬����
        std::call_once(initFlag, create);
        assert(instance); // �C���X�^���X���������쐬���ꂽ���Ƃ��m�F
        return *instance; // �C���X�^���X��Ԃ�
    }

private:
    // �V���O���g���̃C���X�^���X���쐬����֐�
    static void create() {
        instance = new T; // �V�����C���X�^���X���쐬
        // �C���X�^���X�̏I��������SingletonFinalizer�ɒǉ�
        SingletonFinalizer::addFinalizer(&Singleton<T>::destroy);
    }

    // �V���O���g���̃C���X�^���X��j������֐�
    static void destroy() {
        delete instance; // �C���X�^���X���폜
        instance = nullptr; // �|�C���^�����Z�b�g
    }

    static std::once_flag initFlag; // �C���X�^���X�쐬�̂��߂̃t���O
    static T* instance; // �V���O���g���̃C���X�^���X�ւ̃|�C���^
};

// �ÓI�����o�ϐ��̏�����
template <typename T> std::once_flag Singleton<T>::initFlag;
template <typename T> T* Singleton<T>::instance = nullptr;

