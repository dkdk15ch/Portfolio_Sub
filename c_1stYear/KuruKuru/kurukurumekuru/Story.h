#pragma once
#include <DxLib.h>

//	ストーリー描画位置用マクロ
#define STORY_START_LEFT_POSX 15	//	左ページの開始位置
#define STORY_START_RIGHT_POSX (SCREEN_HORI_CENTER -45)	//	右ページの開始位置
#define STORY_START_POSY 30			//	縦方向の開始位置
#define STORY_LEFT_PAGE_END 250		//	左ページの描画限界位置
#define STORY_RIGHT_PAGE_END 510	//	右ページの描画限界位置

//	ストーリーのページ数
#define STORY_PAGE_MAX 6	
#define STORY_ROW 4


//	ChoiceEvent用
#define EVENT_NUM 4	//	イベントの総数
#define EVENT_ARMORER_SHOP 0	//	防具屋
#define EVENT_WIZARD 1			//	魔法使い
#define EVENT_PRIST 2			//	僧侶
#define EVENT_MEDICINE_CELLER 3	//	薬売り

struct StoryLog
{
	int eventLog[STORY_PAGE_MAX] = { 0 };
	bool isEventCleared[STORY_PAGE_MAX] = { false };
};

struct RectOfPicture
{
	int left;
	int top;
	int right;
	int bottom;
};


//	共通宣言
extern char storyTexts[6][STORY_ROW][49];
extern char eventTexts[4][2][41];
extern StoryLog storyLog;
extern RectOfPicture pictureType1;
extern RectOfPicture pictureType2;
extern int currentStoryPage;
//	グラフィックハンドル
extern int event_GraphHandlesList[EVENT_NUM];
extern int event_first;

void LoadGraphEvent(void);
void InitializeStory(void);
void DrawStory(int StoryPage);
void DrawPicture(RectOfPicture Type,int StoryPage);
void DrawEventButton(int left, int top, int right, int bottom, int storyPage);
void ChoiceEvent();
const char* ResultChoice(int choice);	//	選択イベントの結果を返す
