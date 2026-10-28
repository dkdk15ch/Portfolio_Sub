#pragma once

struct BoxRects;
class MapManager;


enum class Direction { Error, Right, Left, Up, Down };

class rootUtils
{
public:
	static Direction GetDirection(const BoxRects& start, const BoxRects& end);
	static int GetMapIndexFromBoxRects(const BoxRects& box);
	static void SetHasPassedOnRoutes(const MapManager& mapmgr, const BoxRects& start, const BoxRects& end);
};

