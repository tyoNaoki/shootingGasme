#include "UIBase.h"

namespace UI {
	//初期化
	Rect::Rect(Vector2D<int>leftTop, Vector2D<int>size, unsigned int color, bool fillFlag) : mLeftTop(leftTop), mSize(size), mColor(color), mFillFlag(fillFlag) {}

	Rect::Rect() : mLeftTop(), mSize(), mColor(GetColor(200, 200, 200)), mFillFlag(false) {}

	//枠の各頂点を取得
	std::vector<Vector2D<int>> Rect::GetVertices()const {
		return { mLeftTop,
			{mLeftTop.x + mSize.x, mLeftTop.y},
			{mLeftTop.x + mSize.x, mLeftTop.y + mSize.y},
			{mLeftTop.x, mLeftTop.y + mSize.y}
		};
	}

	Vector2D<int>Rect::RightBottom()const {
		return mLeftTop + mSize;
	}
	//各色の定義
	extern unsigned int white = GetColor(255, 255, 255);
	extern unsigned int red = GetColor(255, 0, 0);
	extern unsigned int blue = GetColor(0, 0, 255);
	extern unsigned int green = GetColor(0, 255, 0);
	extern unsigned int yellow = GetColor(255, 255, 0);
	extern unsigned int black = GetColor(0, 0, 0);
	extern unsigned int gray = GetColor(192,192,192);
	extern unsigned int lightShade = GetColor(100,100,100);
}

void UIBase::SetLocalPosition(Vector2D<float> localPosition)
{
	mLocalPosition.x = static_cast<int>(localPosition.x);
	mLocalPosition.y = static_cast<int>(localPosition.y);
}

void UIBase::SetWorldPosition(Vector2D<float> worldPosition)
{
	mWorldPosition = worldPosition;
}

void UIBase::SetVisibilty(bool isVisible)
{
	mVisible = isVisible;
}

bool UIBase::IsVisible()
{
	return mVisible;
}
