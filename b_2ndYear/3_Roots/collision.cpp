#include "collision.h"
#include "point.h"


#include "test.h"

bool BoxCollision::DetectPoint(const MutaPoint& point) {
	if (point.getX() < m_rects.left || point.getX() > m_rects.right) {
		return false;

	}
	if (point.getY() < m_rects.top	 || point.getY() > m_rects.bottom)
		return false;

	return true;
}

