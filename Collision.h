#pragma once

#ifndef COLLISION_H 
#define COLLISION_H

#include <vector>
#include "Vector2D.h"

class Actor;

namespace Collision{
	class Line {
	public:
		Vector2D<float> mStart;
		Vector2D<float> mEnd;

		Line(Vector2D<float>start, Vector2D<float>end) : mStart(start), mEnd(end) {}
		Line() : mStart(Vector2D<float>()), mEnd(Vector2D<float>()) {}
	};

	class Rect {
	public:
		Vector2D<float>mLeftTop; // ����̍��W 
		Vector2D<float>mSize; // ��`�̕��ƍ��� 

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
	};

	class RotatedRect {
	public:
		Vector2D<float> center;
		Vector2D<float> halfExtend;
		float angle; // ��]�p�x�i���W�A��GetVertices�j

		std::vector<Line> GetRotatedRectLines();

		std::vector<Vector2D<float>> GetVertices();

		//Vector2D<float>GetTopMiddle();
		//Vector2D<float>GetBottomMiddle();
	};

	class Circle {
	public:
		Vector2D<float> mCenter; // ���S���W 
		float mRadius; // ���a 
		Circle(Vector2D<float>center, float radius) : mCenter(center), mRadius(radius) {}
		Circle() :mCenter(), mRadius() {}
	};

	RotatedRect CalculateAttackRange(const Vector2D<float>& position, float attackWidth, float attackHeight, float angle);

	//��`�����`�F�b�N�֐�
	bool IsPointInsideRect(const Vector2D<float>& point, const Rect& rect);

	//�����Ƌ�`�̕ӂ̌�������֐�
	bool DoLineSegmentsIntersect(const Vector2D<float>& p1, const Vector2D<float>& p2, const Vector2D<float>& q1, const Vector2D<float>& q2);

	//��`�����蔻��֐�
	bool IsColliding(const Rect& r1, const Rect& r2);

	//��`�̉����߂������֐�
	void PushBackRect(Rect& r1, Rect& r2);

	void PushBackActorCollision(Rect& r1, Rect& r2);

	//void PushBackActorCollision(Rect& newRect, const std::vector<Rect>& otherRects, const Rect& oldRect);

	//�l�p�`�Ɛ����̓����蔻��֐�
	bool IsLineIntersectingRect(const Line& line, const Rect& rect);

	//�~�̓����蔻��֐�
	bool IsColliding(const Circle& c1, const Circle& c2);

	bool IsColliding(const Circle& c1, const Rect& r1);

	bool IsColliding(RotatedRect& rotRect, const Rect& rect);

	std::pair<float, float> Project(const Rect& rect, const Vector2D<float>& axis);

	Collision::Line GetOrientedRectangleEdge(Collision::RotatedRect orientedRectangle, int point);

	std::pair<float, float> ProjectOntoAxis(const std::vector<Vector2D<float>>& vertices, const Vector2D<float>& axis);

	bool IsOverlapping(const std::pair<float, float>& proj1, const std::pair<float, float>& proj2);
}

#endif // COLLISION_H


