#pragma once

#include "mapBlockBase.h"
#include "blockSand.h"
#include "blockNutrient.h"
#include "blockWater.h"
#include "blockOxygen.h"
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>

class Player;

class MapManager {
	static constexpr int numOfChipX{ 15 };
	static constexpr int numOfChipY{ 5 };
	static constexpr int mapSize{ numOfChipX * numOfChipY };
public:
	static int IndexOfInPlayerMap;
	std::array<std::unique_ptr<BlockBase>, mapSize> m_mapList;
	void LoadMapCSV(const std::string& filePath);

	static int GetWidth(){ return numOfChipX; }
	static int GetHeight(){ return numOfChipY; }
	static int GetMapSize(){ return mapSize; }

	void RunMapAll(const Player& player);
};

