#pragma once
//	前方宣言
struct BoxRects;
class Resource;
class MapManager;

//	コストを計算
Resource CalcuCostResource(BoxRects start, BoxRects end, Resource baseCost, float disBonus);

//	獲得リソースを計算
Resource CalcuGainResource(const MapManager& mapmgr, const BoxRects& start, const BoxRects& end, float gainEffic);
