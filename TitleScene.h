#pragma once
#include "Scene.h"
#include "Vector2D.h"

class TitleScene :
    public Scene
{
public:
	TitleScene();

	~TitleScene();
	//������
	bool Initialize() override;
	//�X�V�A�`��
	void Update(const float deltaTime) override;
	void Render(const float deltaTime) override;
	//���݂̃}�b�v�擾
	std::shared_ptr<Map>GetMap() const override;
	//�I������
	void End() override;

private:
	bool mInitialze = false;

	int mHandle = -1;
};

