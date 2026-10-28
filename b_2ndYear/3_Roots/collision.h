#pragma once
#include "box.h"

class MutaPoint;

class BoxCollision {
private:
	BoxRects m_rects;	//	当たり判定を持つ矩形頂点
public:
	BoxCollision(float left, float top, float right, float bottm) :
		m_rects(left, top, right, bottm) {}

	//	点を検知
	bool DetectPoint(const MutaPoint& point);

};