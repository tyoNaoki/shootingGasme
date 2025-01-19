#pragma once
#include "Vector2D.h"
#include "DxLib.h"
#include <vector>

namespace UI {
	struct Rect {
		Vector2D<int>mLeftTop; // ����̍��W 
		Vector2D<int>mSize; // ��`�̕��ƍ��� 
		unsigned int mColor;
		bool mFillFlag;

		Rect(Vector2D<int>leftTop, Vector2D<int>size, unsigned int color, bool fillFlag);
		Rect();
		//�e�S�̒��_���W
		std::vector<Vector2D<int>>GetVertices()const;

		Vector2D<int>RightBottom()const;
	};
	//�e�F�̒�`
	extern unsigned int white;
	extern unsigned int red;
	extern unsigned int blue;
	extern unsigned int green;
	extern unsigned int yellow;
	extern unsigned int black;
	extern unsigned int gray;
	extern unsigned int lightShade;
}

class UIBase
{
public:
	virtual ~UIBase() = default;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(float deltaTime) = 0;
	//�`��ʒu�ݒ�
	virtual void SetLocalPosition(Vector2D<float>localPosition);
	//�`�惏�[���h���W�ݒ�
	void SetWorldPosition(Vector2D<float>worldPosition);
	//�\���A��\���؂�ւ�
	void SetVisibilty(bool isVisible);
	//�\�����邩
	bool IsVisible();

protected:

	Vector2D<int>mLocalPosition;

	Vector2D<float>mWorldPosition;

	bool mVisible = true;
};

