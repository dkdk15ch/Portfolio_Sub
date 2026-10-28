#pragma once
#include "DxLib.h"


#define UI_FONT_INDEX_STOP 0

//	フォント用マクロ

//	ストーリー
#define NUM_OF_STORY_FONTS 4	//	storyFontsの要素数
#define FONT_STORY_TEXT 0
#define FONT_STORY_BIG_TEXT 1
#define FONT_STORY_CHOICE1 2
#define FONT_STORY_CHOICE2 3



#define FONT_ATTACK_OF_ENEMY 0

//	共有宣言
extern int UIFonts[1]; //	UI用フォント
extern const char uiStrings[1][165];	//	UI用文字列
extern int storyFonts[NUM_OF_STORY_FONTS];	//	ストーリー用フォント
extern int playGuideFonts[2];	//	プレイガイド用フォント
extern int animationFonts[1];	//	アニメーション用フォント

//	プロトタイプ宣言
void InitalizeUIFonts(void);	//	UIフォント定義
//	テキスト位置計算
void TextPosCalculate(int* textWidth, int* textHeight, int* textLineCount, int fonts, const char uistring[], int centerPosX, int centerPosY, int* textPosX, int* textPosY);
