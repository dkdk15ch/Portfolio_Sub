#pragma once

#include <memory>
#include <vector>
#include "rootType.h"
#pragma region 前方宣言
//	前方宣言
class IRoot;
struct BoxRects;
class Resource;
class Player;
class MapManager;

#pragma endregion

//	Rootを管理するクラス
class RootManager {
private:
	static std::vector<std::shared_ptr<IRoot>> m_allRoot;

public:
	//	リスト内の全RootのRun()実行
	void RunAllRoots(Player& player);

	//	根の作成
	//	始点と終点を選択（正しい視点や終点じゃないとreturn)
	void CreateRoot(const MapManager& mapmgr ,RootType type, BoxRects start, BoxRects end, std::shared_ptr<IRoot> prev);

	//	根の完全破壊
	void EraseTarget(std::shared_ptr<IRoot> target);

	//	不明マスの公開
	//	始点から終点まで順次公開
	//	新規作成のみ走らせるか、それ以外を早期returnさせる。
	//void PublishBlock();

	//	根の一部破壊
	void DestroyPartial(const MapManager& mapmgr,const BoxRects& start, const BoxRects& end, std::shared_ptr<IRoot> destoryRoot);

	//	根の修復
	void Repair(const MapManager& mapmgr ,std::shared_ptr<IRoot> old1, std::shared_ptr<IRoot> old2);

};
