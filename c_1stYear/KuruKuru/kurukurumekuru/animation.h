#pragma once
#include "DxLib.h"

//	捲りアニメーション用
#define PAGE_ANIMATION_TIME 0.001f

//	点滅用
#define UI_DURATION_OF_HIDE 0.5f	//	非表示時間
#define UI_DURATION_OF_BLINK 2.0f	//	表示/非表示の合計時間


//	ヒット演出用
#define COUNT_OF_HIT_EFFECT 0.5f

//	攻撃演出用
#define START_POSX_OF_ATTACK 550	//	攻撃開始位置x
#define START_POSY_OF_ATTACK 40	//	攻撃開始位置y
#define END_POSX_OF_ATTACK 110	//	攻撃終了位置x
#define END_POSX_OF_ATTACK 150	//	攻撃終了位置y

#define CIRECLE_R 20.0	//	円半径
//#define COUNT_OF_ATTACK_EFFECT 1.0f	//	攻撃エフェクト用カウント
#define COUNT_OF_ATTACK_MOVETIME 1.0f	//	

//	攻撃演出用
#define DAMAGAE_EFFECT_DURATION_OF_HIDE 0.2f	//	非表示時間
#define DAMAGAE_EFFECT_DURATION_OF_BLINK 0.4f	//	表示/非表示の合計時間

//	ダメージ演出用
#define DAMAGE_EFFECT_DURATION_OF_HIDE 0.3f	//	非表示時間

//	フェードアウト用
#define COUNT_OF_FADE_OUT 5.0f	//	フェードアウトまでのカウント

struct Line
{
	float startX;
	float startY;
	float endX;
	float endY;
};

struct AnimaRect
{
	float left;
	float top;
	float right;
	float bottom;
};

//	変数宣言
//	捲りアニメーション用
extern Line pageL_Animation;
extern Line pageR_Animation;
extern Line coverLeft;
extern Line coverRight;
extern AnimaRect bookLeft;
extern AnimaRect bookRight;
extern AnimaRect stopButton;
extern float angle;
extern float pageAnimationTime;

extern bool isPlayingBattleAnima;//	戦闘用アニメーション再生状態

//	ヒット演出
extern bool isHit;	//	ヒット状態

extern bool isPlayingAttackOfPlayer;	//	攻撃演出再生状態(プレイヤー)
extern bool isPlayingAttackOfEnemy;	//	攻撃演出再生状態(敵)
extern bool isPlayingDamageEffect;	//	ダメージ演出再生状態
extern bool isPlayingFadeOut;	//	フェードアウト再生状態



//	プロトタイプ宣言
void InitalizeAnimationFonts(void);
void pageAnimation(void);
void BlinkOfBox(int left, int top, int right, int bottom, int color);
void DrugAnimation(int cursorx, int cursory, int startx, int starty, int animationType);
void HitEffect(int* left, int* right, bool isEnemy = false);	//	ヒット演出
void AttackAnimationOfPlayer(void);	//	攻撃演出(プレイヤー)
void AttackAnimationOfEnemy(void);	//	攻撃演出(敵)
void ResetDamageEffectParm(void);//	ダメージ演出の初期化
void DamageEffect(int x1, int y1, int x2, int y2, int GHandle, int TransFlag);	//	ダメージ演出
int FadeOut(int alpha);//	フェードアウト
bool CheckIsPlayingAnima();	//	再生状態のチェック
