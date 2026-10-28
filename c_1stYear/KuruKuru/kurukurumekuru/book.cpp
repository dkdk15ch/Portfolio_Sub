#include "book.h"
#include "button_detection.h"
#include "magic.h"
#include "DxLib.h"
#include "player.h"
#include "Battle.h"
#include "SceneTransition.h"
#include "animation.h"
#include "fonts.h"
#include "PlayGuide.h"
#include "ScreenInfomation.h"
#include <string>
#include <cmath>
#include <math.h>
#include "Util.h"

//	変数定義
PageRect pageLeft = { 5, 10, 265, 465 };	//	左ページの各座標
PageRect pageRight = { 270, 10, 530, 465 };	//	右ページの各座標
PageRect pageChange = { 485,15,525,55 };	//	
int currentPage = -1;

//	目押し用
float timingBarPos = 0;
float timingLength = pageRight.right - pageLeft.left;
float timingTime;
bool barDir;
bool isMoving = false;
unsigned int barColor;
unsigned int changeColor;

//	ページ更新
int indexOfPage;	//	ページ決定用配列の添え字
float centerPosX;	//	中心座標x
float centerPosY;	//	中心座標y
ParagraphPosition firstParagraph = {0,0,0,0,0,0,1};		//	1つ目の魔法名位置
ParagraphPosition secondParagraph = { 0,0,0,0,0,0,2 };	//	2つ目の魔法名位置
ParagraphPosition thirdParagraph = { 0,0,0,0,0,0,3 };	//	3つ目の魔法名位置
ParagraphPosition fourthParagraph = { 0,0,0,0,0,0,4 };	//	4つ目の魔法名位置
ParagraphPosition fifthParagraph = { 0,0,0,0,0,0,5 };	//	5つ目の魔法名位置
int drawPageAlpha = 255;
int pageNoPosX = pageRight.right - 120;	//	ページ数の位置x
int pageNoPosY = pageRight.bottom - 20;//	ページ数の位置y

//	新規ページか判別
bool alreadyTurnedPages[PAGE_MAX + 1] = { 0 };	//	既出ページかどうか
bool isNewPage = false;	//	新規ページ状態

//	新規カウント用
int newWords = 0;
int newPages = 0;
int countPageChange = 0;


void DrawBook(bool LastPage)//	本（枠）の描画
{
	static const double pi = 3.141592653589793;	//	π

	//	左ページ
	DrawLineAA(pageLeft.right, pageLeft.top, pageLeft.right + 20 * cos(pi * (30.0 / 180.0)), pageLeft.top + 20 * sin(pi * (30.0 / 180.0)), 0xffff00);	//	右側面斜辺(上)
	DrawLineAA(pageLeft.right, pageLeft.bottom, pageLeft.right + 20 * cos(pi * (30.0 / 180.0)), pageLeft.bottom + 20 * sin(pi * (30.0 / 180.0)), 0xffff00);	//	下側面斜辺(右)

	for (int i = 0; i < 5; i++)	//	下側面
	{
		DrawLineAA(pageLeft.left + (i * 4),
			pageLeft.bottom + i * (4.0 / 1.732),
			pageLeft.right + (i * 4),
			pageLeft.bottom + i * (4.0 / 1.732),
			0xffff00);
	}
	DrawLineAA(pageLeft.left, pageLeft.bottom, pageLeft.left + 20 * cos(pi * (30.0 / 180.0)), pageLeft.bottom + 20 * sin(pi * (30.0 / 180.0)), 0xffff00);	//	下側面斜辺(左)
	DrawBoxAA(pageLeft.left, pageLeft.top, pageLeft.right, pageLeft.bottom, 0xffff00, false);	//	開いてるページ（左）

	if (LastPage)
	{
		DrawBoxAA(pageRight.left, pageRight.top, pageRight.right, pageRight.bottom, 0x000000, true);	//	開いてるページ（右）
		DrawBoxAA(pageRight.left, pageRight.top, pageRight.right, pageRight.bottom, 0xffff00, false);	//	開いてるページ（右）
		return;
	}

	//	右ページ
	for (int i = 0; i < 5; i++)	//	右側面
	{	
		DrawLineAA(pageRight.right + i *4, 
			pageRight.top + (i * 4) * sin(pi * (30.0 / 180.0)), 
			pageRight.right + i * 4, 
			pageRight.bottom + (i * (4)) * sin(pi * (30.0 / 180.0)), 
			0xffff00);
	}

	DrawLineAA(pageRight.right, pageRight.top, pageRight.right + 20 * cos(pi * (30.0/180.0)), pageRight.top + 20 * sin(pi * (30.0 / 180.0)), 0xffff00);	//	右側面斜辺（上）
	DrawLineAA(pageRight.right, pageRight.bottom, pageRight.right + 20 * cos(pi * (30.0 / 180.0)), pageRight.bottom + 20 * sin(pi * (30.0 / 180.0)), 0xffff00);	//	下側面斜辺(右)
	
	for (int i = 0; i < 5; i++)	//	下側面
	{
		DrawLineAA(pageRight.left + (i * 4), 
			pageRight.bottom + i *(4.0/1.732), 
			pageRight.right + (i * 4) , 
			pageRight.bottom + i * (4.0 / 1.732), 
			0xffff00);
	}
	DrawLineAA(pageRight.left, pageRight.bottom, pageRight.left + 20 * cos(pi * (30.0 / 180.0)), pageRight.bottom + 20 * sin(pi * (30.0 / 180.0)), 0xffff00);	//	下側面斜辺(左)
	DrawBoxAA(pageRight.left, pageRight.top, pageRight.right, pageRight.bottom, 0x000000, true);	//	開いてるページ（右）
	DrawBoxAA(pageRight.left, pageRight.top, pageRight.right, pageRight.bottom, 0xffff00, false);	//	開いてるページ（右）
}

void DetectChangeButton()//　ページチェンジの処理
{
	if (currentTutorial == 2)	//	特定のチュートリアルで無効
		return;
	if (true == CheckIsPlayingAnima())	//	演出中無効
		return;

	//	捲りボタンを押したら動き出す。
	changeColor = 0xffffff;

		//	前フレームで押されていない時のみ
	{
		//	PageChange
		DrawCircleAA(SCREEN_WIDTH - 135, 35, 15, 16, 0xffffff, false, 8.0f);

		if (isPause == false && true == (DetectDrug(pageRight.left,pageRight.top,pageRight.right,pageRight.bottom, &changeColor, 0xff0000, ANI_TYPE_CHANGE)))
		{
			if (isMoving == false)
			{
				//	状態管理
				isMoving = true;
				//	初期化
				timingTime = 0;	
				isLogCleared = false;
				drawPageAlpha = 255;

				if (currentTutorial == 0)	//	チュートリアル中、次のチュートリアルへ
				{
					isRunningTutorial = false;
					currentTutorial ++;
				}

				if (currentTutorial == 4)	//	戦闘最後のチュートリアル中、非表示に
					isViewingPlayGuide = false;

				if (currentTutorial == 3 || currentTutorial == 4)	//	チュートリアル中、次のチュートリアルへ
				{
					if(currentPlayerMP != maxMP)	//	ターンはじめの捲りは処理を行わない
					{
						isRunningTutorial = false;
						currentTutorial ++;
					}
				}
					
				//	MPを消費して、ページをチェンジ。足りなかったらターン消費
				if (currentPlayerMP >= MP_COST_OF_CHANGE)
				{
					//	初期化
					currentPage = 0;	//	現在ページ
					isNewPage = false;	//	新規ページ判定

					currentPlayerMP -= MP_COST_OF_CHANGE;

					if (DoTDamage > 0)	//継続ダメージ
					{
						logList.clear();		//	前のログを削除
						isLogCleared = true;	//	ログの削除状態
						std::string playerlog2 = "エネミー:燃焼(継続ダメージ)";
						logList.push_back(playerlog2);
					}

					DoT();	//	捲ったタイミングで継続ダメージ

					UpdateScreen(ScreenType::BookTurnOver);	//	捲り用画面へ遷移
				}
				else
				{
					isOutOfMP = true;	//	MP切れ状態
					logList.clear();		//	前のログを削除
					isLogCleared = true;	//	ログの削除状態

					//	新しいログを追加
					std::string playerlog1 = "プレイヤー:";
					std::string playerlog2 = "MPが足りない";
					logList.push_back(playerlog1 + playerlog2);

					UpdateTurn();
				}
				countPageChange++;
					
			}
		}
	}
	//	捲りボタン描画
	DrawBoxAA(pageChange.left, pageChange.top, pageChange.right, pageChange.bottom, changeColor, false);
}

void MoveTimingBar()//	捲り時のタイミングバーの移動
{
	if (timingTime >= TIMING_TIME && barDir == true)
		barDir = false;
	else if (timingTime < 0 && barDir == false)
		barDir = true;

	//	タイミングバーの描画
	if (isMoving == true)
	{
		timingBarPos = pageRight.right - (timingTime / TIMING_TIME * timingLength);
		if (timingBarPos <= pageLeft.left)
			timingBarPos = pageLeft.left;

		DrawLineAA(timingBarPos, stopButton.bottom, timingBarPos, pageRight.bottom, barColor, 3.0f);
	}
}

void StopTimingBar()	//	捲り時のタイミングバーの停止
{
	barColor = 0xffffff;

	if (isPause == false && true == (DetectButtonCollision(stopButton.left, stopButton.top, stopButton.right, stopButton.bottom, &barColor, 0xff0000)))
	{
		isMoving = false;		//	移動状態状態を切り替え

		//	ページの決定(端数切り上げ）
		currentPage = static_cast<int>(ceil(PAGE_MAX * (timingTime / TIMING_TIME)));
		
		if (currentPage > PAGE_MAX)
			currentPage = PAGE_MAX;

		if (currentTutorial == 1)	//	チュートリアル中、次のチュートリアルへ
		{
			isRunningTutorial = false;
			currentTutorial = 2;
		}
		UpdateScreen(ScreenType::Battle);		//	バトル画面へ遷移
	}
}

void CheckMemory(ParagraphPosition paragraph, int index)//	覚えている詠唱文を確認
{
	unsigned int charColor = 0xffffff;	//	詠唱文用カラー

	int openSpells[NUM_OPEN_SPELL] = { 0 };	//	最大10文字開ける

	//	新しいページの時
	if (isNewPage == true)
	{

		int length = 0;
		//	詠唱文がすべて埋まっていなければ更新処理
		if (memorizedMagic[pageList[index]].isComplete == false)
		{
			//	新規文字を10個取得し、新規は強調表示させる。
			SRand(currentPage);
			//	新ページの空いてる文を決定
			//	詠唱文の長さを取得
			while (memorizedMagic[pageList[index]].spell[length] != '\0')
			{
				length++;
			}

			for (int i = 0; i < NUM_OPEN_SPELL; i++)
			{
				openSpells[i] = GetRand(length);
				//	空いていなかったら覚える
				if (memorizedMagic[pageList[index]].spell[openSpells[i]] == '0')
				{
					memorizedMagic[pageList[index]].spell[openSpells[i]] = magic[pageList[index]].spell[openSpells[i]];
					newWords++;	//	新規ワード数をカウント
				}
			}
		}
	}
	//	詠唱文がすべて埋まったかチェック
	CheckComplete(index);
	
	//	コンプリートでのボーナス
	if (memorizedMagic[pageList[index]].isComplete == true)
	{
		switch (pageList[index])
		{
		case 0:
		case 3:
		case 6:
			memorizedMagic[pageList[index]].damage += 10;
			break;
		case 1:
		case 4:
		case 7:
			memorizedMagic[pageList[index]].damage += 10;
			memorizedMagic[pageList[index]].mp -= 10;
			break;
		case 2:
		case 5:
		case 8:
			memorizedMagic[pageList[index]].damage += 10;
			memorizedMagic[pageList[index]].mp -= 10;
			memorizedMagic[pageList[index]].damage += 30;
		}
	}

	//	1文字ずつ詠唱文を描画
	int x = 290;
	int y = paragraph.spelPosY;
	int charWidth = GetDrawStringWidthToHandle("0", 1, fonts[FONT_INDEX_MAGIC_SPEL]);
	int charHeight = GetFontSizeToHandle(fonts[FONT_INDEX_MAGIC_SPEL]);

	//	チュートリアル強調表示用
	int left = x;
	int top = y;
	int right = 0;
	int bottom = 0;

	for (int i = 0; memorizedMagic[pageList[index]].spell[i] != '\0'; i++)
	{
		//	改行
		if (memorizedMagic[pageList[index]].spell[i] == '\n')
		{
			if (right < x)	//	チュートリアル強調表示用
				right = x;

			x = 290;
			y += charHeight;
			continue;
		}

		charColor = 0xffffff;

		if (isNewPage == true)
		{
			for (int j = 0; j < NUM_OPEN_SPELL; j++)
			{
				//	新規文字は強調表示
				if (i == openSpells[j])
				{
					charColor = 0xff0000;
				}
			}
		}
		if(memorizedMagic[pageList[index]].isComplete == true)
			charColor = 0xffd700;		//	コンプリートしていたら金色

		//	描画
		char currentChar[2] = { memorizedMagic[pageList[index]].spell[i],'\0' };
		if (memorizedMagic[pageList[index]].spell[i] != '0')
			DrawStringToHandle(x, y, currentChar, charColor, fonts[FONT_INDEX_MAGIC_SPEL]);
		else
			DrawBoxAA(x, y, x + charWidth, y + charHeight, 0x222222, true);
		//	次の描画位置
		x += charWidth;
	}

	if (right < x)	//	改行ない場合用
		right = x;
	bottom = y + charHeight;	//	チュートリアル強調表示用

	if (currentTutorial == 3)	//	チュートリアルで描画
		DrawBoxAA(left, top, right, bottom, 0x00ff00, false);

	//	見つけた文字数分ダメージを補正
	memorizedMagic[pageList[index]].damage = ContrastSpell(index)*3;
	
}

void DrawPage(ParagraphPosition paragraph, int index, float firstPosition = POSY_FIRST_LINE)
{
	float posX = 290 + 30;
	unsigned int nameColor = 0xffffff;
	
	if (isPlayingFadeOut == true)
		drawPageAlpha = FadeOut(drawPageAlpha);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, drawPageAlpha);//	α値

	//	アイコン
	DrawMagicIcon(290, firstPosition, index);

	//	呪文名、詠唱文、ダメージ、MPを表示
	//	呪文名	
	GetDrawFormatStringSizeToHandle(&textWidth, &textHeight, &textLineCount, fonts[FONT_INDEX_MAGIC_NAME], memorizedMagic[pageList[index]].name);
	paragraph.namePosY = firstPosition - (textHeight / 2.0f);	//	例外発生

	//	選択時の当たり判定(名前の部分のみ)位置
	float nameButtonRect[4] = { posX, paragraph.namePosY, posX + textWidth, paragraph.namePosY + textHeight };

	//	詠唱文
	paragraph.spelPosY = paragraph.namePosY + textHeight;
	GetDrawFormatStringSizeToHandle(&textWidth, &textHeight, &textLineCount, fonts[FONT_INDEX_MAGIC_SPEL], memorizedMagic[pageList[index]].spell);	//	例外発生

	//	ダメージ・MP
	if (memorizedMagic[pageList[index]].no % 3 == 0)
		paragraph.DMGPosY = paragraph.spelPosY + textHeight;
	else if (memorizedMagic[pageList[index]].no % 3 == 1)
		paragraph.DMGPosY = paragraph.spelPosY + textHeight;
	else
		paragraph.DMGPosY = paragraph.spelPosY + textHeight;

	GetDrawFormatStringSizeToHandle(&textWidth, &textHeight, &textLineCount, fonts[FONT_INDEX_DMG_MP], "10");

	//	描画範囲があれば書いて次の段落へ
	if (paragraph.DMGPosY + textHeight < pageRight.bottom)
	{
		if (drawPageAlpha == 255)	//	フェードアウト後は判定をとらない
		{
			//	当たり判定を取ったら攻撃
			if (isPause == false && true == (DetectButtonCollision(nameButtonRect[0], nameButtonRect[1], nameButtonRect[2], nameButtonRect[3], &nameColor, 0xff0000)))
			{
				selectedMagicNo = pageList[index];	//	選択された呪文No
				isAttackingOfPlayer = true;	//　攻撃フラグを立てる
				AdditionalEffect(selectedMagicNo);	//	追加効果
				UpdateTurn();	//	ターン更新処理

				if (currentTutorial == 4)	//	戦闘最後のチュートリアル中、非表示に
					isViewingPlayGuide = false;
				
				if (currentTutorial == 2 || currentTutorial == 3 || currentTutorial == 4)	//	チュートリアル中、次のチュートリアルへ
				{
					isRunningTutorial = false;
					currentTutorial++;
				}
			}
		}
		
		if (currentTutorial == 2) 
			DrawBoxAA(nameButtonRect[0], nameButtonRect[1], nameButtonRect[2], nameButtonRect[3], 0x00ff00, false);
			
		//	描画
		DrawFormatStringFToHandle(posX, paragraph.namePosY, nameColor, fonts[FONT_INDEX_MAGIC_NAME], memorizedMagic[pageList[index]].name);
		DrawFormatStringFToHandle(posX + 22, paragraph.DMGPosY, 0xffffff, fonts[FONT_INDEX_DMG_MP], "DMG:%d  MP:%d", memorizedMagic[pageList[index]].damage, memorizedMagic[pageList[index]].mp);

		//	詠唱文はチェックして描画
		CheckMemory(paragraph,index);


		//	次の段落へ
		switch (paragraph.Paragraph)
		{
		case 1:
			DrawPage(secondParagraph, index + 1, paragraph.DMGPosY + textHeight + 10);
			break;
		case 2:
			DrawPage(thirdParagraph, index + 1, paragraph.DMGPosY + textHeight + 10);
			break;
		case 3:
			DrawPage(fourthParagraph, index + 1, paragraph.DMGPosY + textHeight + 10);
			break;
		case 4:
			DrawPage(fifthParagraph, index + 1, paragraph.DMGPosY + textHeight + 10);
			break;
		}
	}	
	//ページ数の描画
	GetDrawFormatStringSizeToHandle(&textWidth, &textHeight, &textLineCount, fonts[FONT_INDEX_PAGE_NO], "ページ数:%d", currentPage);
	if (pageNoPosX + textWidth < pageRight.right - 10)
		pageNoPosX += 1;
	else if (pageNoPosX + textWidth > pageRight.right - 10)
		pageNoPosX -= 1;
	else
		pageNoPosX = pageRight.right - 10 - textWidth;

	DrawFormatStringToHandle(pageNoPosX, pageNoPosY, 0xffffff, fonts[FONT_INDEX_PAGE_NO], "ページ数:%d", currentPage);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);//	αを戻す
}

void UpdatePage()	//	ページ内容の更新
{
	//	ページ数から先頭の呪文を決定
	SRand(currentPage);
	indexOfPage = GetRand(256);

	//	新規ページか判別
	if (isNewPage == false)
	{
		if (alreadyTurnedPages[currentPage - 1] == false)
		{
			isNewPage = true;
			alreadyTurnedPages[currentPage - 1] = true;
			newPages++;
		}
	}

	//	ページ内容描画
	DrawPage(firstParagraph, indexOfPage);
}

void DrawResult()//	リザルト描画
{
	DrawFormatStringFToHandle(290, 30, 0xffffff, fonts[FONT_INDEX_MAGIC_NAME], "RESULT");

	DrawFormatStringFToHandle(290, 70, 0xffffff, fonts[FONT_INDEX_MAGIC_NAME], "NEW PAGE ; %d", newPages);
	DrawFormatStringFToHandle(290, 100, 0xffffff, fonts[FONT_INDEX_MAGIC_NAME], "NEW WORDS ; %d", newWords);

}

void ResetAlreadyTurnedPages()	//	既出ページかどうかの初期化
{	//	全てのページを見発見にする
	for(int i=0;i<PAGE_MAX+1;i++)	
		alreadyTurnedPages[i] = false;
}

