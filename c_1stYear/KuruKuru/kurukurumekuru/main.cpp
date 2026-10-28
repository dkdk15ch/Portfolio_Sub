#include "DxLib.h"
#include "magic.h"
#include "player.h"
#include "Enemy.h"
#include "book.h"
#include "button_detection.h"
#include "animation.h"
#include "time.h"
#include "Battle.h"
#include "SceneTransition.h"
#include "fonts.h"
#include "Sound.h"
#include "Story.h"
#include "ScreenInfomation.h"
#include "Title.h"

///////////////////////////////////////////////////////////////////////////////////////								
//								
//	ここから＃defineを定義する							
//



//
//	ここまでで#defineを定義する
//
///////////////////////////////////////////////////////////////////////////////////////		


///////////////////////////////////////////////////////////////////////////////////////
//								
//	ここからグローバル変数を定義する
//




//
//	ここまででグローバル変数を定義する
//
///////////////////////////////////////////////////////////////////////////////////////


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//	ウィンドウモードで起動する						
	ChangeWindowMode(TRUE);

	// ＤＸライブラリの初期化							
	if (DxLib_Init() == -1) return -1;

	// 描画先画面を裏画面にする							
	SetDrawScreen(DX_SCREEN_BACK);
	///////////////////////////////////////////////////////////////////////////////////////								
	//								
	//	ここからローカル変数を定義する							
	//

	DATEDATA Date;	//	乱数用時刻

	//	各種初期化
	InitializeDeltaTime();
	InitializeFonts();
	InitalizeUIFonts();
	ResetMemory();
	InitializeStory();
	InitalizeAnimationFonts();
	CreateTitleFonts();

	//
	//	ここまででローカル変数を定義する
	//
	///////////////////////////////////////////////////////////////////////////////////////		




	////////////////////////////////////////////////////////////////////////////						
	//
	//	読込処理はここから
	//

	LoadGraphPlayer();	//	プレイヤー画像ロード
	LoadGraphEnemy();	//	敵用画像ロード
	LoadGraphEvent();	//	選択イベント用画像ロード
	LoadSound();		//	サウンドロード
	LoadGraphTitle();




	//
	//	ここまで読込処理を行う
	//
	////////////////////////////////////////////////////////////////////////////

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 裏画面の内容を表画面に反映する						
		ScreenFlip();
		// 画面全体を初期化						
		ClearDrawScreen();

		// 乱数のシードを設定（現在の時刻を使用）
		GetDateTime(&Date);
		SRand(Date.Sec+Date.Min*60);

		////////////////////////////////////////////////////////////////////////////						
		//
		//	ここからプログラムを入力する
		//

		//	デルタタイムの取得
		GetDeltaTime();

		//	マウス操作
		unsigned int cursorColor = 0xffffff;
		GetMousePoint(&cursorX, &cursorY);
		if (INPUT_MOUSE_LEFT && inputPrevMouseLeftState == false)
			cursorColor = 0xff0000;


		//	画面切り替え
		switch (screenCurrent)
		{
			case ScreenType::Title:
			{
				if (DrawTitle(deltaTime) == true)
				{
					UpdateScreen(ScreenType::Story);
				}
			}
			break;
			case ScreenType::Battle:
			{
				isMoving = false;
				unsigned int nextColor = 0x000000;

				if (isEnemyDead == true)
				{
					DrawBook();		//	本
					DrawEnemy();	//	Enemy
					UpdateBattleLog();	//	バトルログ更新
					DrawPlayer();	//	プレイヤー描画

					//	リザルト
					DrawResult();

					DrawFormatStringToHandle(pageRight.right - 110, pageRight.bottom - 60, 0xffff00, storyFonts[FONT_STORY_BIG_TEXT], "NEXT");
					if (isPause == false && true == (DetectDrug(270, 10, 530, 470, &nextColor, 0xffff00, ANI_TYPE_NEXT)))
					{
						enemyStatusList.currentHP[encounteredEnemy] = enemyStatusList.maxHP[encounteredEnemy];
						isEnemyDead = false;


						if (currentTutorial == 4)	//	チュートリアル中、次のチュートリアルへ
						{
							isRunningTutorial = false;
							currentTutorial++;
						}

						if (encounteredEnemy == ENEMY_LIST_INDEX_D)
						{
							UpdateScreen(ScreenType::GameClear);
						}
						else
						{
							UpdateScreen(ScreenType::StoryChoice);
						}
					}

				}
				else
				{
					if (isBattle == false)	//	戦闘開始の初期状態設定
					{
						ActionOfEnemy();	//	敵アクションの初回
						isBattle = true;
						newWords = 0;
						newPages = 0;
						currentPage = 0;	//	現在のページを初期化
						currentTurn = 0;	//	現在のターンを初期化
						//	戦闘開始時のログ
						logList.clear();
						std::string startlog = "が現れた";
						logList.push_back(enemyStatusList.name[encounteredEnemy] + startlog);
					}

					if (currentPlayerHP <= 0)
						screenCurrent = ScreenType::GameOver;	//	タイトル画面に遷移

					DrawBook();		//	本
					DrawEnemy();	//	Enemy
					UpdateBattleLog();	//	バトルログ更新

					if (isNewPage == true)	//	NewPage描画
						DrawFormatString(pageRight.left + 40, 20, 0xffffff, "New Page");

					//	ページ内容描画
					if (currentPage != 0)
					{
						//	UpdatePageで以下の処理
						//	①ページ内容描画②呪文の当たり判定、当たり判定を検知で③ターン更新処理
						//	ターン更新時：プレイヤーと敵行動、ターン経過、追加状態初期化
						UpdatePage();
					}

					DetectChangeButton();	//	ページチェンジボタンの処理

					RunTutorial();	//チュートリアル

					DrawPlayer();	//	プレイヤー描画

					if (isOutOfMP == true)	//	MP切れ演出(ヒット演出と同じ)
						HitEffect(&playerGraph.left, &playerGraph.right);

					if (isPlayingAttackOfPlayer == true)	//	攻撃演出（プレイヤー）
						AttackAnimationOfPlayer();


					if (isHit == true)	//	敵被弾時のヒット演出
					{
						ChangeBrightOfEnemy();
						HitEffect(&enemyGraphRect.left, &enemyGraphRect.right, true);
						DrawExtendGraph(enemyGraphRect.left, enemyGraphRect.top, enemyGraphRect.right, enemyGraphRect.bottom, enemy_GraphHandleList[encounteredEnemy], true);	//	敵画像描画
						SetDrawBright(255, 255, 255);	//	輝度を戻す

					}

					if (isOutOfMP == false && isPlayingAttackOfPlayer == false && isHit == false && isPlayingAttackOfEnemy == true)
						AttackAnimationOfEnemy();//攻撃演出(敵)、プレイヤー攻撃中は再生しない。

					if (isPlayingDamageEffect == true)
						DamageEffect(playerGraph.left, playerGraph.top, playerGraph.right, playerGraph.bottom, player_GraphHandle, true);

					if (isPlayingBattleAnima == true && CheckIsPlayingAnima() == false)
						ResetUpdateTurnParm();	//	初期化

					//	デバッグ用
					DrawFormatString(15, pageRight.bottom - 20, 0xffffff, "TURN: %d", currentTurn);
				}
			}
			break;
			case ScreenType::BookTurnOver:
			{
				//	目押し
				MoveTimingBar();
				if (isMoving == true)
				{
					//	停止ボタンの処理
					//	止めた瞬間に呪文選択できないように修正する
					StopTimingBar();

					DrawBoxAA(pageLeft.left, stopButton.bottom, pageRight.right, pageRight.bottom, 0x00ff00, false);
					//	ページアニメーション
					pageAnimation();
				}

				if (isRunningTutorial == true)
					RunTutorial();

				//	プレイヤーの描画
				DrawPlayer();
			}
			break;
			case ScreenType::Story:
			{
				DrawBook();				//	仮描画
				DrawStory(0);				//	仮のストーリ
				DrawPicture(pictureType1,0);
				DrawStory(1);				//	説明
				DrawPlayer();				//	プレイヤー描画
			}
			break;
			case ScreenType::StoryChoice:
			{
				DrawBook();

				isPlayingFadeOut = false;	//	フェードアウト状態初期化

				ChoiceEvent();	//	選択イベント

				if(currentTutorial == 5)
					RunTutorial();
				
				DrawPlayer();	//	プレイヤー描画
			}
			break;
			case ScreenType::GameOver:
			{
				//	メッセージの描画
				DrawFormatStringFToHandle(SCREEN_HORI_CENTER - 150, SCREEN_VERT_CENTER, 0xffffff, storyFonts[FONT_STORY_BIG_TEXT], "GAME OVER");

				//	決定キーが入力されたらタイトル画面に遷移するための条件式
				if (INPUT_MOUSE_LEFT && inputPrevMouseLeftState == false)
				{
					//	プレイヤーステータスの初期化
					maxHP = MAX_HP_OF_PLAYER;	//	maxHP
					maxMP = MAX_MP_OF_PLAYER;	//	maxMP
					currentPlayerHP = maxHP;	//	現在のHP
					currentPlayerMP = maxMP;	//	現在のMP
					ResetMemory();	//	覚えた呪文初期化

					currentTutorial = 0;	//	チュートリアル初期化
					ResetDamageEffectParm();	//	演出初期化
					ResetEnemyParm();	//	敵の初期化
					ResetDrugParm();//	ドラッグ用変数初期化
					ResetUpdateTurnParm();//	ターン更新時の初期化



					screenCurrent = ScreenType::Title;	//	ストーリー画面に遷移
				}
			}
			break;
			case ScreenType::GameClear:
			{
				DrawBook(true);

				//	メッセージの描画
				DrawFormatStringFToHandle(SCREEN_HORI_CENTER , SCREEN_VERT_CENTER - 200, 0xffffff, storyFonts[FONT_STORY_BIG_TEXT], "GAME \nCLEAR");

				//	決定キーが入力されたらタイトル画面に遷移するための条件式
				if (INPUT_MOUSE_LEFT && inputPrevMouseLeftState == false)
				{
					//	プレイヤーステータスの初期化
					maxHP = MAX_HP_OF_PLAYER;	//	maxHP
					maxMP = MAX_MP_OF_PLAYER;	//	maxMP
					currentPlayerHP = maxHP;	//	現在のHP
					currentPlayerMP = maxMP;	//	現在のMP
					ResetMemory();	//	覚えた呪文初期化

					currentTutorial = 0;	//	チュートリアル初期化
					ResetDamageEffectParm();	//	演出初期化
					ResetEnemyParm();	//	敵の初期化
					ResetDrugParm();//	ドラッグ用変数初期化
					ResetUpdateTurnParm();//	ターン更新時の初期化
					ResetAlreadyTurnedPages();	//ページの既出の初期化


					screenCurrent = ScreenType::Title;	//	ストーリー画面に遷移
				}

			}
			break;
		}

		//	マウス操作デバッグ用
		DrawCircleAA(cursorX, cursorY, 5, 8, 0xffffff, true);

		//	現在のフレームの入力状態を保存
		inputPrevMouseLeftState = INPUT_MOUSE_LEFT;

		//
		// 
		// 
		// 
		//
		// 
		// 
		//	ここまででプログラムを入力する
		//
		////////////////////////////////////////////////////////////////////////////						

	}

	// ＤＸライブラリの使用終了							
	DxLib_End();

	return 0;
}
