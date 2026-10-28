#include "rootUtils.h"
#include "box.h"
#include "mapList.h"
#include "mapInfo.h"

Direction rootUtils::GetDirection(const BoxRects& start, const BoxRects& end) {
	float startLeft = start.left;
	float endLeft = end.left;
	float startTop = start.top;
	float endTop = end.top;

	if (startLeft == endLeft && startTop == endTop)
	{
		return Direction::Error;	//	“¯‚¶ˆÊ’u‚Í–³Œø
	}

	if (startLeft == endLeft)
	{
		return (endTop > startTop) ? Direction::Down : Direction::Up;
	}
	if (startTop == endTop)
	{
		return (endLeft > startLeft) ? Direction::Right : Direction::Left;
	}

	return Direction::Error;	//	–³Œø‚È•ûŒü

}

int rootUtils::GetMapIndexFromBoxRects(const BoxRects& box) {
		int x = static_cast<int>((box.left - offsetX) / BlockBase::blockSize);
		int y = static_cast<int>((box.top - offsetY) / BlockBase::blockSize);

		return y * mapColumns + x;
}

void rootUtils::SetHasPassedOnRoutes(const MapManager& mapmgr, const BoxRects& start, const BoxRects& end) {
	int sIndex = rootUtils::GetMapIndexFromBoxRects(start);
	int eIndex = rootUtils::GetMapIndexFromBoxRects(end);
	int startIndex = std::min(sIndex, eIndex);
	int endIndex = std::max(sIndex, eIndex);
	Direction dir = rootUtils::GetDirection(start, end);
	switch (dir)
	{
	case Direction::Right:
		startIndex += 1;
		endIndex += 1;
	case Direction::Left:
		for (int i = startIndex; i < endIndex; i++) {
			mapmgr.m_mapList[i]->ToggleHasPassed();
		}
		return;
	case Direction::Up:
		startIndex += mapColumns;
		endIndex += mapColumns;
	case Direction::Down:
		for (int i = startIndex; i < endIndex; i += mapColumns) {
			mapmgr.m_mapList[i]->ToggleHasPassed();
		}
	default:
		return;
	}

}
