#pragma once
#include "DxLib.h"
#include "Util.h"

//	フォント配列用マクロ
#define FONT_INDEX_TITLE_LOGO 0
#define FONT_INDEX_TITLE_BUTTON 1
//	点滅用
#define UI_DURATION_OF_BLINK 1.5f
#define UI_DURATION_OF_HIDE 0.5f

extern int Titlefonts[2];
extern int TitleGraph;

//	タイトルロゴ
extern Point uiTitleLogoTextPosition;
extern Point uiTitleButtonTextPosition;
extern Rect uiTitleButtonFrameRect;
extern Rect TitleGraphRect;

void LoadGraphTitle();
void CreateTitleFonts();
bool DrawTitle(float deltaTime);