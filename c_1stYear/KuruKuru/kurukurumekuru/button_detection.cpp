#include "button_detection.h"
#include "animation.h"

//	初期化
bool inputPrevMouseLeftState = false;
int cursorX;
int cursorY;
bool isPause = false;

//	ドラッグ用
int startX = 0;	//	始点x
int startY = 0;	//	始点y
int endX = 0;	//	終点x
int endY = 0;	//	終点y
bool isDrug = false;

int requiredDrugX = 0;	//	横方向の必要なドラッグ量
int requiredDrugY = 0;	//	縦方向必要なドラッグ量

//	ボタンのクリック検知
bool DetectButtonCollision(int left, int top, int right, int bottom, unsigned int* text, unsigned int textColor)
{

	if (cursorX >= left && cursorX <= right)
	{
		if (cursorY >= top && cursorY <= bottom)
		{
			*text = textColor;		//	テキストの色を変更

			if (inputPrevMouseLeftState == false)	//	前フレームで押してない時のみ
			{
				//	カーソルとボタンが当たっている時に、マウスの左ボタンが押されたらtrue
				if ((inputPrevMouseLeftState == false) && INPUT_MOUSE_LEFT)
				{
					return true;
				}
			}
		}
	}
	return false;
};

void ResetDrugParm()	//	ドラッグ用変数初期化
{
	endX = 0;
	endY = 0;
	startX = 0;
	startY = 0;
}

//	ドラッグ操作
bool DetectDrug(int left, int top, int right, int bottom, unsigned int* text, unsigned int textColor, int animationType)
{
	//	横方向の必要なドラッグ量
	if (animationType == ANI_TYPE_NEXT)	//	NEXTのドラッグ量
		requiredDrugX = -100;
	else if (animationType == ANI_TYPE_CHANGE)	//	PageChangeのドラッグ量
		requiredDrugX = -40;
	

	if (true == DetectButtonCollision(left, top, right, bottom, text, textColor))
	{	//	特定の位置でクリックされたら始点を設定
		startX = cursorX;	//	始点x
		startY = cursorY;	//	始点y
		isDrug = true;		//	ドラッグ状態
	}
	
	if (isDrug == true)	//	ドラッグ中の処理
	{
		if (INPUT_MOUSE_LEFT == false)	//	左クリックが押されていなかったらドラッグ終了
		{
			endX = cursorX;	//	終点x
			endY = cursorY;	//	終点y
			isDrug = false;	//	ドラッグ状態
	
			//	縦方向の必要なドラッグ量
			if (animationType == ANI_TYPE_NEXT)	//	NEXTのドラッグ量
				requiredDrugY = 1 + (endY - startY);
			else if (animationType == ANI_TYPE_CHANGE)	//	PageChangeのドラッグ量
				requiredDrugY = 1 - (endY - startY);
	
			if (endX - startX < requiredDrugX && requiredDrugY < 0)	//	一定量ドラッグしていたらtrueを返す
			{//	初期化して戻り値を返す
				ResetDrugParm();	//	初期化
				return true;
			}
			else//	ドラッグ量が不十分だった時の処理
			{
				ResetDrugParm();	//	初期化
			}
		}
		DrugAnimation(cursorX, cursorY, startX, startY, animationType);	//	ドラッグアニメーション
	}


	return false;
}

