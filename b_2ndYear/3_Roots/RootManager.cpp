#include "rootManager.h"
#include "point.h"
#include "rootBase.h"
#include "resource.h"
#include "rootFactory.h"
#include "Player.h"
#include "DxLib.h"
#include "string"
#include "box.h"
#include "InputManager.h"


#include "Test.h"

std::vector<std::shared_ptr<IRoot>> RootManager::m_allRoot;

namespace {
	//	仮のgain
	Resource gainTest = { 30,30,30 };
	Resource repairTest = { 30,30,0 };

	void SetNextForPrev(std::shared_ptr<IRoot> prev, std::shared_ptr<IRoot> next) {
		std::shared_ptr<IRoot> prevRootPtr = prev->GetPrevRoot().lock();
		prevRootPtr->SetNextRoot(next);
	}
}

// public
void RootManager::RunAllRoots(Player& player) {


	for (auto& root : m_allRoot) {
		root->Run();
		// プレイヤーが根の当たり判定と重なって、クリックしたら
		// プレイヤーの選択状態を変更する
		if (!root->DetectPlayer(player)) continue;
		if (MouseInput::IsLeftTriggerd()) {
			player.SetIsSelecting(true);

			// unique_ptrなので、コピーして保持はできない。
			// shared_ptrにしてもいいが...
			player.SetSelectedRoot(root);
		}
			
		

	}

}

//　根の完全破壊	
void RootManager::EraseTarget(std::shared_ptr<IRoot> target) {
	//	①targetをm_allRootから探す
	//	②m_allRootのtargetを削除
	auto begin = m_allRoot.begin();
	auto end = m_allRoot.end();
	auto ite = std::remove_if(
		begin, end,
		[target](const std::shared_ptr<IRoot>& ptr) {
			return ptr == target;
		}
	);
	m_allRoot.erase(ite, end);
}

//	根の生成
void RootManager::CreateRoot(const MapManager& mapmgr,RootType type, BoxRects start, BoxRects end, std::shared_ptr<IRoot> prev) {

	std::shared_ptr<IRoot> newRoot;
	newRoot = RootFactory::Create(mapmgr,type, start, end, prev);
	if (newRoot == nullptr)	return;

	m_allRoot.push_back(std::move(newRoot));
}

//	根の一部破壊
void RootManager::DestroyPartial(const MapManager& mapmgr,const BoxRects& start, const BoxRects& end, std::shared_ptr<IRoot> destroyRoot) {
	//	破壊される部分（引数）を空けるように根を二つ追加する。
	//	new1をoldの m_pointStart から start まで作成
	BoxRects startN1 = destroyRoot->GetStart();
	BoxRects endN1 = start;
	RootType type = destroyRoot->GetType();
	std::shared_ptr<IRoot> prevN1 = destroyRoot->GetPrevRoot().lock();
	std::shared_ptr<IRoot> newRoot1;
	newRoot1 = RootFactory::Create(mapmgr,type, startN1, endN1,prevN1);

	//	new2をendからoldの m_pointEnd まで作成
	BoxRects startN2 = end;
	BoxRects endN2 = destroyRoot->GetEnd();
	std::shared_ptr<IRoot> newRoot2;
	newRoot2 = RootFactory::Create(mapmgr,type, startN2, endN2, nullptr);

	//	oldのprevRootの m_nextRootList にnew1を追加
	SetNextForPrev(destroyRoot, newRoot1);

	//	oldの m_nextRootList の全要素の m_prevRoot をnew2に変更
	destroyRoot->SetPrevRootForAllNext(newRoot2);

	//	oldの削除 m_allRootから削除
	EraseTarget(destroyRoot);

	//	m_allRoot に new1,2を移動
	m_allRoot.push_back(std::move(newRoot1));
	m_allRoot.push_back(std::move(newRoot2));
}

//	根の修復
void RootManager::Repair(const MapManager& mapmgr,std::shared_ptr<IRoot> old1, std::shared_ptr<IRoot> old2) {
	//	newRoot を作ってから、既存を削除
	BoxRects start = old1->GetStart();
	BoxRects end = old2->GetEnd();
	RootType type = old1->GetType();
	std::shared_ptr<IRoot> prev = old1->GetPrevRoot().lock();


	//	gainとcostがいる!!
	//	startとendからgainを計算する
	
	std::shared_ptr<IRoot> newRoot;
	//newRoot = RootBase::Create(start, end, newPrev);
	newRoot = RootFactory::Create(mapmgr, type, start, end, prev);
	SetNextForPrev(old1, newRoot);

	//	既存削除
	EraseTarget(old1);
	EraseTarget(old2);
}
