#include "mapList.h"
#include "blockFactory.h"
#include "algorithm"
#include "mapBlockBase.h"
#include "InputManager.h"
#include "Tree.h"

int MapManager::IndexOfInPlayerMap = -1; // 初期化

void MapManager::LoadMapCSV(const std::string& filePath) {
    std::ifstream file(filePath);
	float chipSize = BlockBase::blockSize;

    std::string line;
    int y = 0;
    while (std::getline(file, line) && y < numOfChipY) {
        std::stringstream ss(line);
        std::string cell;
        int x = 0;
        while (std::getline(ss, cell, ',') && x < numOfChipX) {
            if (cell.empty() || cell.find_first_not_of(" \t\n\r") == std::string::npos) {
                m_mapList[y * numOfChipX + x] = nullptr;
            }
            else if (std::all_of(cell.begin(), cell.end(), ::isdigit)) {
                BlockType blockType = static_cast<BlockType>(std::stoi(cell));
                m_mapList[y * numOfChipX + x] = BlockFactory::CreateBlock(blockType, x*chipSize, y*chipSize);
            }
            else {
                m_mapList[y * numOfChipX + x] = nullptr;
            }
            ++x;
        }
        ++y;
    }
}

void MapManager::RunMapAll(const Player& player) {

	for (int i = 0; i < GetMapSize(); i++) {
		//	DetectPlayerで枠の色を変更するので、
//	DetectPlayerをDrawの前に呼び出す必要がある
        if (m_mapList[i]->DetectPlayer(player) && MouseInput::IsLeftTriggerd()) {
			// indexを保存
			MapManager::IndexOfInPlayerMap = i;
        }
        m_mapList[i]->Run();
	}
    TreeInst.Run();
}