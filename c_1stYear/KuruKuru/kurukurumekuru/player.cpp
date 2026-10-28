#include "player.h"
#include "time.h"
#include "button_detection.h"
#include "ScreenInfomation.h"
#include "Story.h"
#include "fonts.h"
#include "PlayGuide.h"
#include "animation.h"
#include "Battle.h"

//	プレイヤーステータス用
float maxHP = MAX_HP_OF_PLAYER;
float maxMP = MAX_MP_OF_PLAYER;
float currentPlayerHP = maxHP;
float currentPlayerMP = maxMP;
int HPLength = 0;
int MPLength = 0;

//	栞用
int bookMarkPosXo = 550;
int bookMarkWidth = 85;
int bookMarkHeight = 460;
float timeForSizeChange = TIME_SIZE_CHANGE;

//	状態管理用
bool isViewingPlayGuide = false;	//	プレイガイド表示の状態
extern bool prevFrameIsViewingPlayGuide = false;	//前フレームのプレイガイド状態

//	グラフィックハンドル
int player_GraphHandle;
RectOfPlayer playerGraph = {//	プレイヤー描画位置
	bookMarkPosXo + 5, 15, bookMarkPosXo + 80, 105
};



//	グラフィックハンドルの取得
void LoadGraphPlayer()
{
	player_GraphHandle = LoadGraph("player.png", true);
}

//	プレイヤーのステータス更新処理
void UpdatePlayerStatus()
{
	if (isViewingPlayGuide == true)
	{
		HPLength = 270 - 145;
		MPLength = 270 - 145;

	}
	else
	{
		HPLength = maxHP * 2;
		MPLength = maxMP * 2;

	}
		
	//	HP
	unsigned int HPColor = 0x00ff00;
	if (currentPlayerHP <= 30)
		HPColor = 0xff0000;
	DrawFormatString(bookMarkPosXo + 5, 115, 0xffffff, "HP");
	DrawFormatString(bookMarkPosXo + 5, 130, 0xffffff, "%3.0f", currentPlayerHP);
	DrawBoxAA(bookMarkPosXo + 5, 145 +HPLength * (1 - currentPlayerHP / maxHP), bookMarkPosXo + 40, 145 + HPLength, HPColor, true);
	DrawBoxAA(bookMarkPosXo + 5, 145, bookMarkPosXo + 40, 145+HPLength, 0x00ffff, false);
	
	//	MP
	unsigned int MPColor = 0x0000ff;

		

	if (isOutOfMP == true)
		MPColor = 0xaa0000;
	DrawFormatString(bookMarkPosXo + 45, 115, 0xffffff, "MP");
	DrawFormatString(bookMarkPosXo + 45, 130, 0xffffff, "%3.0f", currentPlayerMP);
	//DrawBoxAA(bookMarkPosXo + 45, 145 + (maxMP*2) * (1 - currentPlayerMP / maxMP), bookMarkPosXo + 80, 145 + maxMP*2, MPColor, true);
	DrawBoxAA(bookMarkPosXo + 45, 145 + MPLength * (1 - currentPlayerMP / maxMP), bookMarkPosXo + 80, 145 + MPLength, MPColor, true);

	
	DrawBoxAA(bookMarkPosXo + 45, 145, bookMarkPosXo + 80, 145+MPLength, 0x00ffff, false);

}

//	栞のサイズ変更
void ChangeSizeBookMark()
{
	if(timeForSizeChange > TIME_SIZE_CHANGE/2)
		timeForSizeChange -= deltaTime;
	
	if (timeForSizeChange < 0)
	{
		timeForSizeChange = TIME_SIZE_CHANGE;

	}
	float ratio = timeForSizeChange / TIME_SIZE_CHANGE;
	bookMarkHeight = BOOK_MARK_HEIGHT * ratio;
	bookMarkWidth = BOOK_MARK_WIDTH * ratio;
}

//	プレイヤーの描画
void DrawPlayer()
{
	//ChangeSizeBookMark();
	unsigned int bookMarkColor = 0x007777;
	
	//if (isViewingPlayGuide == false)	//	通常時の描画
	{
		//	ボタンの当たり判定を取ったらプレイガイドを表示
		if (prevFrameIsViewingPlayGuide == false)
		{
			if (true == (DetectButtonCollision(bookMarkPosXo, 10, bookMarkPosXo + bookMarkWidth, 10 + bookMarkHeight, &bookMarkColor, 0x005555)))
			{
				isViewingPlayGuide = true;
				isPause = true;
			}
		}
			
		//	栞
		if (isViewingPlayGuide == false)	//	プレイガイド非表示時
		{
			DrawBoxAA(bookMarkPosXo, 10, bookMarkPosXo + bookMarkWidth, ((10 + bookMarkHeight) - bookMarkWidth / 2), bookMarkColor, true);

			DrawTriangleAA(bookMarkPosXo, ((10 + bookMarkHeight) - bookMarkWidth / 2), bookMarkPosXo, 10 + bookMarkHeight, bookMarkPosXo + bookMarkWidth / 2, ((10 + bookMarkHeight) - bookMarkWidth / 2), bookMarkColor, true);
			DrawTriangleAA(bookMarkPosXo + bookMarkWidth, ((10 + bookMarkHeight) - bookMarkWidth / 2), bookMarkPosXo + bookMarkWidth, 10 + bookMarkHeight, bookMarkPosXo + bookMarkWidth / 2, ((10 + bookMarkHeight) - bookMarkWidth / 2), bookMarkColor, true);
		}
		else//	プレイガイド表示時
			DrawBoxAA(bookMarkPosXo, 10, bookMarkPosXo + bookMarkWidth, 270, bookMarkColor, true);

		//	プレイヤー画像描画
		DrawBoxAA(bookMarkPosXo + 5, 15, bookMarkPosXo + 80, 105, 0x00ffff, false);
		
		if(isPlayingDamageEffect == false)
			DrawExtendGraph(playerGraph.left,playerGraph.top,playerGraph.right,playerGraph.bottom, player_GraphHandle, true);
		
		DrawFormatString(bookMarkPosXo + 10, 20, 0x00ffff, "Player");

		//	プレイヤーのステータス
		UpdatePlayerStatus();
	}
	//else//	プレイガイド表示時の描画
	if(isViewingPlayGuide == true)
	{
		//	ボタンの当たり判定を取ったらプレイガイドを非表示
		if (prevFrameIsViewingPlayGuide == true)
		{
			if (true == (DetectButtonCollision(SCREEN_HORI_CENTER - 50, 270, SCREEN_WIDTH - 5, 470, &bookMarkColor, 0x005555)))
			{
				isViewingPlayGuide = false;
				isPause = false;

				if (playGuideNo != 0)
				{
					playGuideNo = 0;
				}

			}
		}
		

		//	栞
		DrawBoxAA(SCREEN_HORI_CENTER - 5, 270, bookMarkPosXo, 470, bookMarkColor, true);
		DrawTriangleAA(bookMarkPosXo, 270, bookMarkPosXo, 470, bookMarkPosXo+bookMarkWidth, 270, bookMarkColor, true);
		DrawTriangleAA(SCREEN_HORI_CENTER - 50, 270, SCREEN_HORI_CENTER - 5, 270, SCREEN_HORI_CENTER - 5, 370, bookMarkColor, true);
		DrawTriangleAA(SCREEN_HORI_CENTER - 50, 470, SCREEN_HORI_CENTER - 5, 470, SCREEN_HORI_CENTER - 5, 370, bookMarkColor, true);

		//	プレイガイドの表示
		ViewPlayGuide();
		prevFramePlayGuideNo = playGuideNo;
		prevFrameIsViewingPlayGuide = isViewingPlayGuide;
	}
}

void DamagePlayerByAttack(int damage)	//	攻撃によるダメージ
{
	currentPlayerHP -= damage;
	if (currentPlayerHP < 0)
		currentPlayerHP = 0;
}

