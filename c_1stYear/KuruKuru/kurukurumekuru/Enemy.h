#pragma once
#include "DxLib.h"

#define ENEMY_NUM 4	//	敵の種類総数
#define ENEMY_LIST_INDEX_A 0	
#define ENEMY_LIST_INDEX_B 1
#define ENEMY_LIST_INDEX_C 2
#define ENEMY_LIST_INDEX_D 3
#define ENEMY_GRAPH_SIZE 200	//	エネミー画像サイズ
#define ENEMY_GRAPH_SIZE_HALF 100	//	エネミー画像サイズ(半分)

#define EFFECT_OF_ENEMY_NUM 2	//	エフェクトの種類数
#define EFFECT_GRAPH_SIZE 30	//	エフェクトサイズ


//	アクションタイプ
#define ENEMY_ACTION_TYPE_ATTACK 0	//	攻撃
#define ENEMY_ACTION_TYPE_GUARD 1	//	防御
#define ENEMY_ACTION_TYPE_SPECIAL 2	//	特殊

struct ActionInfo
{
	const int no;	//	行動番号
	const char actionName[22];	//	行動名	
	const int actionType;	//	アクションタイプ
	const int turnsUntilAction;	//	行動までのターン
	const int Damage;	//	ダメージ
};

struct EnemyStatus
{
	const char name[ENEMY_NUM][22];
	float currentHP[ENEMY_NUM];
	const float maxHP[ENEMY_NUM];
	ActionInfo actionInfo[ENEMY_NUM][4];
};

struct RectOfEnemyGraph
{
	int left;
	int top;
	int right;
	int bottom;
};

//	共有宣言
extern int encounteredEnemy;	//	遭遇中の敵
extern EnemyStatus enemyStatusList;	//	敵情報管理
extern int turnsUntilCurrentAction;	//	現在の行動までのターン
extern bool isBattle;	//	戦闘状態
extern bool isEnemyDead;//	敵の死亡状態
extern int guardValue;
//	グラフィックハンドル
extern int enemy_GraphHandleList[ENEMY_NUM];	//	敵用

extern int effectOfEnemy_GraphHandlesList[EFFECT_OF_ENEMY_NUM];	//	追加効果用
extern int addEffectGraphIndex;	//	追加効果描画用
extern bool hasAddEffect;
extern RectOfEnemyGraph enemyGraphRect;	//	敵画像左半分の描画位置
extern RectOfEnemyGraph enemyGraphRight;//	敵画像右半分の描画位置


//	プロトタイプ宣言
void LoadGraphEnemy(void);	//	画像読み込み
float GetAverageScale(int graphHandle);	//	平均スケール取得
void ResetEnemyParm();	//敵のパラメータ初期化
void KillEnemy(void);	//	敵撃破時の処理
void DamageEnemyByAttack(int index);	//	敵の体力減少
void UpdateEnemyStatus(void);	//敵のステータス更新
void ChangeBrightOfEnemy(void);//	敵の輝度変更
void DrawEnemy(void);	//	敵の描画
void ActionOfEnemy(void);	//敵の行動
void DrawEnemyAction(void);	//敵の行動描画
const char* GetEnemyActionName();	//	敵の行動名取得
int GetEnemyActionDamage();	//	敵の攻撃ダメージを取得
void UpdateBattleLog(void);	//	敵の行動描画