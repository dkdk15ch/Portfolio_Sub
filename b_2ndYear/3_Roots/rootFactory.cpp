#include "rootFactory.h"
#include <assert.h>
#include "rootBase.h"
#include "IRoot.h"
#include "rootUtils.h"
#include "mapList.h"
#include "mapInfo.h"
//	サブクラス
#include "rootScout.h"
#include "rootPioneer.h"

#include "DxLib.h"

namespace {
	//	経路上に既存ルートがないか確認
	//	あったら、その位置のindexを返す。
	int CheckHasPassed(const MapManager& mapmgr, const BoxRects& start, const BoxRects& end) {
		// startからendまで順番通りにに確認していき、HasPassed で止める。
		int startIndex = rootUtils::GetMapIndexFromBoxRects(start);
		int endIndex = rootUtils::GetMapIndexFromBoxRects(end);
		Direction dir = rootUtils::GetDirection(start, end);
		switch (dir)
		{
		case Direction::Right:
			for (int i = startIndex + 1; i <= endIndex; i++) {
				if (mapmgr.m_mapList[i]->GetHasPassed()) return i-1;
			}
			break;
		case Direction::Left:
			for (int i = startIndex - 1; i >= endIndex; i--) {
				if (mapmgr.m_mapList[i]->GetHasPassed()) return i+1;
			}
			break;
		case Direction::Down:
			for (int i = startIndex + mapColumns; i <= endIndex; i += mapColumns) {
				if (mapmgr.m_mapList[i]->GetHasPassed()) return i - mapColumns;
			}
			break;
		case Direction::Up:
			for (int i = startIndex - mapColumns; i >= endIndex; i -= mapColumns) {
				if (mapmgr.m_mapList[i]->GetHasPassed()) return i + mapColumns;
			}
			break;
		default:
			return -1;
		}
	}
}


std::shared_ptr<IRoot> RootFactory::Create(const MapManager& mapmgr, RootType type, BoxRects start, BoxRects end, std::shared_ptr<IRoot> prev) {

	//	直線以外は生成不可
	if (!(start.left == end.left || start.top == end.top)) {
		return nullptr;
	}

	//	経路上に既にあったらその地点を終点に変更する
	int endIndex = CheckHasPassed(mapmgr, start, end);
	float left = end.left;
	float top = end.top;
	float right = end.right;
	float bottom = end.bottom;
	if (endIndex != -1) {
		left = mapmgr.m_mapList[endIndex]->GetLeft();
		top = mapmgr.m_mapList[endIndex]->GetTop();
		right = mapmgr.m_mapList[endIndex]->GetRight();
		bottom = mapmgr.m_mapList[endIndex]->GetBottom();
	}
	BoxRects endRects = BoxRects(left, top, right, bottom);

	switch (type)
	{
	case RootType::Null:
		return nullRoot;
	case RootType::Scout:
		return std::make_shared<Scout>(start, endRects, prev);
	case RootType::Pioneer:
		return std::make_shared<Pioneer>(mapmgr,start, endRects, prev);
	default:
		assert(false && "RootFactory : 未登録のRootType");
	}
}