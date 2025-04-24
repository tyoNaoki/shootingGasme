#pragma once
#include "UIBase.h"
#include <unordered_map>

class UIText;

class GameOverPanel :
    public UIBase
{
public:
	//�Q�[���I�[�o�[�e�L�X�g�A���x���A�X�R�A�A���j���̕\���p�l���̍쐬����
	GameOverPanel(const int width,const int height);
	
	~GameOverPanel();
	//�X�V�A�`��
	void Update(float deltaTime) override;
	void Draw(float deltaTime) override;

private:
	//UI�ǉ�
	void AddUIElement(std::shared_ptr<UIText>ui);
	//UI���X�g
	std::vector<std::shared_ptr<UIText>>mUIList;
};

