#include "DxLib.h"

//	入力用マクロ
#define INPUT_MOUSE_LEFT ((GetMouseInput() & MOUSE_INPUT_LEFT)==MOUSE_INPUT_LEFT)

//	ドラッグアニメーション用マクロ
#define ANI_TYPE_NEXT 0	//	NEXT
#define ANI_TYPE_CHANGE 1	//	PageChange

struct ButtonRect
{
	float left;
	float top;
	float right;
	float bottom;
};

//	カーソル位置の取得
extern int cursorX;	//	カーソルのX座標
extern int cursorY;	//	カーソルのY座標

//	状態管理
extern bool inputPrevMouseLeftState;	//	前フレームで左クリックが押されて
extern bool isPause;	//	ポーズかどうか（プレイガイド開いているとき）

//	プロトタイプ宣言
bool DetectButtonCollision(int left, int top, int right, int bottom, unsigned int* text, unsigned int textColor);
bool DetectDrug(int left, int top, int right, int bottom, unsigned int* text, unsigned int textColor, int animationType);
void ResetDrugParm(void);