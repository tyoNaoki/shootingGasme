#include "Collision.h"
#include <iostream>
#include <utility>
#include "Actor.h"
#include <string>

namespace Collision{
	RotatedRect CalculateAttackRange(const Vector2D<float>& position, float attackWidth, float attackHeight, float angle)
	{
		float attackOffset = 0;
		// 角度に基づいて攻撃範囲の中心点を計算
		float radian = Vector2D<float>::GetRadiansFromDegrees(angle);
		float centerX = position.x + cos(radian) * attackHeight / 2;
		float centerY = position.y + sin(radian) * attackHeight / 2;

		Collision::RotatedRect attackRange;

		// 矩形の左上の座標を計算
		attackRange.center = { centerX,centerY };
		attackRange.halfExtend.x = attackHeight / 2;
		attackRange.halfExtend.y = attackWidth / 2;
		attackRange.angle = radian;

		return attackRange;
	}

	bool IsPointInsideRect(const Vector2D<float>& point, const Rect& rect) {
		return point.x >= rect.mLeftTop.x &&
			point.x <= rect.RightBottom().x &&
			point.y >= rect.mLeftTop.y &&
			point.y <= rect.RightBottom().y;
	}
	 
	bool DoLineSegmentsIntersect(const Vector2D<float>& p1, const Vector2D<float>& p2, const Vector2D<float>& q1, const Vector2D<float>& q2) {
		auto ccw = [](const Vector2D<float>& a, const Vector2D<float>& b, const Vector2D<float>& c) {
			return (c.y - a.y) * (b.x - a.x) > (b.y - a.y) * (c.x - a.x); };

		return (ccw(p1, q1, q2) != ccw(p2, q1, q2)) && (ccw(p1, p2, q1) != ccw(p1, p2, q2));
	}

	bool IsColliding(const Rect& r1, const Rect& r2) {
		return (r1.mLeftTop.x < r2.RightBottom().x &&
			r1.RightBottom().x > r2.mLeftTop.x &&
			r1.mLeftTop.y < r2.RightBottom().y &&
			r1.RightBottom().y > r2.mLeftTop.y);
	}
	
	void PushBackRect(Rect& r1, Rect& r2) {
		float overlapLeft = r2.RightBottom().x - r1.mLeftTop.x;
		float overlapRight = r1.RightBottom().x - r2.mLeftTop.x;
		float overlapTop = r2.RightBottom().y - r1.mLeftTop.y;
		float overlapBottom = r1.RightBottom().y - r2.mLeftTop.y;
		
		bool fromLeft = overlapLeft < overlapRight;
		bool fromTop = overlapTop < overlapBottom;
		
		float minOverlapX = fromLeft ? overlapLeft : overlapRight;
		float minOverlapY = fromTop ? overlapTop : overlapBottom;
		
		if (minOverlapX < minOverlapY) {
			r1.mLeftTop.x -= fromLeft ? -minOverlapX : minOverlapX;
		} else {
			r1.mLeftTop.y -= fromTop ? -minOverlapY : minOverlapY;
		}
	}
	

	/*
	void PushBackRect(Rect& r1, Rect& r2) {
		Vector2D<float>halfSize(r1.mSize / 2);
		Vector2D<float>halfSize2(r2.mSize / 2);

		// 各矩形の中心点を計算 
		Vector2D<float> center1 = r1.mLeftTop + halfSize; 
		Vector2D<float> center2 = r2.mLeftTop + halfSize2;


		// 重なりの計算 
		float overlapLeft = (center2.x + halfSize2.x) - (center1.x - halfSize.x);
		float overlapRight = (center1.x + halfSize.x) - (center2.x - halfSize2.x);
		float overlapTop = (center2.y + halfSize2.y) - (center1.y - halfSize.y);
		float overlapBottom = (center1.y + halfSize.y) - (center2.y - halfSize2.y);
		
		// どちらの方向から衝突しているかを判定
		bool fromLeft = overlapLeft < overlapRight;
		bool fromTop = overlapTop < overlapBottom;
		
		// 最小の重なり量を算出 
		float minOverlapX = fromLeft ? overlapLeft : overlapRight;
		float minOverlapY = fromTop ? overlapTop : overlapBottom;
		
		// 水平方向と垂直方向でどちらの重なりが少ないかを判定し、押し戻す 
		if (minOverlapX < minOverlapY) {
			r1.mLeftTop.x += fromLeft ? -minOverlapX : minOverlapX; 
		} else {
			r1.mLeftTop.y += fromTop ? -minOverlapY : minOverlapY; 
		}
	}
	*/

	void PushBackActorCollision(Rect& r1, Rect& r2){
		Vector2D<float>halfSize(r1.mSize / 2);
		Vector2D<float>halfSize2(r2.mSize / 2);

		// 各矩形の中心点を計算 
		Vector2D<float> center1 = r1.mLeftTop + halfSize;
		Vector2D<float> center2 = r2.mLeftTop + halfSize2;

		// 重なりの計算 
		float overlapLeft = (center2.x + halfSize2.x) - (center1.x - halfSize.x);
		float overlapRight = (center1.x + halfSize.x) - (center2.x - halfSize2.x);
		float overlapTop = (center2.y + halfSize2.y) - (center1.y - halfSize.y);
		float overlapBottom = (center1.y + halfSize.y) - (center2.y - halfSize2.y);

		// どちらの方向から衝突しているかを判定
		bool fromLeft = overlapLeft < overlapRight;
		bool fromTop = overlapTop < overlapBottom;

		// 最小の重なり量を算出 
		float minOverlapX = fromLeft ? overlapLeft : overlapRight;
		float minOverlapY = fromTop ? overlapTop : overlapBottom;

		// 水平方向と垂直方向でどちらの重なりが少ないかを判定し、押し戻す 
		if (minOverlapX < minOverlapY) {
			float adjustment = minOverlapX / 2;
			r1.mLeftTop.x += fromLeft ? -adjustment : adjustment;
			r2.mLeftTop.x -= fromLeft ? -adjustment : adjustment;
		}
		else {
			float adjustment = minOverlapY / 2;
			r1.mLeftTop.y += fromTop ? -adjustment : adjustment;
			r2.mLeftTop.y -= fromTop ? -adjustment : adjustment;
		}
	}

	/*
	void PushBackActorCollision(Rect& newRect, const Collision::Rect& otherRects, const Rect& oldRect)
	{
		auto vec = newPosition - actor1->GetOldWorldPosition2D();
		auto collingColl = actor2->GetCollision();
		vec.x -= 
	}
	*/

	bool IsLineIntersectingRect(const Line& line, const Rect& rect) {

		// 端点が矩形の内部にあるかどうかをチェック 
		if (IsPointInsideRect(line.mStart, rect) || IsPointInsideRect(line.mEnd, rect)) {
			return true;
		}

		// 矩形の4辺を定義
		Vector2D<float> topRight(rect.mLeftTop.x + rect.mSize.x, rect.mLeftTop.y);
		Vector2D<float> bottomLeft(rect.mLeftTop.x, rect.mLeftTop.y + rect.mSize.y);
		Vector2D<float> bottomRight = rect.RightBottom();
		Line edges[4] = {
			{rect.mLeftTop, topRight},
			{rect.mLeftTop, bottomLeft},
			{topRight, bottomRight},
			{bottomLeft, bottomRight}
		};

		// 各辺と線分の交差をチェック 
		for (const auto& edge : edges) {
			if (DoLineSegmentsIntersect(line.mStart, line.mEnd, edge.mStart, edge.mEnd)) {
				return true;
			}
		}

		return false;
	}

<<<<<<< HEAD
	bool IsColliding(Actor* actor, Actor* actor2)
	{
		return false;
	}

=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
	bool IsColliding(const Circle& c1, const Circle& c2) {
		float dx = c1.mCenter.x - c2.mCenter.x; 
		float dy = c1.mCenter.y - c2.mCenter.y;
		float distance = sqrt(dx * dx + dy * dy);

		return distance <= (c1.mRadius + c2.mRadius);
	}

	bool IsColliding(const Circle& c1, const Rect& r1)
	{
		// 四角の最も近い点を求める
		float closestX = max(r1.mLeftTop.x, min(c1.mCenter.x, r1.RightBottom().x));
		float closestY = max(r1.mLeftTop.y, min(c1.mCenter.y, r1.RightBottom().y));
		
		// 円の中心と四角の最も近い点との距離を計算
		float distanceX = c1.mCenter.x - closestX;
		float distanceY = c1.mCenter.y - closestY;
		
		// ピタゴラスの定理を使用して距離を計算
		float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
		
		// 距離が円の半径の平方以下であれば衝突している 
		return distanceSquared < (c1.mRadius * c1.mRadius);
	}

	bool IsColliding(RotatedRect& rotatedRect, const Rect& rect) {
		auto rotatedVertices = rotatedRect.GetVertices();
		auto rectVertices = rect.GetVertices();
		std::vector<Vector2D<float>> axes = {
			(rotatedVertices[1] - rotatedVertices[0]).Perpendicular(),
			(rotatedVertices[2] - rotatedVertices[1]).Perpendicular(),
			(rectVertices[1] - rectVertices[0]).Perpendicular(),
			(rectVertices[2] - rectVertices[1]).Perpendicular()
		};
		
		for (const auto& axis : axes) {
			auto rotatedProj = ProjectOntoAxis(rotatedVertices, axis);
			auto rectProj = ProjectOntoAxis(rectVertices, axis);
			
			if (!IsOverlapping(rotatedProj, rectProj)) {
				return false;
			}
		}
		
		return true;
	}
	
	std::pair<float, float> Project(const Rect& rect, const Vector2D<float>& axis) {
		auto vertices = rect.GetVertices();

		float min = axis.Dot(vertices[0]);
		float max = min;

		for (int i = 1; i < 4; ++i) {
			float projection = axis.Dot(vertices[i]);
			
			if (projection < min) {
				min = projection;
			} else if (projection > max) {
				max = projection;
			}
		} 
		return {min, max };
	}

	Collision::Line GetOrientedRectangleEdge(Collision::RotatedRect orientedRectangle, int point)
	{
		Collision::Line edge;
		auto vecA = Vector2D<float>(orientedRectangle.halfExtend);
		auto vecB = Vector2D<float>(orientedRectangle.halfExtend);

		// 右回りで取得する
		// 0,2
		// 1,3は対辺を表す
		switch (point % 4) {
		case 0:
			vecA.x = -vecA.x;
			break;
		case 1:
			vecB.y = -vecB.y;
			break;
		case 2:
			vecA.y = -vecA.y;
			vecB = vecB.NegateVector();
				break;
		case 3:
			vecA = vecA.NegateVector();
			vecB.x = -vecB.x;
			break;
		}

		float degree = Vector2D<float>::GetDegreesFromRadians(orientedRectangle.angle);

		vecA = vecA.RotateVector(degree);
		vecA = vecA + orientedRectangle.center;

		vecB = vecB.RotateVector(degree);
		vecB = vecB + orientedRectangle.center;

		edge.mStart = vecA;
		edge.mEnd = vecB;
		return edge;
	}

	std::pair<float, float> ProjectOntoAxis(const std::vector<Vector2D<float>>& vertices, const Vector2D<float>& axis)
	{
		float minProj = vertices[0].Dot(axis);
		float maxProj = minProj;
		
		for (size_t i = 1; i < vertices.size(); ++i) {
			float proj = vertices[i].Dot(axis);
			if (proj < minProj) minProj = proj;
			if (proj > maxProj) maxProj = proj;
		}
		
		return { minProj, maxProj };
	}

	// 投影が重なっているか確認する関数
	bool IsOverlapping(const std::pair<float, float>& proj1, const std::pair<float, float>& proj2) {
		return !(proj1.second < proj2.first || proj2.second < proj1.first);
	}

	std::vector<Line> RotatedRect::GetRotatedRectLines()
	{
		std::vector<Line>lines;
		lines.resize(4);

		float width = halfExtend.x;
		float height = halfExtend.y;

		float cosAngle = std::cos(angle);
		float sinAngle = std::sin(angle);

		/*
		vertices[0] = { center.x + width / 2 * cosAngle - height / 2 * sinAngle, center.y + width / 2 * sinAngle + height / 2 * cosAngle };
		vertices[1] = { center.x - width / 2 * cosAngle - height / 2 * sinAngle, center.y - width / 2 * sinAngle + height / 2 * cosAngle };
		vertices[2] = { center.x - width / 2 * cosAngle + height / 2 * sinAngle, center.y - width / 2 * sinAngle - height / 2 * cosAngle };
		vertices[3] = { center.x + width / 2 * cosAngle + height / 2 * sinAngle, center.y + width / 2 * sinAngle - height / 2 * cosAngle };

		*/

		lines[0] = Collision::GetOrientedRectangleEdge(*this,0);
		lines[1] = Collision::GetOrientedRectangleEdge(*this, 1);
		lines[2] = Collision::GetOrientedRectangleEdge(*this, 2);
		lines[3] = Collision::GetOrientedRectangleEdge(*this, 3);

		return lines;
	}

	std::vector<Vector2D<float>> RotatedRect::GetVertices()
	{
		std::vector<Vector2D<float>> vertices(4);
		float cosA = std::cos(angle);
		float sinA = std::sin(angle);
		// Calculate the corners of the rotated rectangle
		vertices[0] = center + Vector2D<float>(-halfExtend.x * cosA - halfExtend.y * sinA, -halfExtend.x * sinA + halfExtend.y * cosA);
		vertices[1] = center + Vector2D<float>( halfExtend.x * cosA - halfExtend.y * sinA, halfExtend.x * sinA + halfExtend.y * cosA);
		vertices[2] = center + Vector2D<float>( halfExtend.x * cosA + halfExtend.y * sinA, halfExtend.x * sinA - halfExtend.y * cosA);
		vertices[3] = center + Vector2D<float>(-halfExtend.x * cosA + halfExtend.y * sinA, -halfExtend.x * sinA - halfExtend.y * cosA);
		return vertices; 
	}

<<<<<<< HEAD
	

=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
	/*
	Vector2D<float> RotatedRect::GetTopMiddle()
	{
		float radian = angle; 
		float cosAngle = std::cos(radian);
		float sinAngle = std::sin(radian);
		
		// 上真ん中の座標計算 
		Vector2D<float> topMiddle = {
			//center.x + height / 2 * sinAngle,
			center.x + width / 2 * sinAngle,
			center.y - height / 2 * cosAngle
		}; 
		
		return topMiddle;
	}

	Vector2D<float> RotatedRect::GetBottomMiddle()
	{
		float radian = angle; 
		float cosAngle = std::cos(radian);
		float sinAngle = std::sin(radian);
		
		// 下真ん中の座標計算 
		Vector2D<float> bottomMiddle = {
			//center.x - height / 2 * sinAngle,
			center.x - width / 2 * sinAngle,
			center.y + height / 2 * cosAngle
		};
		
		return bottomMiddle;
	}
	*/
<<<<<<< HEAD
	bool Line::HandleCollision(Collision::Shape& shape)
	{
		if (auto rect = dynamic_cast<Collision::Rect*>(&shape))
		{
			return IsLineIntersectingRect(*this, *rect);
		}
		return false;
	}
	bool Rect::HandleCollision(Collision::Shape& shape)
	{
		if (auto rect = dynamic_cast<Collision::Rect*>(&shape))
		{
			return IsColliding(*this, *rect);
		}
		else if(auto circle = dynamic_cast<Collision::Circle*>(&shape))
		{
			return IsColliding(*circle,*this);
		}
		else if (auto rotRect = dynamic_cast<Collision::RotatedRect*>(&shape))
		{
			return IsColliding(*rotRect, *this);
		}

		return false;
	}

	bool RotatedRect::HandleCollision(Collision::Shape& shape)
	{
		if (auto rect = dynamic_cast<Collision::Rect*>(&shape))
		{
			return IsColliding(*this, *rect);
		}

		return false;
	}

	bool Circle::HandleCollision(Collision::Shape& shape)
	{
		if (auto rect = dynamic_cast<Collision::Rect*>(&shape))
		{
			return IsColliding(*this, *rect);
		}

		else if (auto circle = dynamic_cast<Collision::Circle*>(&shape))
		{
			return IsColliding(*circle, *this);
		}

		return false;
	}
=======
>>>>>>> 1b517a8c9311f4690511d76cf319c9a675cc9420
}