#pragma once

struct BoxRects
{
	const float left, top, right, bottom;
	BoxRects(float left, float top, float right, float bottom) :
		left{ left }, top{ top }, right{ right }, bottom{ bottom } {}

};