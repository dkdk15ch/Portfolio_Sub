#include "Player.h"
#include "DxLib.h"
#include "InputManager.h"
#include "color.h"

namespace {

	unsigned int colorPointer = Color::white;

	void ChengeColor() {
		if (MouseInput::IsLeftPressed()) { colorPointer = Color::red; }
		if (MouseInput::IsLeftReleased()) { colorPointer = Color::white; }
	}

}

void Player::Draw() const {
	float x = GetCenter().getX();
	float y = GetCenter().getY();
	ChengeColor();
	DrawCircleAA(x, y, 4, 12, colorPointer, true);

}

void Player::Run(){
	UpdatePlayerPos();
	Draw();

}

//	private
void Player::UpdatePlayerPos() {
	int mx, my;
	GetMousePoint(&mx, &my);
	//	Ç±Ç±Ç≈m_centerÇïœçXÇ∑ÇÈ
	SetCenter(static_cast<int>(mx), static_cast<int>(my));

}