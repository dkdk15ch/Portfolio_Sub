#include "rootBase.h"
#include "Player.h"

bool RootBase::DetectPlayer(const Player& player) {
	//	先端が選べない状態じゃないかもチェックする
	if (m_endCollision.DetectPoint(player.GetCenter()) == true) return true ;
	
	return false;
}