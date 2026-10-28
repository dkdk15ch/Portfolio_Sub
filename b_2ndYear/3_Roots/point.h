#pragma once

struct Point {
	Point(float x, float y) : posx{ x }, posy{ y } {};
	const float posy;
	const float posx;
};

class MutaPoint {
private:
	float posy;
	float posx;
public:
	MutaPoint(float x, float y) : posx{ x }, posy{ y } {};
	void set(float x, float y) { posx = x; posy = y; }
	void setX(float x) { posx = x; }
	void setY(float y) { posy = y; }
	float getX() const { return posx; }
	float getY() const { return posy; }
};