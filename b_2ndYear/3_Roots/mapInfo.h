#pragma once
#include "mapList.h"
#include "mapBlockBase.h"
#include "imgHandle.h"

// ‰ğ‘œ“x
constexpr float screenWidth = 1280.0f;
constexpr float screenHeight = 768.0f;

const int mapColumns = MapManager::GetWidth();
const int mapRows = MapManager::GetHeight();
const float offsetX = (screenWidth - (BlockBase::blockSize * mapColumns)) / 2.0f;
const float offsetY = screenHeight - (BlockBase::blockSize * mapRows);

// ‰æ–Ê‚ğ“Ç‚İ‚İ
extern ImageHandles imgHandles;
void RoadTreeImage(const MapManager& map);
void RoadBlockImage();

