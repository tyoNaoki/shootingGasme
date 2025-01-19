#pragma once
#include "Vector2D.h"
#include "DxLib.h"
#include <vector>

namespace UI {
	struct Rect {
		Vector2D<int>mLeftTop; // 左上の座標 
		Vector2D<int>mSize; // 矩形の幅と高さ 
		unsigned int mColor;
		bool mFillFlag;

		Rect(Vector2D<int>leftTop, Vector2D<int>size, unsigned int color, bool fillFlag);
		Rect();
		//各４つの頂点座標
		std::vector<Vector2D<int>>GetVertices()const;

		Vector2D<int>RightBottom()const;
	};
	//各色の定義
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
	//描画位置設定
	virtual void SetLocalPosition(Vector2D<float>localPosition);
	//描画ワールド座標設定
	void SetWorldPosition(Vector2D<float>worldPosition);
	//表示、非表示切り替え
	void SetVisibilty(bool isVisible);
	//表示するか
	bool IsVisible();

protected:

	Vector2D<int>mLocalPosition;

	Vector2D<float>mWorldPosition;

	bool mVisible = true;
};

