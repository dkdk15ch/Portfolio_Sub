#include <cstdlib>
#include <algorithm>
#include "calcuResource.h"
#include "box.h"
#include "resource.h"
#include "mapList.h"
#include "mapBlockBase.h"
#include "mapInfo.h"
#include "rootUtils.h"

//	コストを計算
Resource CalcuCostResource(BoxRects start, BoxRects end, Resource baseCost, float disBonus) {

	int blockSize = BlockBase::blockSize;

	//	blockSizeで正規化
	auto distance = std::abs((start.left - end.left)/blockSize);
	//	垂直方向の場合、y方向の距離
	if (distance == 0)
		distance = std::abs((start.top - end.top)/blockSize);

	auto n = baseCost.GetNutrient();
	auto w = baseCost.GetWater();
	auto o = baseCost.GetOxygen();
	//	

	float ratio = std::min(distance * distance * disBonus, 0.3f);
	//ratio = std::max(ratio, 0.3f);

	n -= static_cast<int>(n * ratio);
	w -= static_cast<int>(w * ratio);
	o -= static_cast<int>(o * ratio);
	n *= distance;
	w *= distance;
	o *= distance;

	return Resource(n, w, o);
}

Resource CalcuGainResource(const MapManager& mapmgr ,
	const BoxRects& start, const BoxRects& end, float gainEfficiency) {
	//	走査順を統。
	//	左から右　or　上から下
	int sIndex = rootUtils::GetMapIndexFromBoxRects(start);
	int eIndex = rootUtils::GetMapIndexFromBoxRects(end);
	int startIndex = std::min(sIndex, eIndex);
	int endIndex = std::max(sIndex, eIndex);
	//	各マスにアクセスして、通過マスの獲得リソースの総和を求める。
	//	ただし、伸ばし始める地点のマスは加算しない。
	//	右・下にいくときは、startIndexを無視。
	//	左・上にいくときは、endIndexを無視。
	Direction dir = rootUtils::GetDirection(start, end);
	Resource total = Resource();
	switch (dir) {
	case Direction::Right:
		startIndex += 1;
		endIndex += 1;
	case Direction::Left:
		for (int i = startIndex; i < endIndex; i++) {
			Resource r = mapmgr.m_mapList[i]->GetResouce();
			total = Resource::AddResouce(total,r);
		}
		return total;
	case Direction::Down:
		startIndex += mapColumns;
		endIndex += mapColumns;
	case Direction::Up:
		for (int i = startIndex; i < endIndex; i += mapColumns) {
			Resource r = mapmgr.m_mapList[i]->GetResouce();
			total = Resource::AddResouce(total, r);
		}
		return total;

	}

	return Resource{0,0,0};	
}

