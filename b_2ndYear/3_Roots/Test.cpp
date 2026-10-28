#include "Test.h"
#include "DxLib.h"
#include "color.h"
#include "box.h"
#include "IRoot.h"
#include "Player.h"
#include "mapList.h"
#include <memory>
#include "InputManager.h"

float Test::msgPosX{ 10.0f };
float Test::msgPosY{ 150.0f };

namespace {
	bool prevMouseLeft = false;
}

void Test::Test_Print(std::string message, float posY) {
	DrawFormatStringF(msgPosX, msgPosY+ posY, Color::white, "%s",message.c_str());
}

void Test::Test_Point(float x, float y) {
	DrawCircleAA(x, y, 8, 12, Color::red);
}

void Test::Test_Root(MapManager& mapmgr,RootManager& rootmgr,int startIndex, int endIndex) {
	float startLeft		= mapmgr.m_mapList[startIndex]->GetLeft();
	float startTop		= mapmgr.m_mapList[startIndex]->GetTop();
	float startRight	= mapmgr.m_mapList[startIndex]->GetRight();
	float startBottom	= mapmgr.m_mapList[startIndex]->GetBottom();
	BoxRects startblockRects(startLeft, startTop, startRight, startBottom);

	float endLeft = mapmgr.m_mapList[endIndex]->GetLeft();
	float endTop = mapmgr.m_mapList[endIndex]->GetTop();
	float endRight = mapmgr.m_mapList[endIndex]->GetRight();
	float endBottom = mapmgr.m_mapList[endIndex]->GetBottom();
	BoxRects endBlockRects(endLeft, endTop, endRight, endBottom);
	//Test::Test_Point(endLeft, endTop);
	//Test::Test_Point(endRight, endBottom);

	rootmgr.CreateRoot(mapmgr, RootType::Pioneer, startblockRects, endBlockRects, nullRoot);
}

void Test::Test_PlayerCreateRoot(Player& player, MapManager& mapmgr, RootManager& rootmgr) {
	// 選択した瞬間にブロックの判定をとってしまうので、1Fずらす。
	
	if (!player.GetIsSelecting()) return;	// 選択中でなければ何もしない

	// ブロックをまだ選んでないなら何もしない。
	int endIndex = MapManager::IndexOfInPlayerMap;
	if (endIndex == -1) return;

	std::shared_ptr<IRoot> startRoot = player.GetSelectedRoot().lock();
	if (!startRoot)
	{
		Test::Test_Print("startRoot is nullptr", -20);
		return;
	}
	
	// 選んだ根の先端部分から伸ばす
	BoxRects startRects = startRoot->GetEnd();
	Test::Test_Point(startRects.left, startRects.top);
	Test::Test_Point(startRects.right,startRects.bottom);

	// マップと当たり判定をとり、クリックしたところへ伸ばす。
	// クリックしたところの、indexを取得したい。
	float endLeft = mapmgr.m_mapList[endIndex]->GetLeft();
	float endTop = mapmgr.m_mapList[endIndex]->GetTop();
	float endRight = mapmgr.m_mapList[endIndex]->GetRight();
	float endBottom =mapmgr.m_mapList[endIndex]->GetBottom();
	BoxRects endBlockRects(endLeft, endTop, endRight, endBottom);
	Test::Test_Point(endLeft, endTop);
	Test::Test_Point(endRight, endBottom);

	// ルートを作成
	// 最後の引数がうまくいくか…一旦nullでテスト
	// null -> ok
	rootmgr.CreateRoot(mapmgr, RootType::Pioneer, startRects, endBlockRects, std::move(startRoot));
	player.SetIsSelecting(false); // 選択状態を解除

	// 初期化
	MapManager::IndexOfInPlayerMap = -1;
}
