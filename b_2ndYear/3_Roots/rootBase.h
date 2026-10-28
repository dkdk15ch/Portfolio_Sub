#pragma once
#include <array>
#include "rootType.h"
#include "resource.h"
#include "point.h"
#include "IRoot.h"
#include "rootUtils.h"
#include "mapBlockBase.h"
#include "collision.h"


class RootBase : public IRoot{
private:
	RootType m_type;
	BoxRects m_start;	
	BoxRects m_end;		
	BoxCollision m_endCollision;	//	終点(根の先端)のみ当たり判定

	std::array<std::weak_ptr<IRoot>, 4> m_nextRootVector{};		
	
	std::weak_ptr<IRoot> m_prevRoot = nullRoot;					//	前の根が無いときはnullRootを使いまわす
	Resource m_gainResource;	//	獲得用リソース
	Resource m_costRecource;	//	コスト用リソース
public:
	//	コンストラクタ・デストラクタ
	RootBase(RootType type,const BoxRects& start, const BoxRects& end, std::shared_ptr<IRoot> prev) :
		m_type(type)
		, m_start(start.left,start.top, start.right, start.bottom)
		, m_end(end.left,end.top,end.right,end.bottom)
		, m_endCollision(end.left, end.top, end.right, end.bottom)
		, m_prevRoot(prev) {
	};


	virtual ~RootBase() {};

	//	純粋仮想
	virtual void Draw() const = 0;
	virtual void Run() = 0;
	
	//static inline std::unique_ptr<RootBase> Create(Point start, Point end, RootBase* prev) {
	//	return nullptr;
	//}

	//	メンバ変数のgetter,setter
	#pragma region SetGetFunctions
	// m_type
	RootType GetType()const override{ return m_type; }
	// m_pointStart, m_pointEnd 
	BoxRects GetStart() const override{ return m_start; }
	BoxRects GetEnd() const override{ return m_end; }
	//　m_nextRootVector
	void SetNextRoot(std::shared_ptr<IRoot> nextRoot) override{
		for (auto& rootptr : m_nextRootVector) {
			rootptr = nextRoot;
		}
	}                                                                            
	std::array<std::weak_ptr<IRoot>,4> GetNextRoot() const override{ return m_nextRootVector; }
	//	m_prevRoot
	void SetPrevRoot(std::shared_ptr<IRoot> prevRoot) override{ m_prevRoot = prevRoot; }
	std::weak_ptr<IRoot> GetPrevRoot() const override{ return m_prevRoot; }
	//	m_nextRootVector　の一括変更
	void SetPrevRootForAllNext(std::shared_ptr<IRoot> newPrev) override{
		for (auto& nextRootWeak : m_nextRootVector) {
			auto nextRoot = nextRootWeak.lock();
			if (nextRoot)
				nextRoot->SetPrevRoot(newPrev);
		}
	}
	//	リソース
	void SetGain(Resource gain) override{ m_gainResource = gain; }
	Resource GetGain()const override{ return m_gainResource; }
	void SetCost(Resource cost) override{ m_costRecource = cost; }
	Resource GetCost()const override{return m_costRecource;}


	#pragma endregion
	
	virtual bool DetectPlayer(const Player& player) override;
};
