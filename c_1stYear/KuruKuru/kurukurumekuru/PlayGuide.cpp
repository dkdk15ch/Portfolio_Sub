#include "ScreenInfomation.h"
#include "PlayGuide.h"
#include "story.h"
#include "fonts.h"
#include "player.h"
#include "button_detection.h"
#include "Battle.h"
#include "Text.h"
#include "SceneTransition.h"

//	プレイガイド用
int playGuidePosX;
int playGuidePosY;
int playGuideWidth, playGuideHeight, playGuideLineCount;
int playGuideNo = 0;
int prevFramePlayGuideNo = 0;

//	操作説明用
char playGuideTexts[PLAY_GUIDE_NUM][76] =
{
	{"PageChange：",},	//  PageChange 
	{"攻撃：",},	//	攻撃
	{"呪文強化："},	//	呪文強化
	{"敵の攻撃："},	//	敵の攻撃
	{"選択イベント" }
};

char playGuideTexts1[ROW_OF_PLAY_GUIDE_TEXTS1][67] = {	//	PageChangeテキスト
	"PageChange",
	"①右のページをドラッグすると",
	"　ページを変えることができる",
	"②任意のタイミングで停止",
	"③捲るのにはMP30消費、",
	"　MPがあればターン中無制限"
	};

char playGuideTexts2[ROW_OF_PLAY_GUIDE_TEXTS2][67] = {	//	攻撃
	"攻撃",
	"①呪文名を選択して攻撃",
	"　ターンとMPを消費",
	"②MPが足りない行動をすると",
	"何も行わないでターン経過"
	};

char playGuideTexts3[ROW_OF_PLAY_GUIDE_TEXTS3][67] = {
	"呪文の強化",
	"①詠唱文を見つけるほど",
	"　呪文が強化される",
	"②新規の文字は赤",
	"　既存の文字は白"
};

char playGuideTexts4[ROW_OF_PLAY_GUIDE_TEXTS4][67] = {
	"敵の攻撃",
	"①NEXTに敵の次の行動",
	"",
	//"　捲りすぎに注意",
	//"②ページを捲るたびに変化",
	"　",
	""
};

char playGuideTexts5[ROW_OF_PLAY_GUIDE_TEXTS5][67] = {
	"選択イベント",
	"①戦闘後に発生する",
	"②神父:HP回復",
	"　魔法使い:最大MP+",
	"　防具屋:最大HP+",
	"　怪しい薬売り:???"
};



void ViewPlayGuide()
{
	//	プレイガイドの表示

	unsigned int textColor = 0xffffff;
	playGuidePosX = SCREEN_HORI_CENTER;
	playGuidePosY = 310;

	if (playGuideNo !=0 && isViewingPlayGuide == false)
		return;

	DrawFormatString(playGuidePosX, 280, 0x00ff00, "PLAY GUIDE");

	switch (playGuideNo)
	{
		case 0://	メニュー画面
		{
			for (int i = 0; i < PLAY_GUIDE_NUM; i++)
			{
				textColor = 0xffffff;
				//	テキストサイズ取得
				GetDrawFormatStringSizeToHandle(&playGuideWidth, &playGuideHeight, &playGuideLineCount, storyFonts[FONT_STORY_TEXT], playGuideTexts[i]);

				//	項目名で当たり判定を取り、クリックでその項目を表示
				if (prevFramePlayGuideNo == 0  && true == (DetectButtonCollision(playGuidePosX, playGuidePosY, playGuidePosX + playGuideWidth, playGuidePosY + playGuideHeight, &textColor, 0xffff00)))
				{		
					playGuideNo = i+1;
					isViewingPlayGuide = true;
					isPause = true;
				}

				//	描画
				DrawFormatStringToHandle(playGuidePosX, playGuidePosY, textColor, storyFonts[FONT_STORY_TEXT], playGuideTexts[i]);

				//	改行
				playGuidePosX = SCREEN_HORI_CENTER;;
				playGuidePosY += playGuideHeight;
			}
		}
		break;
		//	各項目を、項目名は緑、内容は白で描画
		case 1://	pageChange
			for (int i = 0; i < ROW_OF_PLAY_GUIDE_TEXTS1; i++)
			{
				//	テキストサイズ取得
				GetDrawFormatStringSizeToHandle(&playGuideWidth, &playGuideHeight, &playGuideLineCount, storyFonts[FONT_STORY_TEXT], playGuideTexts1[i]);

				if (i == 0)
					textColor = 0x00ff00;
				else
					textColor = 0xffffff;

				if (screenCurrent != ScreenType::Story)
				{
					switch (currentTutorial)	//	チュートリアル用表示
					{
						//	該当の部分以外を非強調
					case 0:
					{
						if (i > 2)
							textColor = 0x00bbbb;
					}
					break;
					case 1:
					{
						//if(i!=0 && i!=3)
						if (i == 1 || i == 2)
							textColor = 0x00bbbb;
					}
					break;
					}
				}
				DrawFormatStringToHandle(playGuidePosX, playGuidePosY, textColor, storyFonts[FONT_STORY_TEXT], playGuideTexts1[i]);

				playGuidePosY += playGuideHeight;
			}
		break;
		case 2://	攻撃
			for (int i = 0; i < ROW_OF_PLAY_GUIDE_TEXTS2; i++)
			{
				//	テキストサイズ取得
				GetDrawFormatStringSizeToHandle(&playGuideWidth, &playGuideHeight, &playGuideLineCount, storyFonts[FONT_STORY_TEXT], playGuideTexts2[i]);

				if (i == 0)
					textColor = 0x00ff00;
				else
					textColor = 0xffffff;

				DrawFormatStringToHandle(playGuidePosX, playGuidePosY, textColor, storyFonts[FONT_STORY_TEXT], playGuideTexts2[i]);

				playGuidePosY += playGuideHeight;
			}
			break;

		case 3://	呪文の強化
		{
			for (int i = 0; i < ROW_OF_PLAY_GUIDE_TEXTS3; i++)
			{
				//	テキストサイズ取得
				GetDrawFormatStringSizeToHandle(&playGuideWidth, &playGuideHeight, &playGuideLineCount, storyFonts[FONT_STORY_TEXT], playGuideTexts3[i]);

				if (i == 0)
					textColor = 0x00ff00;
				else
					textColor = 0xffffff;

				DrawFormatStringToHandle(playGuidePosX, playGuidePosY, textColor, storyFonts[FONT_STORY_TEXT], playGuideTexts3[i]);

				playGuidePosY += playGuideHeight;
			}

		}
		break;
		case 4://	敵の攻撃
		{
			for (int i = 0; i < ROW_OF_PLAY_GUIDE_TEXTS4; i++)
			{
				//	テキストサイズ取得
				GetDrawFormatStringSizeToHandle(&playGuideWidth, &playGuideHeight, &playGuideLineCount, storyFonts[FONT_STORY_TEXT], playGuideTexts4[i]);

				if (i == 0)
					textColor = 0x00ff00;
				else
					textColor = 0xffffff;

				DrawFormatStringToHandle(playGuidePosX, playGuidePosY, textColor, storyFonts[FONT_STORY_TEXT], playGuideTexts4[i]);

				playGuidePosY += playGuideHeight;
			}
		}
		break;
		case 5://	選択イベント
		{
			for (int i = 0; i < ROW_OF_PLAY_GUIDE_TEXTS5; i++)
			{
				//	テキストサイズ取得
				GetDrawFormatStringSizeToHandle(&playGuideWidth, &playGuideHeight, &playGuideLineCount, storyFonts[FONT_STORY_TEXT], playGuideTexts5[i]);

				if (i == 0)
					textColor = 0x00ff00;
				else
					textColor = 0xffffff;

				DrawFormatStringToHandle(playGuidePosX, playGuidePosY, textColor, storyFonts[FONT_STORY_TEXT], playGuideTexts5[i]);

				playGuidePosY += playGuideHeight;
			}

		}
		break;
	}

}

