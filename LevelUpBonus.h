#pragma once
#include <string>

//���x���A�b�v���̑I�������V�̃J�e�S���[
enum class RewardType {
	WEAPON_UP,
	STATUS_UP,
	WEAPON_GET
};

class LevelUpBonus
{
public:
	//�Ή���������^�O�A��V�J�e�S���[�A��V�I�����̕\�L����閼�O�A������ݒ�
	LevelUpBonus(std::string tag,RewardType type,std::string name,std::string description);

	//�Ή���������^�O���擾
	std::string GetTag() const;
	//��V�J�e�S���[���擾
	RewardType GetType() const;
	//��V�I�����̕\�L����閼�O
	std::string GetName() const;
	//��V�I�����̕\�L��������
	std::string GetDescription() const;
	//�p����̐ݒ肳�ꂽ��V��t�^����
	virtual void Apply()const = 0;
	//���݂̃v���C���[�̃X�e�[�^�X���X�V
	void UpdateCurrentPlayer();

private:
	//����^�O
	std::string mTag;
	//��V�J�e�S���[
	RewardType mType;
	//��V�I�����̕\�L����閼�O
	std::string mName;
	//��V�I�����̕\�L��������
	std::string mDescription;
};

