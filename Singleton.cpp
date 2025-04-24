#include "Singleton.h"

namespace {

    constexpr int kMaxFinalizersSize = 256; // �ő�t�@�C�i���C�U��
    std::mutex gMutex; // �r������p�̃~���[�e�b�N�X
    int gNumFinalizersSize = 0; // ���݂̃t�@�C�i���C�U��
    SingletonFinalizer::FinalizerFunc gFinalizers[kMaxFinalizersSize]; // �t�@�C�i���C�U�̔z��

}

void SingletonFinalizer::addFinalizer(FinalizerFunc func) {
    std::lock_guard<std::mutex> lock(gMutex); // �~���[�e�b�N�X�Ń��b�N
    assert(gNumFinalizersSize < kMaxFinalizersSize); // �ő吔�𒴂��Ȃ����m�F
    gFinalizers[gNumFinalizersSize++] = func; // �t�@�C�i���C�U��ǉ�
}

void SingletonFinalizer::finalize() {
    std::lock_guard<std::mutex> lock(gMutex); // �~���[�e�b�N�X�Ń��b�N
    for (int i = gNumFinalizersSize - 1; i >= 0; --i) {
        (*gFinalizers[i])(); // �t���Ńt�@�C�i���C�U���Ăяo��
    }
    gNumFinalizersSize = 0; // �t�@�C�i���C�U�������Z�b�g
}
