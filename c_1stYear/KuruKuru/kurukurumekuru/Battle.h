#pragma once
#include <list>
#include <string>

//	共有宣言
extern int currentTurn;	//	現在のターン
extern std::list<std::string> logList;	//	ログリスト
extern bool isLogCleared;	//	ログの消去状態
extern int currentTutorial; //	現在のチュートリアル
extern bool isRunningTutorial; //	チュートリアル進行状態
extern bool isAttackingOfPlayer;	//	プレイヤーの攻撃状態
extern bool isOutOfMP;	//	MP切れかの状態

//	プロトタイプ宣言
void UpdateTurn(void);
void Attack(int index);
void RunTutorial(void);
void ResetUpdateTurnParm(void);


