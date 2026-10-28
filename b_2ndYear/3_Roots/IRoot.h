#pragma once

#include "point.h"
#include "rootType.h"
#include "resource.h"
#include "box.h"
#include <memory>
#include <array>

class Player;	// 前方宣言

class IRoot {
public:
	~IRoot() {};
	virtual void Run() = 0;

#pragma region SetGetFunctions
	virtual RootType GetType() const = 0;
	virtual BoxRects GetStart() const = 0;
	virtual BoxRects GetEnd() const = 0;
	virtual void SetNextRoot(std::shared_ptr<IRoot> nextRoot) = 0;
	virtual std::array<std::weak_ptr<IRoot>, 4> GetNextRoot() const = 0; 
	virtual void SetPrevRoot(std::shared_ptr<IRoot> prevRoot) = 0; 
	virtual std::weak_ptr<IRoot> GetPrevRoot() const = 0;
	virtual void SetPrevRootForAllNext(std::shared_ptr<IRoot> newPrev) = 0;
	virtual void SetGain(Resource gain) = 0;
	virtual Resource GetGain() const = 0;
	virtual void SetCost(Resource cost) = 0;
	virtual Resource GetCost() const = 0;
#pragma endregion
	//	プレイヤーとの当たり判定
	virtual bool DetectPlayer(const Player& player) = 0;
	//	描画
	virtual void Draw() const = 0;
};

// PrevRootが無いとき用
class NullRoot : public IRoot {
public:
	void Run() override {}
#pragma region SetGetFunctions
	RootType GetType() const override { return RootType::Null; }
	BoxRects GetStart() const override { return BoxRects(0,0,0,0); }
	BoxRects GetEnd() const override { return BoxRects(0, 0, 0, 0); }
	void SetNextRoot(std::shared_ptr<IRoot> nextRoot) override {}
	std::array<std::weak_ptr<IRoot>, 4> GetNextRoot() const override {return {};}
	void SetPrevRoot(std::shared_ptr<IRoot> prevRoot) override {}
	std::weak_ptr<IRoot> GetPrevRoot() const override { return {}; }
	void SetPrevRootForAllNext(std::shared_ptr<IRoot> newPrev) override {}
	void SetGain(Resource gain) override {}
	Resource GetGain() const override { return Resource(0, 0, 0); }
	void SetCost(Resource cost) override {}
	Resource GetCost() const override { return Resource(0, 0, 0); }
#pragma endregion
	bool DetectPlayer(const Player& player) override {
		return false; 
	}
	void Draw() const override {}

};

extern std::shared_ptr<NullRoot> nullRoot;