#pragma once

#ifndef COLLISION_H 
#define COLLISION_H

#include <vector>
#include "Vector2D.h"
#include <functional>

class Actor;

namespace Collision{
	class Shape {
	public:
		virtual ~Shape() = default;

		virtual bool IsHit(Collision::Shape& shape) = 0;

		void SetCollisionCallback(const std::function<void(std::shared_ptr<Actor>)>& callback);

		void HandleCollision(std::shared_ptr<Actor>hitActor);

	private:
		std::function<void(std::shared_ptr<Actor>)> mCollisionCallback; // 衝突時のコールバック関数

	};

	class Line : public Shape{
	public:
		Vector2D<float> mStart;
		Vector2D<float> mEnd;

		Line(Vector2D<float>start, Vector2D<float>end) : mStart(start), mEnd(end) {}
		Line() : mStart(Vector2D<float>()), mEnd(Vector2D<float>()) {}

		bool IsHit(Collision::Shape& shape)override;
	};

	class Rect : public Shape{
	public:
		Vector2D<float>mLeftTop; // 左上の座標 
		Vector2D<float>mSize; // 矩形の幅と高さ 

		Rect(Vector2D<float>leftTop, Vector2D<float>size) : mLeftTop(leftTop), mSize(size) {}
		Rect():mLeftTop(),mSize(){}

		Vector2D<float> RightBottom() const {
			return Vector2D<float>(mLeftTop.x + mSize.x, mLeftTop.y + mSize.y);
		}

		std::vector<Vector2D<float>>GetVertices()const{
			return { mLeftTop,
				{mLeftTop.x + mSize.x, mLeftTop.y},
				{mLeftTop.x + mSize.x, mLeftTop.y + mSize.y},
				{mLeftTop.x, mLeftTop.y + mSize.y}
			};
		}

		bool IsHit(Collision::Shape& shape) override;
	};

	class RotatedRect : public Shape{
	public:
		Vector2D<float> center;
		Vector2D<float> halfExtend;
		float angle; // 回転角度（ラジアンGetVertices）

		std::vector<Line> GetRotatedRectLines();

		std::vector<Vector2D<float>> GetVertices();

		bool IsHit(Collision::Shape& shape)override;

		//Vector2D<float>GetTopMiddle();
		//Vector2D<float>GetBottomMiddle();
	};

	class Circle :public Shape{
	public:
		Vector2D<float> mCenter; // 中心座標 
		float mRadius; // 半径 
		Circle(Vector2D<float>center, float radius) : mCenter(center), mRadius(radius) {}
		Circle() :mCenter(), mRadius() {}

		bool IsHit(Collision::Shape& shape)override;
	};

	RotatedRect CalculateAttackRange(const Vector2D<float>& position, float attackWidth, float attackHeight, float angle);

	//矩形内部チェック関数
	bool IsPointInsideRect(const Vector2D<float>& point, const Rect& rect);

	//線分と矩形の辺の交差判定関数
	bool DoLineSegmentsIntersect(const Vector2D<float>& p1, const Vector2D<float>& p2, const Vector2D<float>& q1, const Vector2D<float>& q2);

	//矩形当たり判定関数
	bool IsColliding(const Rect& r1, const Rect& r2);

	//矩形の押し戻し処理関数
	void PushBackRect(Rect& r1, Rect& r2);

	void PushBackActorCollision(Rect& r1, Rect& r2);

	//void PushBackActorCollision(Rect& newRect, const std::vector<Rect>& otherRects, const Rect& oldRect);

	//四角形と線分の当たり判定関数
	bool IsLineIntersectingRect(const Line& line, const Rect& rect);

	bool IsColliding(Actor* actor,Actor* actor2);

	//円の当たり判定関数
	bool IsColliding(const Circle& c1, const Circle& c2);

	bool IsColliding(const Circle& c1, const Rect& r1);

	bool IsColliding(RotatedRect& rotRect, const Rect& rect);

	std::pair<float, float> Project(const Rect& rect, const Vector2D<float>& axis);

	Collision::Line GetOrientedRectangleEdge(Collision::RotatedRect orientedRectangle, int point);

	std::pair<float, float> ProjectOntoAxis(const std::vector<Vector2D<float>>& vertices, const Vector2D<float>& axis);

	bool IsOverlapping(const std::pair<float, float>& proj1, const std::pair<float, float>& proj2);
}

#endif // COLLISION_H


