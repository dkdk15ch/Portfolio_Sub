#pragma once
#include "DxLib.h"

#define TIMING_TIME 1.0f
#define PAGE_MAX 2000
#define POSY_FIRST_LINE 70
#define MP_COST_OF_CHANGE 30
#define NUM_OPEN_SPELL 10

struct PageRect
{
	float left;
	float top;
	float right;
	float bottom;
};

struct ParagraphPosition
{
	//	呪文の各要素の位置
	float namePosX;
	float namePosY;
	float spelPosX;
	float spelPosY;
	float DMGPosX;
	float DMGPosY;
	//	段落数を管理用
	int Paragraph;	

};

//	変数宣言
extern PageRect pageRight ;
extern PageRect pageLeft ;
extern PageRect pageChange;
extern int currentPage;
extern int countPageChange;

//	目押し用
extern float timingBarPos;
extern float timingLength;
extern float timingTime;
extern bool barDir;
extern bool isMoving;

//	ページ更新用
extern int indexOfPage;	//	ページ決定用配列の添え字
extern ParagraphPosition firstParagraph;	//	1つ目の魔法名位置
extern ParagraphPosition secondParagraph;	//	2つ目の魔法名位置
extern ParagraphPosition thirdParagraph;	//	3つ目の魔法名位置
extern ParagraphPosition fourthParagraph;	//	4つ目の魔法名位置
extern ParagraphPosition fifthParagraph;	//	5つ目の魔法名位置
extern int drawPageAlpha;	//	α値

//	新規ページか判別用
extern bool isNewPage;

//	新規カウント用
extern int newWords;
extern int newPages;

//	プロトタイプ宣言
//void DebugLog(void);
void DrawBook(bool LastPage = false);	//	本（枠）の描画
void DetectChangeButton(void);	//　ページチェンジの処理
void MoveTimingBar(void);	//	捲り時のタイミングバーの移動
void StopTimingBar(void);	//	捲り時のタイミングバーの停止
void UpdatePage(void);		//	ページ内容の更新
void ChackMemory(ParagraphPosition paragraph, int index);		//	覚えている詠唱文を確認
void DrawResult(void);		//	リザルト描画
void ResetAlreadyTurnedPages(void);//	既出ページかどうかの初期化


