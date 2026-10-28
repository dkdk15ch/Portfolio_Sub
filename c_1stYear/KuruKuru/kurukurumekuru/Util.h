#pragma once
#include "Dxlib.h"

#define SCREEN_WIDTH 640.0f	//	画面の幅
#define SCREEN_HEIGHT 480.0f	//	画面の高さ
#define SCREEN_HORI_CENTER 320.0f	//	画面の横方向の中心
#define SCREEN_VERT_CENTER 240.0f	//	画面の縦方向の中心
#define SCREEN_LOGO_POSY 120.0f		//	ロゴ縦方向の中心

//	カラーマクロ
#define COLOR_WHITE (GetColor(255,255,255))
#define COLOR_RED (GetColor(255,0,0))
#define COLOR_GREEN (GetColor(0,255,0))
#define COLOR_BLUE (GetColor(0,0,255))
#define COLOR_YELLOW (GetColor(255,255,0))
#define COLOR_GRAY (GetColor(125,125,125))
#define COLOR_BLACK (GetColor(0,0,0))



//	2次元座標用
struct Point
{
	float posX;
	float posY;
};

//	矩形定義用
struct Rect
{
	float rectleft;
	float rectright;
	float recttop;
	float rectbottom;
};

//	計算用変数
extern int textWidth;
extern int textHeight;
extern int textLineCount;

static void TextPosCalculate(int* textWidth, int* textHeight, int* textLineCount, int fonts, const char uistring[], float centerPosX, float centerPosY, float* textPosX, float* textPosY)
{
	GetDrawFormatStringSizeToHandle(&*textWidth, &*textHeight, &*textLineCount, fonts, uistring);
	*textPosX = centerPosX - (*textWidth / 2.0f);
	*textPosY = centerPosY - (*textHeight / 2.0f);
}


//	ボタン位置計算用関数
static void ButtonPosCalculate(int* textWidth, int* textHeight, int* textLineCount, int fonts, const char uistring[], float centerPosX, float centerPosY, float* textPosX, float* textPosY, float* frameLeft, float* frameTop, float* frameRight, float* frameBottom)
{
	TextPosCalculate(&*textWidth, &*textHeight, &*textLineCount, fonts, uistring, centerPosX, centerPosY, &*textPosX, &*textPosY);

	*textWidth = *textWidth * 1.5f / 2.0f;
	*textHeight = *textHeight * 1.5f / 2.0f;
	*frameLeft = centerPosX - *textWidth;
	*frameTop = centerPosY - *textHeight;
	*frameRight = centerPosX + *textWidth;
	*frameBottom = centerPosY + *textHeight;
}
