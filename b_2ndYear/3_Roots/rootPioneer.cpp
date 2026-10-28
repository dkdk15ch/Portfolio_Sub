#include "rootPioneer.h"
#include "rootBase.h"
#include "rootType.h"
#include "calcuResource.h"
#include "color.h"
#include "DxLib.h"
#include "mapList.h"
#include <assert.h>
#include <stdexcept>
#include "mapList.h"

//	Dxlibのマクロとstd::min,maxが競合するので、
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include <algorithm>


#include <string>
#include "Test.h"
namespace {
	// Test用
	std::string costString;
	std::string gainString;

	Resource baseCost = Resource(50, 50, 50);
	constexpr float disBonus{ 0.05f };
	constexpr float gainTest{ 1.0f };
	constexpr float halfWidth{128.0f / 4.0f};
}


Pioneer::Pioneer(const MapManager& mapmgr,BoxRects start, BoxRects end, std::shared_ptr<IRoot> prev) :
	RootBase(RootType::Pioneer ,start, end, prev) 
{
	Resource cost = CalcuCostResource(start, end, baseCost,disBonus);
	SetGain(cost);
	Resource gain = CalcuGainResource(mapmgr, start, end, gainTest);
	SetCost(gain);

	// Test描画用
	costString = "Cost: N =" + std::to_string(cost.GetNutrient()) + " W =" + std::to_string(cost.GetWater()) + "　O =" + std::to_string(cost.GetOxygen());
	gainString = "Gain: N =" + std::to_string(gain.GetNutrient()) + " W =" + std::to_string(gain.GetWater()) + "　O =" + std::to_string(gain.GetOxygen());

	rootUtils::SetHasPassedOnRoutes(mapmgr, start, end);
}

void Pioneer::Draw()const {
	BoxRects start = this->GetStart();
	BoxRects end = this->GetEnd();
	//	描画範囲を決定
	float left = std::min(start.left, end.left);
	float right = std::max(start.right, end.right);
	float top = std::min(start.top, end.top);
	float bottom = std::max(start.bottom, end.bottom);

	float offset = 15.0f;
	left	+= offset;
	right	-= offset;
	top		+= offset;
	bottom	-= offset;

	DrawBoxAA(left,top,right,bottom, Color::brown, false, 10.0f);
}

void Pioneer::Run() {
	Draw();
	Test::Test_Print(costString, +20.0f);
	Test::Test_Print(gainString, +40.0f);
}