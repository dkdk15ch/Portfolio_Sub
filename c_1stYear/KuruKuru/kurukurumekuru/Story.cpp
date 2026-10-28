#include "Story.h"
#include "Battle.h"
#include "book.h"
#include "button_detection.h"
#include "enemy.h"
#include "player.h"
#include "SceneTransition.h"
#include "ScreenInfomation.h"
#include "fonts.h"
#include "Text.h"
#include "time.h"
#include "PlayGuide.h"
#include <DxLib.h>
#include <string>



int storyFonts[NUM_OF_STORY_FONTS];	//	フォント用配列
//	現在のストーリー情報
int currentStoryTextIndex = 0;	
int currentTextPosX = STORY_START_LEFT_POSX;	//	ストーリー描画位置x
int currentTextPosY = STORY_START_POSY;	//	ストーリー描画位置y
int storyWidth, storyHeight, storyLineCount;	//	テキストサイズ用
//int eventLog[STORY_PAGE_MAX] = { 0 };	//	イベント履歴
StoryLog storyLog;
int currentStoryPage = 0;	//	現在のストーリーページ保持
//	ChoiceEvent用
int choice1 = 0;	//	選択イベント（左）
int choice2 = 0;	//	選択イベント（右)
int choiceTextPosX = 0;
int choiceTextPosY = 0;
int choiceTextWidth, choiceTextHeight, choiceTextLineCount;
float countOfChoiceText = 0.0f;
bool isSelectedChoiceEvent = false;
int selectedChoiceNo = -1;

//	状態管理
bool isDecidedEvent = false;	//	選択イベントの抽選状態
bool isSuccessfulMedicineEvent = false;	//	選択イベント（薬売り）の成功状態

RectOfPicture pictureType1 ={10,155,260,355};	//	左ページ挿絵位置
RectOfPicture pictureType2 = { 280,155,520,355 };	//	右ページ挿絵位置
ButtonRect eventButton = {0,0,0,0};	

//	ストーリー用
char storyTexts[6][STORY_ROW][49] =
{

	//{"竜の秘宝を求め、竜の山へ","入り口で、?","と出会った"},
	{"・・・古本屋にいたはずだが","ここは？","なんだこの本","ここを出たければ・・・"},
	{"その後、先へと進み","中腹で、?","と出会った"},
	{"その後、先へと進み","滝で、?","と出会った"},
	{"その後、先へと進み","頂上手前で?","と出会った"},
	{"頂上にたどり着いた","ボスドラゴンと","戦闘になった"},
	{"クリア","",""}

};

//	イベント用
char eventTexts[4][2][41] =
{
	{"防具屋","防具を手に入れた"},	//	防具売り 最大HP+10
	{"魔法使い","魔法道具をもらった"},	//  最大MP　+10 
	{"神父○○","回復魔法を受けた。"},	//	神父 体力20回復
	{"怪しい薬売り","怪しい薬を飲んだ。"}	//	怪しい薬売り 最大体力+20(HP20回復) or -10
};

//	基本操作説明
extern char basicControlGuideTexts[CONTROL_GUIDE_NUM][53] = {
	{"基本操作\nマウス左クリックとドラッグ"},
	{""},
	{"・対象をクリックで選択"},
	{"・PlayGuide:"},
	{"　右の栞をクリックで開閉"},
	{"　各項目クリックでで詳細"},
	{"・NEXT"},
	{"　ドラッグしてクリックを"},
	{"　離すと次へ進む"}
};

//	グラフィックハンドル
int event_GraphHandlesList[EVENT_NUM];
int choiceResultGraphHandle;
int event_first;

void LoadGraphEvent()	//	画像のロード
{
	event_GraphHandlesList[EVENT_ARMORER_SHOP]= LoadGraph("event_aromorer_shop.png", true);	//	選択イベント（防具屋)
	event_GraphHandlesList[EVENT_WIZARD] = LoadGraph("event_wizard.png", true);	//	選択イベント（魔法使い）
	event_GraphHandlesList[EVENT_PRIST] = LoadGraph("event_prist.png", true);	//	選択イベント(僧侶)
	event_GraphHandlesList[EVENT_MEDICINE_CELLER] = LoadGraph("event_medicine.png", true);	//	選択イベント(薬売り)
	choiceResultGraphHandle = LoadGraph("event_icon.png", true);	//	イベント用アイコン
	event_first = LoadGraph("event_first.png", true);	//	最初のイベント画像
}

void InitializeStory()	//	フォントハンドル定義
{
	storyFonts[FONT_STORY_TEXT] = CreateFontToHandle(NULL, 17, -1);
	storyFonts[FONT_STORY_BIG_TEXT] = CreateFontToHandle(NULL, 50, -1);
	storyFonts[FONT_STORY_CHOICE1] = CreateFontToHandle(NULL, 50, -1);
	storyFonts[FONT_STORY_CHOICE2] = CreateFontToHandle(NULL, 36, -1);
}

const char* result;


const char* ResultChoice(int choice)	//	選択イベントの結果を返す
{
	switch (choice)	//	各選択イベントで処理を分岐
	{
		case EVENT_ARMORER_SHOP:
		{
			result = "MaxHP";
			return result;
		}
		case EVENT_WIZARD:
		{
			result = "maxMP";
			return result;
		}
		case EVENT_PRIST:
		{
			result = "HP";
			return result;
		}
		case EVENT_MEDICINE_CELLER:
		{
			if (isSuccessfulMedicineEvent == false)
			{
				result = "maxHP";
			}
			else
			{
				result = "maxHP maxMP";
			}
			return result;
		}
		default:
		{
			result = "ERROR";
			return result;
		}
	}
}

unsigned int resultColor = 0xff7f;

void DrawChoiceResult(int choice)
{
	countOfChoiceText += deltaTime;

	if (countOfChoiceText > 2.0f)//	2秒経ったら次へ進む
	{
		countOfChoiceText = 0.0f;	//	カウント初期化
		selectedChoiceNo = -1;	//	初期化
		isDecidedEvent = false;	//	選択イベントの抽選状態

		encounteredEnemy++;			//	次の敵へ
		UpdateScreen(ScreenType::Battle);	//	画面遷移
	}

	//	ステータスの上昇低下などを可視化
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);//	半透明な背景
	int font = storyFonts[FONT_STORY_CHOICE1];
	float angle = 0;//通常は↑向き
	static const double pi = 3.141592653589793;	//	π
	if (selectedChoiceNo == EVENT_MEDICINE_CELLER)
	{
		if (isSuccessfulMedicineEvent == true)
		{
			resultColor = 0xff7f;	//	背景色変更
			font = storyFonts[FONT_STORY_CHOICE2];
		}
		else
		{
			resultColor = 0xff6347;//	背景色変更
			angle = pi * 180 / 180;//	↓向きに
		}
	}
	
	DrawBoxAA(SCREEN_HORI_CENTER - 150, SCREEN_VERT_CENTER - 50, SCREEN_HORI_CENTER + 150, SCREEN_VERT_CENTER + 50, resultColor, true);//	背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);//	αを戻す

	TextPosCalculate(&choiceTextWidth, &choiceTextHeight, &choiceTextLineCount, font, result, SCREEN_HORI_CENTER, SCREEN_VERT_CENTER, &choiceTextPosX, &choiceTextPosY);//	テキスト位置計算
	DrawFormatStringToHandle(choiceTextPosX, choiceTextPosY, 0x333333, font, result);//	テキスト描画
	int iconSize = choiceTextHeight;
	DrawRotaGraph(choiceTextPosX + choiceTextWidth + iconSize / 2, choiceTextPosY + iconSize/2, iconSize/256.0, angle, choiceResultGraphHandle, true);//上下アイコン描画

}


void RunChoiceEvent(int choice)	//	選択イベントの実行処理
{
	if (currentTutorial == 5)	//	チュートリアル中の処理
	{
		isRunningTutorial = false;
		currentTutorial++;
		isViewingPlayGuide = false;
	}

	isSelectedChoiceEvent = true;	//	選択イベント選択済みに
		
	switch (choice)	//	各選択イベントで処理を分岐
	{
		case EVENT_ARMORER_SHOP://	防具屋
		{
			maxHP += 10;		//	最大体力+10
			currentPlayerHP += 10;	//	現在のHPも増加分増やす
		}
		break;
		case EVENT_WIZARD:	//	魔法使い
		{
			maxMP += 10;		//	最大MP+10
			currentPlayerMP += 10;	//	現在のMP+10
		}
		break;
		case EVENT_PRIST:	//	僧侶
		{
			currentPlayerHP += 20;	//	体力を20回復
			if (currentPlayerHP > maxHP)	//	HP上限
				currentPlayerHP = maxHP;
		}
		break;
		case EVENT_MEDICINE_CELLER:	//	薬売り
		{
			//	最大体力+10 of -10
			if (GetRand(100) % 2 == 0)	//	成功
			{
				maxHP += 20;	//	最大HP+20
				currentPlayerHP += 20;	//	現在のHP+20
				maxMP += 10;		//	最大MP+10
				currentPlayerMP += 10;	//	現在のMP+10
				isSuccessfulMedicineEvent = true;
			}
			else//	失敗
			{
				maxHP -= 10;
				if (currentPlayerHP > maxHP)
					currentPlayerHP = maxHP;
				isSuccessfulMedicineEvent = false;
			}
		}
	}
	result = ResultChoice(choice);
}




//	ストーリー描画
void DrawStory(int storyPage)
{
	int pageEnd;	//	ページ内の描画限界位置
	if (storyPage % 2 == 0 || storyPage == 0)	//	storyPage偶数0,2...で左側、基数で1,3...で右側と判断してX座標を変える
	{
		//	左ページ
		currentTextPosX = STORY_START_LEFT_POSX;
		pageEnd = STORY_LEFT_PAGE_END;
		DrawFormatString(pageLeft.left + 10, pageLeft.bottom - 20, 0xffffff, "%d", storyPage+1);
	
		//	ストーリー前半
		for (int i = 0; i < STORY_ROW; i++)
		{
			//	ストーリー描画
			DrawFormatStringToHandle(currentTextPosX, currentTextPosY, 0xffffff, storyFonts[FONT_STORY_TEXT], storyTexts[storyPage][currentStoryTextIndex]);
			//	テキスト位置を更新
			GetDrawFormatStringSizeToHandle(&storyWidth, &storyHeight, &storyLineCount, storyFonts[FONT_STORY_TEXT], storyTexts[storyPage][currentStoryTextIndex]);

			currentTextPosY += storyHeight;
			currentStoryTextIndex++;
		}
	}
	else
	{
		//	右ページ
		currentTextPosX = STORY_START_RIGHT_POSX;
		pageEnd = STORY_RIGHT_PAGE_END;
		DrawFormatString(pageRight.right -20, pageRight.bottom - 20, 0xffffff, "%d", storyPage+1);
	
		//	プレイガイド描画
		unsigned int textColor = 0xffffff;

		for (int i = 0; i < CONTROL_GUIDE_NUM; i++)
		{
			DrawFormatStringToHandle(currentTextPosX, currentTextPosY, textColor, storyFonts[FONT_STORY_TEXT], basicControlGuideTexts[i]);
				
			currentTextPosY += storyHeight;
			if(i==0)
				currentTextPosY += storyHeight;
		}
	}

	currentTextPosY = STORY_START_POSY;
	currentStoryTextIndex = 0;

	//	ストーリー後半（イベントクリアで描画）
	if (storyLog.isEventCleared[storyPage] == true)
	{
		//	イベント内容に応じてテキストを描画
		//	描画位置更新
		if (storyPage % 2 == 0 || storyPage == 0)
		{
			//	左ページ
			currentTextPosX = STORY_START_LEFT_POSX;
			currentTextPosY = pictureType1.bottom + 10;

		}
		else
		{
			//	右ページ
			currentTextPosX = STORY_START_RIGHT_POSX;
			currentTextPosY = pictureType2.bottom + 10;
		}
	}	

	unsigned int nextColor = 0xffffff;
	//次のページに進むボタン
	DrawFormatStringToHandle(pageRight.right - 110, pageRight.bottom - 60, 0xffff00, storyFonts[FONT_STORY_BIG_TEXT], "NEXT");
		if (isPause == false && true == (DetectDrug(270, 10, 530, 470, &nextColor, 0xffff00,ANI_TYPE_NEXT)))
		{
			UpdateScreen(ScreenType::Battle);
		}
}

void DrawEventButton(int left,int top, int right, int bottom, int storyPage)
{
	//	イベントがクリア済みかで描画を分岐
	if (storyLog.isEventCleared[storyPage] == false)	//	未クリア時
	{
		//	ボタン描画
		DrawBoxAA(left, top, right, bottom, 0x00ffff, true);
		DrawFormatStringToHandle(left + 30, top, 0x000000, storyFonts[FONT_STORY_TEXT], "？EVENT");
	}
	else//	クリア時
	{
		DrawBoxAA(left, top, right, bottom, 0x000000, true);
		DrawFormatStringToHandle(currentTextPosX - 13, currentTextPosY, 0xffffff, storyFonts[FONT_STORY_TEXT], "%s",eventTexts[storyLog.eventLog[storyPage]][0]);
	}
}

void DrawPicture(RectOfPicture Type,int storyPage)
{
	unsigned int boxColor = 0x770000;

	//	イベントがクリアしていなければ、挿絵の当たり判定で選択イベントへ
	if (storyLog.isEventCleared[storyPage] == false)
	{
		if (isPause == false && true == DetectButtonCollision(Type.left, Type.top, Type.right, Type.bottom, &boxColor, 0xff0000))
		{
			//UpdateScreen(ScreenType::StoryChoice);
		}
		//	挿絵領域を描画
		DrawBoxAA(Type.left, Type.top, Type.right, Type.bottom, boxColor, true);
		DrawBoxAA(Type.left, Type.top, Type.right, Type.bottom, 0xff0000, false);
	}
	else
	{
		//	選択イベントの挿絵
		DrawBoxAA(Type.left, Type.top, Type.right, Type.bottom, 0xff0000, false);
		DrawFormatStringToHandle(Type.left + 10, Type.top + 100, 0xff0000, storyFonts[FONT_STORY_TEXT], "%sの挿絵",eventTexts[storyLog.eventLog[storyPage]][0]);
	}

	if (storyLog.isEventCleared[storyPage] == false)
	{
		//DrawFormatStringToHandle(Type.left + 70, Type.bottom -30, 0x000000, storyFonts[FONT_STORY_TEXT], "EVENT CHOICE");
		//DrawFormatStringToHandle(Type.left + 100, Type.top + 70, 0x000000, storyFonts[FONT_STORY_BIG_TEXT], "？");
	}


	//DrawFormatString(Type.left + 10, Type.top + 10, 0x000000, "挿絵です");
	DrawExtendGraph(Type.left+30, Type.top, Type.right-30, Type.bottom, event_first, true);

}


void ChoiceEvent()
{
	//	選択肢を抽選
	if (isDecidedEvent == false)
	{
		int i = 1;
		choice1 = GetRand(100)% EVENT_NUM;
		choice2 = GetRand(140)% EVENT_NUM;

		while (choice1 == choice2)	//	被っている間再抽選
		{	
			choice2 = GetRand(i*200) % EVENT_NUM;
			i++;
		}
		isDecidedEvent = true;
	}
		
	unsigned int choice1Color = 0x000000;	//	選択肢1用
	unsigned int choice2Color = 0x000000;	//	選択肢2用
	//	選択肢1
	if (isPause == false && isSelectedChoiceEvent == false)
	{
		if (true == (DetectButtonCollision(pageLeft.left, pageLeft.top, pageLeft.right, pageLeft.bottom, &choice1Color, 0xaaaa00)))
		{
			RunChoiceEvent(choice1);
			selectedChoiceNo = choice1;
			isSelectedChoiceEvent = false;
		}
	}
	//	選択肢1の描画
	DrawBoxAA(pageLeft.left, pageLeft.top, pageLeft.right, pageLeft.bottom, choice1Color, true);
	DrawBoxAA(pageLeft.left, pageLeft.top, pageLeft.right, pageLeft.bottom,0xffff00, false);
	DrawExtendGraph(pageLeft.left, pageLeft.top + 50, pageLeft.right, pageLeft.bottom-50, event_GraphHandlesList[choice1],true);
	DrawFormatString(5 + 10, 20, 0xffffff, "選択肢1");
	DrawFormatString(5 + 10, 40, 0xffffff, "%s",eventTexts[choice1][0]);

	//	選択肢2
	if (isPause == false && isSelectedChoiceEvent == false)
	{
		if (true == DetectButtonCollision(pageRight.left, pageRight.top, pageRight.right, pageRight.bottom, &choice2Color, 0xaaaa00))
		{
			RunChoiceEvent(choice2);
			selectedChoiceNo = choice2;
			isSelectedChoiceEvent = false;
		}
	}

	//	選択肢2の描画
	DrawBoxAA(pageRight.left, pageRight.top, pageRight.right, pageRight.bottom, choice2Color, true);
	DrawBoxAA(pageRight.left, pageRight.top, pageRight.right, pageRight.bottom, 0xffff00, false);
	DrawExtendGraph(pageRight.left, pageRight.top +50, pageRight.right, pageRight.bottom -50, event_GraphHandlesList[choice2], true);
	DrawFormatString(pageRight.left + 5 + 10, 20, 0xffffff, "選択肢2");
	DrawFormatString(pageRight.left + 5 + 10, 40, 0xffffff, "%s", eventTexts[choice2][0]);

	if (selectedChoiceNo != -1)
	{
		result = ResultChoice(selectedChoiceNo);
		DrawChoiceResult(selectedChoiceNo);	//結果の描画
	}
	
}

