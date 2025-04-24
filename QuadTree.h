#pragma once
#include <cstdint>
#include "Collision.h"
#include "DebugHelper.h"
#include "SceneManager.h"
#include "CharacterBase.h"

#define WIN_W 1920
#define WIN_H 1080


    // �ő�[��depth�̎l���؂̃Z����, 1 + 4 + 16 + ... + 4^depth = {(2^2)^(depth+1)) - 1} / 3
    static constexpr uint64_t sum_of_tree(const uint64_t depth) {
        return ((((uint64_t)1) << (2 * depth + 2)) - 1) / 3;
    }
    
    // calc 2^depth, �ő�[��depth�̎l���؂̈�ӂ̕������ɑ���
    static constexpr uint64_t MAX_SPLIT(const uint64_t depth) {
        return ((uint64_t)1) << depth;
    }
    
    static const uint64_t offset[16] = { 0, 1, 5, 21, 85, 341, 1365, 5461, 21845, 87381, 349525, 1398101, 5592405, 22369621,    89478485, 357913941 }; // (4^n - 1) / 3
    
    // for GetMSBPos(), magic-number table
    static const unsigned char s_abyLog2[32] = {
         1,  2, 29,  3, 30, 20, 25,  4, 31, 23, 21, 11, 26, 13, 16,  5,
        32, 28, 19, 24, 22,  10, 12, 15, 27, 18,  9, 14, 17,  8,  7,  6
    };

    /*
     * �ő� 2^16 x 2^16 �܂ł̕���(= MAX_D<=15)�ɑΉ�
     * �Ⴆ�� 8x8�����Ȃ� MAX_D=3
     */
     
template <unsigned int MAX_D>
class QuadTree
{
public:
    /*
     * �I�u�W�F�N�g���l���؂ɓo�^����
     */
    void Push(Actor* obj)
    {
        const auto id = GetCellID(obj);
        this->cell[id].push_back(obj);
    }

    /*
     * ���t���[���ɔ����Ė؂̏�Ԃ����Z�b�g����
     */
    void Cleanup()
    {
        for (int i = 0; i < sum_of_tree(MAX_D); i++) {
            cell[i].clear();
        }
    }

private:
    /*
     * �[��depth��z�l��z_value�����m�[�h�ɓo�^���ꂽ�I�u�W�F�N�g�ɑ΂��ďՓ˔�����s��
     */
    void HitTest(uint64_t depth, uint64_t z_value, std::vector<Actor*>& stack)
    {
        const uint64_t id = GetCellID(depth, z_value);
        const std::vector<Actor*>& lst = cell[id];
        const uint64_t lst_size = lst.size();

        // �Փ˔�������Ă���
        for (int i = 0; i < lst_size; i++) {
            //auto c1 = lst[i]->GetCollision();

            // �X�^�b�N�ƃm�[�h
            for (int j = 0; j < stack.size(); j++) {
                //�Փ�
                if (Collision::IsColliding(lst[i],stack[j])) {
                    /*
                    //Collision::PushBackRect(c1, c2);
                    auto newPosi = c1.mLeftTop + (c1.mSize / 2);
                    auto localPosi = SCENE_M.GetCurrentScene()->GetMap()->GetLocalFromWorldPosition(newPosi);
                    //���W�X�V
                    lst[i]->SetLocalPosition2D(localPosi);
                    lst[i]->SetWorldPosition2D(newPosi);

                    DEBUG_HELPER.DrawCollision(c1);
                    DEBUG_HELPER.DrawCollision(c2);
                    */
                }
            }

            // �m�[�h�����m
            for (int j = i + 1; j < lst_size; j++) {
                //�Փ�
                if (Collision::IsColliding(lst[i], lst[j])) {
                    
                    /*
                    Collision::PushBackRect(lst[i],lst[j]);
                    auto newPosi = c1.mLeftTop + (c1.mSize / 2);
                    auto localPosi = SCENE_M.GetCurrentScene()->GetMap()->GetLocalFromWorldPosition(newPosi);
                    //���W�X�V
                    lst[i]->SetLocalPosition2D(localPosi);
                    lst[i]->SetWorldPosition2D(newPosi);

                    DEBUG_HELPER.DrawCollision(c1);
                    DEBUG_HELPER.DrawCollision(c2);
                    */
                    
                }
            }
        }

        // �[�����ő�Ȃ�I��
        if (depth == MAX_D)
            return;

        // �X�^�b�N�ɐς�
        for (int i = 0; i < lst.size(); i++) {
            stack.push_back(lst[i]);
        }

        // �q��Ԃ֍ċA
        for (uint8_t i = 0; i < 4; i++) {
            this->HitTest(depth + 1, (z_value << 2) + i, stack);
        }
        // �X�^�b�N����~�낷
        for (int i = 0; i < lst_size; i++) {
            stack.pop_back();
        }
        return;
    }

public:
    /*
     * �o�^����Ă���S�I�u�W�F�N�g�ɑ΂��ďՓ˔�����s��
     */
    void HitTest()
    {
        std::vector<Actor*> stack;
        this->HitTest(0, 0, stack);
    }

private:
    std::vector<Actor*> cell[sum_of_tree(MAX_D)]; // tree of vector<T*>

    // �[����z�l����m�[�h�ԍ��𓾂�
    static constexpr uint64_t GetCellID(uint64_t depth, uint64_t z_value)
    {
        return  offset[depth] + z_value;
    }

    // �ő�16bit�̓��͂��Abit�����΂��ɂ��ďo�͂���B
    static uint64_t BitSeparate16(uint64_t n)
    {
        n = (n | (n << 8)) & (uint64_t)0x00FF00FF; // 9~16bit
        n = (n | (n << 4)) & (uint64_t)0x0F0F0F0F; // 5~8bit
        n = (n | (n << 2)) & (uint64_t)0x33333333; // 3~4 bit
        n = (n | (n << 1)) & (uint64_t)0x55555555; // 1~2bit
        return n;
    }

    // �ő�16bit�̓���x, y����32bit��z�l�𓾂�
    // x, y�͍ŏ������P�ʂ̃O���b�h��ō��ォ��E��x, ����y�ړ��̈Ӗ�
    static constexpr uint64_t GetZ(uint64_t x, uint64_t y)
    {
        return (BitSeparate16(x) | (BitSeparate16(y) << 1));
    }

    // 32bit�̐�����MSB�̈ʒu��1-indexed�œ���
    // 0x00000001�̏ꍇ��1��Ԃ�
    // 0x00800F00�̏ꍇ��24��Ԃ�
    // 0x00000000�̏ꍇ�͌���`�I
    static constexpr uint64_t GetMSBPos(uint32_t uVal)
    {
        /* Propagates MSB to all the lower bits. */
        uVal |= (uVal >> 1); // 2~1 bit
        uVal |= (uVal >> 2); // 4~3 bit
        uVal |= (uVal >> 4); // 8~5 bit
        uVal |= (uVal >> 8); // 16~9 bit
        uVal |= (uVal >> 16); // 32~17 bit
        /* Turns off all the bits except MSB. */
        uVal >>= 1;
        uVal++;
        /* Parameter uVal must be a power of 2. */
        const uint32_t MAGIC = 0x07D6E531;
        uVal = (MAGIC * uVal) >> 27;

        return (int64_t)s_abyLog2[uVal & 0x1F];
    }

    // �Ίp��2�_����m�[�h�ԍ��𓾂�
    // (x1, y1) <= (x2, y2)�Ƃ���
    static uint64_t GetCellID(float x1, float y1, float x2, float y2)
    {
        // �P�ʒ����`�̕��ƍ���
        constexpr float DX = static_cast<float>(WIN_W) / MAX_SPLIT(MAX_D);
        constexpr float DY = static_cast<float>(WIN_H) / MAX_SPLIT(MAX_D);
        constexpr int64_t MAX_SPLIT_NUM = MAX_SPLIT(MAX_D);
        // 0 ~ MAX_SPLIT-1 �̃O���b�h���W�֕ϊ�
        int64_t ix1 = (int64_t)(x1 / DX);
        int64_t iy1 = (int64_t)(y1 / DY);
        int64_t ix2 = (int64_t)(x2 / DX);
        int64_t iy2 = (int64_t)(y2 / DY);
        if (ix1 < 0 || ix2 >= MAX_SPLIT_NUM) {
            ix1 = ix1 < 0 ? 0 : ix1 >= MAX_SPLIT_NUM ? MAX_SPLIT_NUM - 1 : ix1;
            ix2 = ix2 < 0 ? 0 : ix2 >= MAX_SPLIT_NUM ? MAX_SPLIT_NUM - 1 : ix2;
        }
        if (iy1 < 0 || iy2 >= MAX_SPLIT_NUM) {
            iy1 = iy1 < 0 ? 0 : iy1 >= MAX_SPLIT_NUM ? MAX_SPLIT_NUM - 1 : iy1;
            iy2 = iy2 < 0 ? 0 : iy2 >= MAX_SPLIT_NUM ? MAX_SPLIT_NUM - 1 : iy2;
        }
        // 0 <= ix1, iy1, ix2, iy2 < MAX_SPLIT_NUM <= 2^16
        const uint64_t x = ix1 xor ix2;
        const uint64_t y = iy1 xor iy2;
        const uint64_t xy = x | y;
        const uint64_t z = GetZ(ix1, iy1);
        if (xy == 0) {
            return offset[MAX_D] + z;
        }
        uint64_t msb_pos = GetMSBPos(static_cast<uint32_t>(xy)); // 1-indexed, assume xy < 2^32
        uint64_t L = MAX_D - msb_pos;
        return offset[L] + (z >> (msb_pos << 1));
    }

    uint64_t GetCellID(Actor* actor)
    {
        /*
        if(actor->GetActorType()==CharacterType::ENEMY)
        {
            CharacterBase* chara = dynamic_cast<CharacterBase*>(actor);

            if(!chara) return -1;

            auto left = SCENE_M.GetCurrentScene()->GetMap()->GetLocalFromWorldPosition(chara->GetCollision().mLeftTop);
            auto c = Collision::Rect(left, chara->GetCollision().mSize).GetVertices();
            return GetCellID(c[0].x, c[0].y, c[2].x, c[2].y);
        }
        */
        return -1;
    }
};