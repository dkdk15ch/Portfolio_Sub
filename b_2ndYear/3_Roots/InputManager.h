#pragma once

class MouseInput {
public:
	static void Update();	
	static void SetPrevInput();
	//	左・右クリック
	static bool IsLeftPressed();	//	（左）押している時
	static bool IsLeftTriggerd();	//	（左）押された瞬間
	static bool IsLeftReleased();	//	（左）離した瞬間
	static bool IsRightPressed();	//	（右）押している時
private:
	static int m_NowInput;
	static int m_PrevInput;
};


