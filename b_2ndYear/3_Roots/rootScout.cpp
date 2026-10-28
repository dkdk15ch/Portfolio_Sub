#include "rootScout.h"
#include "calcuResource.h"
#include "DxLib.h"


namespace {
	Resource baseCost = Resource(10,10,10);
	float disBonus = 0.1f;
}

Scout::Scout(BoxRects start, BoxRects end, std::shared_ptr<IRoot> prev) : RootBase(RootType::Scout,start, end, prev){

}

void Scout::Draw()const {
	//DrawBoxAA(drawPosition.left, drawPosition.top, drawPosition.right, drawPosition.bottom, Color::green, false);
}




void Scout::Run() {
	//Draw();
}