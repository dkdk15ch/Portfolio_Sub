#include "InputManager.h"
#include "dxlib.h"

int MouseInput::m_NowInput{ 0 };
int MouseInput::m_PrevInput{ 0 };

//	現在と前フレームの入力更新
void MouseInput::Update() {	
	m_NowInput = GetMouseInput();
}

void MouseInput::SetPrevInput() {
	// 最後に呼ばれるようにする。
	m_PrevInput = m_NowInput;
}

//	左クリックが押されてる時
bool MouseInput::IsLeftPressed(){
	return (m_NowInput & MOUSE_INPUT_LEFT) != 0;
}

//	左クリックが押された瞬間
bool MouseInput::IsLeftTriggerd(){
	return (m_NowInput & MOUSE_INPUT_LEFT) && !(m_PrevInput & MOUSE_INPUT_LEFT);
}

//	左クリックが離された瞬間
bool MouseInput::IsLeftReleased() {
	return !(m_NowInput & MOUSE_INPUT_LEFT) && (m_PrevInput & MOUSE_INPUT_LEFT);
}

//	右クリックが押されているとき
bool MouseInput::IsRightPressed(){
	return (m_NowInput & MOUSE_INPUT_RIGHT) != 0;
}

