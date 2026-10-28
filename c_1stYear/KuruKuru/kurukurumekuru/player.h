#pragma once
#include "DxLib.h"

#define TIME_SIZE_CHANGE 10.0f
#define BOOK_MARK_WIDTH 100
#define BOOK_MARK_HEIGHT 460

#define MAX_HP_OF_PLAYER 100
#define MAX_MP_OF_PLAYER 100



struct RectOfPlayer
{
	int left;
	int top;
	int right;
	int bottom;
};

//	共通宣言
extern RectOfPlayer playerGraph;	//	プレイヤー画像頂点
extern float currentPlayerHP;	//	現在HP
extern float currentPlayerMP;	//	現在MP
extern float maxHP;	//	最大HP
extern float maxMP;	//	最大MP
extern int player_GraphHandle;	//	プレイヤー画像グラフィックハンドル

//	栞用
extern int bookMarkWidth;
extern int bookMarkHeight;
extern float timeForSizeChange;

//	プロトタイプ宣言
void LoadGraphPlayer(void);	//	プレイヤー画像読み込み
void UpdatePlayerStatus(void);	//	プレイヤーステータス更新
void ChangeSizeBookMark(void);	//	栞のサイズ変更
void DrawPlayer(void);	//	プレイヤー描画
void DamagePlayerByAttack(int damage);	//	攻撃によるダメージ

