#pragma once
#include <string>
#include "rootManager.h"
#include "mapList.h"


class Test
{
public:
	static float msgPosX;	//	メッセージ表示位置
	static float msgPosY;	//	メッセージ表示位置
	static void Test_Print(std::string message, float posY=0);
	static void Test_Point(float x, float y);
	static void Test_Root(MapManager& mapmgr, RootManager& rootmgr, int startIndex, int endIndex);
	static void Test_PlayerCreateRoot(Player& player, MapManager& mapmgr, RootManager& rootmgr);
};

