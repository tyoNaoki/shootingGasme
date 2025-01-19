#pragma once
#include "Singleton.h"
#include "UIBase.h"
#include "UIText.h"
#include "HPBase.h"
#include "UITime.h"
#include "MenuPanel.h"
#include "HUD.h"
#include "GameClearPanel.h"
#include "GameOverPanel.h"
#include "TitlePanel.h"
#include "MenuScenePanel.h"
#include "LevelUpPanel.h"
#include "Canvas.h"
#include <unordered_map>
#include <vector>

#define UI_M Singleton<UIManager>::get_instance()

class UIManager
{
friend class Singleton<UIManager>;

public:
	//�L�����o�X�ǉ�
	void AddCanvas(std::string name,const std::shared_ptr<Canvas>);
	//�Ώۂ̖��O�̃L�����o�X���擾
	std::shared_ptr<Canvas>GetCanvas(std::string name);
	//�X�V�A�`��
	void Update(float deltaTime);
	void Draw(float deltaTime);

	//�L�����o�X�̍폜
	void ClearCanvas();
	bool RemoveCanvas(std::string name);
	//�L�����o�X�̕\���A��\��
	void SetVisibiltyToCanvas(std::string name,bool isVisibilty);

private:
	~UIManager();

	std::unordered_map<std::string,std::shared_ptr<Canvas>>mCanvasList;
};

