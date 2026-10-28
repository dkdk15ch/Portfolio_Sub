#include "mapBlockBase.h"
#include "DxLib.h"
#include "Player.h"
#include "mapInfo.h"

float BlockBase::blockSize{80.0f};
float LineThick = 1.0f;

BlockBase::BlockBase(BlockType no, float x, float y, const Resource& res, int imgHandle) :
	m_No{static_cast<int>(no)}
	, m_resourceOfMapBlock{res}
	, m_left{x + offsetX}, m_top{y + offsetY}, m_right{m_left+blockSize}, m_bottom{m_top+blockSize}
	, m_collision{m_left,m_top,m_right,m_bottom}
	, m_imgHandle{imgHandle}
{
	
}

//	プレイヤーと当たったら枠の色boxColorを変えて強調
bool BlockBase::DetectPlayer(const Player& player) {
	// プレイヤーが選択中でないときは当たり判定を行わない
	
	// 根の先端を選択するタイミングでは呼び出さないようにする。
	// 根の先端を選択 -> ブロックを選択
	// となるように
	if (player.GetIsSelecting() == false)	return false;
	
	bool result = m_collision.DetectPoint(player.GetCenter());

	if (result){ 
		boxColor = Color::red;
		LineThick = 2.0f;
	}
	else{
		boxColor = Color::black;
		LineThick = 1.0f;
	}

	return result;
}

void BlockBase::Draw() const{

	if(m_imgHandle == -1) {
		DrawFormatString(10, 150, Color::white, "handle== -1");

		return; // No image to draw
	}

	DrawGraphF(m_left,m_top, m_imgHandle,true);
	DrawBoxAA(m_left, m_top, m_left + blockSize, m_top + blockSize, boxColor, false, LineThick);
}

void BlockBase::Run() {
	
	Draw();
}
