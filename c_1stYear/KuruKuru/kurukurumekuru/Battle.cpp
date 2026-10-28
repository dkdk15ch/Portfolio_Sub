#include "Battle.h"
#include "player.h"
#include "Enemy.h"
#include "magic.h"
#include "book.h"
#include "Text.h"
#include "fonts.h"
#include "PlayGuide.h"
#include "animation.h"
#include "button_detection.h"
#include "SceneTransition.h"
#include <list>
#include <string>

int currentTurn = 0;	//	現在のターン
//bool isSelecting = true;	//	選択画面かどうか
bool isLogCleared = false;	//	クリア状態
bool isAttackingOfPlayer = false;	//	プレイヤーの攻撃状態
bool isOutOfMP = false;	//	MP切れかの状態
int tutorialTextWidth, tutorialTextHeight, tutorialTextLineCount;	//	テキストサイズ用
int tutorialTextPosX, tutorialTextPosY;		//	チュートリアルテキスト位置	
int currentTutorial = 0;	//	現在のチュートリアル
bool isRunningTutorial = false;	//	チュートリアル状態管理用
std::list<std::string> logList;	//	Log用リスト
std::string playerlog1 = "プレイヤー:";
std::string effectlog = "エネミー：ずぶぬれ";

char tutorialTexts1[4][49] =
{"チュートリアル戦","まずはページを捲って攻撃","呪文強化","敵を倒してクリアしよう"};

void ResetUpdateTurnParm()	//	ターン更新時の初期化処理
{
	//初期化(MP,アニメションフラグ)
	currentPlayerMP = maxMP;	//	MPを回復
	isMoving = true;	//	捲りアニメーション
	isPlayingBattleAnima = false;	//	バトルアニメション再生状態
}

//	ターンの更新処理
void UpdateTurn()
{
	if (isAttackingOfPlayer == true)
	{	
		Attack(selectedMagicNo);	//	プレイヤーの行動を実行
		isAttackingOfPlayer = false;
	}
	ActionOfEnemy();	//	敵の行動を実行
	
	//	ターン更新時の処理
	currentTurn += 1;

	if (addEffectGraphIndex == INDEX_OF_WATER_EFFECT_GRAPH)
	{
	}
	else if (DoTDamage != MAX_DOT_SECOND && DoTDamage != MAX_DOT_THIRD)	//	継続ダメージが減ってなければ初期化しない
	{
		DoTDamage = 0;		//	継続ダメージ初期化
		addEffectGraphIndex = -1;	//	追加効果初期化
		hasAddEffect = false;	//	追加状態の有無切り替え
	}
};

//	プレイヤーの攻撃時の処理
void Attack(int magicNo)
{
	logList.clear();		//	前のログを削除
	isLogCleared = true;	//	ログの削除状態

	if (currentPlayerMP < memorizedMagic[magicNo].mp)	//	MPが足りないとき
	{
		isOutOfMP = true;

		//	不足時何も起きずにターン消費
		//	新しいログを追加
		std::string playerlog1 = "プレイヤー:";
		std::string playerlog2 = "MPが足りない";
		logList.push_back(playerlog1 + playerlog2);\

		addEffectGraphIndex = -1;	//	追加効果初期化
		hasAddEffect = false;	//	追加状態の有無切り替え
	}
	else//	MPが足りる時、攻撃処理（ダメージは演出終了後に与える）
	{
		isPlayingAttackOfPlayer = true;	

		currentPlayerMP -= memorizedMagic[magicNo].mp;		//	MP消費	
		if (guardValue < 0)	//	ガード値があったら初期化
			guardValue = 0;

		//	新しいログを追加
		std::string playerlog2 = memorizedMagic[magicNo].name;
		logList.push_back(playerlog1 + playerlog2);
		//	追加効果時のログ追加
		if (addEffectGraphIndex == INDEX_OF_WATER_EFFECT_GRAPH)
			logList.push_back(effectlog);
		
		if (enemyStatusList.currentHP[encounteredEnemy] < 0) //	敵のHPの下限
			enemyStatusList.currentHP[encounteredEnemy] = 0;
	}
};

void ClearBattleLog()
{
	logList.clear();		//	前のログを削除
	isLogCleared = true;	//	ログの削除状態
}

//	バトルログの更新処理
void UpdateBattleLog()
{
	DrawFormatString(15, 380, 0xffffff, "Log");	//	ログ描画

	int x = 15;
	int y = 400;
	if (logList.empty() == false)	//	ログがあったら順次書き込む
	{
		for (std::string log : logList)
		{
			DrawString(x, y, log.c_str(), 0xffffff);	//	ログ描画
			y += 20;
		}
	}
}

//	チュートリアル初期化処理
void ResetTutorial()
{

}

//	チュートリアル戦用
void RunTutorial()
{
	if (currentTutorial > 5)	//	全てのチュートリアルが終わっていたら抜ける
		return;

	unsigned int gameGuideColor;	//	ゲームガイド用カラー
	unsigned int skipColor;	//	スキップボタンのカラー
	tutorialTextPosX = 290;	//	チュートリアルテキスト位置x
	tutorialTextPosY = 70;	//	チュートリアルテキスト位置y

	//	ゲームの流れ
	if (currentPage == 0)	//	ページ内容が描かれていない時のみ
	{
		if (screenCurrent == ScreenType::Battle)	//	戦闘画面のみ
		{
			for (int i = 0; i < 4; i++)
			{
				gameGuideColor = 0x555555;	//	デフォルトのカラー

				if (i == 0)	//	先頭は赤
					gameGuideColor = 0xff0000;
				else if (i == 1)	//	チュートリアル中、該当部分を強調
				{
					if (currentTutorial == 0 || currentTutorial == 1 || currentTutorial ==2)
						gameGuideColor = 0xffffff;
				}
				else if (i == 2)	//	チュートリアル中、該当部分を強調
				{
					if (currentTutorial == 3)
						gameGuideColor = 0xffffff;
				}
				else if (i == 3)	//	チュートリアル中、該当部分を強調
				{
					if (currentTutorial >= 4)
						gameGuideColor = 0xffffff;
				}
				GetDrawFormatStringSizeToHandle(&tutorialTextWidth, &tutorialTextHeight, &tutorialTextLineCount, storyFonts[FONT_STORY_TEXT], tutorialTexts1[i]);	//	テキストサイズ取得
				DrawFormatStringToHandle(tutorialTextPosX, tutorialTextPosY, gameGuideColor, storyFonts[FONT_STORY_TEXT], tutorialTexts1[i]);
				tutorialTextPosY += tutorialTextHeight * 2;	//	改行
			}

			skipColor = 0x777777;	//	スキップのデフォルトカラー
			//	スキップボタンが押されたらチュートリアルをスキップ
			if (true == DetectButtonCollision(tutorialTextPosX, tutorialTextPosY, tutorialTextPosX + 200, tutorialTextPosY + 50, &skipColor, 0xffffff))
			{	//	チュートリアルを最後より後に飛ばし、各種初期化
				currentTutorial = 6;	
				isRunningTutorial = false;
				isViewingPlayGuide = false;
				isPause = false;
			}
			//	スキップボタン描画
			DrawBoxAA(tutorialTextPosX, tutorialTextPosY, tutorialTextPosX + 200, tutorialTextPosY + 50, skipColor, false);	
			DrawFormatStringToHandle(tutorialTextPosX + 10, tutorialTextPosY + 10, skipColor,storyFonts[FONT_STORY_TEXT], "Tutorial Skip");
		}
	}
	
	switch (currentTutorial)	//	現在のチュートリアルに応じて各処理を行う
	{
		case 0:	//	PageChange①
		{
			//	初回処理
			if (isRunningTutorial == false)
			{
				isRunningTutorial = true;
				isViewingPlayGuide = true;
				playGuideNo = 1;
			}
			BlinkOfBox(pageRight.left, pageRight.top, pageRight.right, pageRight.bottom, 0x00ff00);				//	説明する部分を強調表示
		}
		break;
		case 1: // PageChange②③
		{
			//	初回処理
			if (isRunningTutorial == false)
			{
				isRunningTutorial = true;
				isViewingPlayGuide = true;
				playGuideNo = 1;
			}
			//	強調表示
			BlinkOfBox(stopButton.left, stopButton.top, stopButton.right, stopButton.bottom, 0x00ff00);
		}
		break;
		case 2:	//	攻撃
		case 3:	//	呪文強化
		{
			//	初回処理
			if (isRunningTutorial == false)
			{
				isRunningTutorial = true;
				isViewingPlayGuide = true;
				playGuideNo = currentTutorial;
			}
		}
		break;
		case 4:	//	敵の攻撃
		{
			//	初回処理
			if (isRunningTutorial == false)
			{
				isRunningTutorial = true;
				isViewingPlayGuide = true;
				playGuideNo = currentTutorial;
			}
			//	強調表示(戦闘画面のみ描画)
			if (screenCurrent == ScreenType::Battle)
				BlinkOfBox(10, 320, 260, 375, 0x00ff00);
		}
		break;
		case 5:	//	選択イベント
		{
			//	選択イベント画面になったら初回処理
			if (screenCurrent == ScreenType::StoryChoice)
			{
				if (isRunningTutorial == false)
				{
					isRunningTutorial = true;
					isViewingPlayGuide = true;
					playGuideNo = currentTutorial;
				}
			}
		}
		break;
	}
}