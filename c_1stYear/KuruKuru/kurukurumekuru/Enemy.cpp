#include "Enemy.h"
#include "player.h"
#include "Battle.h"
#include "SceneTransition.h"
#include "Story.h"
#include "button_detection.h"
#include "animation.h"
#include "magic.h"
#include <list>

int turnsUntilCurrentAction;

RectOfEnemyGraph enemyGraphRect = {
10, 50, 10 + ENEMY_GRAPH_SIZE, 50 + ENEMY_GRAPH_SIZE
};

bool isInAction = false;	//	行動中フラグ、複数ターンかかる行動用
int actionNo = 0;	//	行動中のアクション

bool isBattle = false;	//	戦闘状態
bool isEnemyDead = false;//	敵の死亡状態
int encounteredEnemy = 0;//	戦闘中の敵を判別

int guardValue = 0;	//	ガード値

const char* executedActionName;	//	実行された敵の行動名
int executedActionDamage;	//	実行された敵行動のダメージ

EnemyStatus enemyStatusList = 
{
	//	1000かなり強い,500まぁ勝てる
	{"-自称-子羊","グリフォン","クラーケン","BOSS"},	//	name
	{150,500,800,1000},	//	currentHP
	{150,500,800,1000},	//	maxHP
	{
		{	//	enemyAのactionInfo
			{0,"サボっている",ENEMY_ACTION_TYPE_ATTACK,0,0},
			{1,"かみつく",ENEMY_ACTION_TYPE_ATTACK,1,10},
			{2,"かみちぎる",ENEMY_ACTION_TYPE_ATTACK,2,20}
		},
		{	//	enemyBのactionInfo
			{0,"ついばむ",ENEMY_ACTION_TYPE_ATTACK,0,10},
			{1,"とっしん",ENEMY_ACTION_TYPE_ATTACK,1,20},
			{2,"サイクロン",ENEMY_ACTION_TYPE_ATTACK,2,40}
		},
		{	//	enemyCのactionInfo
			{0,"たこすみ",ENEMY_ACTION_TYPE_ATTACK,0,10},
			{1,"たたきつける",ENEMY_ACTION_TYPE_ATTACK,1,20},
			{2,"おおなみ",ENEMY_ACTION_TYPE_ATTACK,3,70}
		},
		{	//	enemyDのactionInfo
			{0,"かみちぎる",ENEMY_ACTION_TYPE_ATTACK,0,20},
			{1,"サイクロン",ENEMY_ACTION_TYPE_ATTACK,1,30},
			{2,"ブレス",ENEMY_ACTION_TYPE_ATTACK,2,60}
		},
	}
};

//グラフィックハンドル
int enemy_GraphHandleList[ENEMY_NUM];
int deadEnemy_GraphHandleList[ENEMY_NUM];

int effectOfEnemy_GraphHandlesList[EFFECT_OF_ENEMY_NUM];
int addEffectGraphIndex = -1;	//	追加効果描画用
bool hasAddEffect = false;	//	追加状態の有無


void LoadGraphEnemy()
{
	enemy_GraphHandleList[0] = LoadGraph("enemy_wolf.png",true);
	enemy_GraphHandleList[1] = LoadGraph("enemy_griffon.png", true);
	enemy_GraphHandleList[2] = LoadGraph("enemy_kraken.png", true);
	enemy_GraphHandleList[3] = LoadGraph("enemy_dragon.png", true);

	//　死亡時用画像
	//	モノクロ化
	deadEnemy_GraphHandleList[0] = LoadGraph("enemy_wolf.png", true);
	deadEnemy_GraphHandleList[1] = LoadGraph("enemy_griffon.png", true);
	deadEnemy_GraphHandleList[2] = LoadGraph("enemy_kraken.png", true);
	deadEnemy_GraphHandleList[3] = LoadGraph("enemy_dragon.png", true);
	GraphFilter(deadEnemy_GraphHandleList[0], DX_GRAPH_FILTER_MONO, 30,3);
	GraphFilter(deadEnemy_GraphHandleList[1], DX_GRAPH_FILTER_MONO, 30,3);
	GraphFilter(deadEnemy_GraphHandleList[2], DX_GRAPH_FILTER_MONO, 30,3);
	GraphFilter(deadEnemy_GraphHandleList[3], DX_GRAPH_FILTER_MONO, 30,3);
	
	effectOfEnemy_GraphHandlesList[0] = LoadGraph("effect_fire.png", true);
	effectOfEnemy_GraphHandlesList[1] = LoadGraph("effect_water.png", true);
}

float GetAverageScale(int graphHandle)	//	平均スケール取得
{
	int originalWidth, originalHeight;
	GetGraphSize(graphHandle, &originalWidth, &originalHeight);

	float scaleX = 200.0 / (float)originalWidth;
	float scaleY = 200.0 / (float)originalHeight;
	float avgScale = (scaleX + scaleY) / 2.0f;

	return avgScale;
}

void ResetEnemyParm()	//敵のパラメータ初期化
{
	switch (encounteredEnemy)	//	HP初期化
	{
		case ENEMY_LIST_INDEX_D:
		{
			enemyStatusList.currentHP[ENEMY_LIST_INDEX_D] = enemyStatusList.maxHP[ENEMY_LIST_INDEX_D];
		}
		case ENEMY_LIST_INDEX_C:
		{
			enemyStatusList.currentHP[ENEMY_LIST_INDEX_C] = enemyStatusList.maxHP[ENEMY_LIST_INDEX_C];

		}
		case ENEMY_LIST_INDEX_B:
		{
			enemyStatusList.currentHP[ENEMY_LIST_INDEX_B] = enemyStatusList.maxHP[ENEMY_LIST_INDEX_B];

		}
		case ENEMY_LIST_INDEX_A:
		{
			enemyStatusList.currentHP[ENEMY_LIST_INDEX_A] = enemyStatusList.maxHP[ENEMY_LIST_INDEX_A];
		}
		break;
	}

	//	各種初期化
	isInAction = false;
	isBattle = false;
	isPlayingAttackOfEnemy = false;
	addEffectGraphIndex = -1;
	DoTDamage = 0;		//	継続ダメージ初期化

	ResetDrugParm();
	ResetUpdateTurnParm();

	encounteredEnemy = 0;
}

void KillEnemy()	//	敵撃破時の処理
{
	if (enemyStatusList.currentHP[encounteredEnemy] == 0)
	{
		isEnemyDead = true;

		//	各種初期化
		isInAction = false;
		isBattle = false;
		isPlayingAttackOfEnemy = false;
		addEffectGraphIndex = -1;
		DoTDamage = 0;		//	継続ダメージ初期化

		ResetDrugParm();
		ResetUpdateTurnParm();

		logList.clear();

		std::string killLog1 = enemyStatusList.name[encounteredEnemy];
		std::string killLog2 = "を撃破";
		logList.push_back(killLog1 + killLog2);

	}
}

void DamageEnemyByAttack(int magicNo)	//	敵の体力減少
{
	int dmg = GetMagicDamage(magicNo);

	enemyStatusList.currentHP[encounteredEnemy] -= dmg /* + guardValue*/;		//	ダメージ処理(攻撃のダメージ - ガード値)
	if (enemyStatusList.currentHP[encounteredEnemy] < 0)
		enemyStatusList.currentHP[encounteredEnemy] = 0;

	selectedMagicNo = 0;	//	初期化

	KillEnemy();
}

//	敵のステータス更新
void UpdateEnemyStatus()
{
	DrawFormatString(10, 250, 0xffffff, "HP: %3.0f",enemyStatusList.currentHP[encounteredEnemy]);
	DrawBoxAA(10 , 270, 260 - 250 * (1 - enemyStatusList.currentHP[encounteredEnemy] / enemyStatusList.maxHP[encounteredEnemy]), 310, 0x00ff00, true);
	DrawBoxAA(10, 270, 260, 310, 0x00ffff, false);
}

void ChangeBrightOfEnemy()
{
	if (hasAddEffect == true)	//	追加状態なら輝度を変更
	{
		if (addEffectGraphIndex == 0)	//	燃焼
			SetDrawBright(255, 150, 150);
		else if (addEffectGraphIndex == 1)	//	ずぶぬれ
			SetDrawBright(150, 150, 255);
	}

}

void DrawEnemy()	//	敵の描画
{
	ChangeBrightOfEnemy();	//	輝度変更
	//	敵の画像描画
	DrawBoxAA(10, 50, 260, 250, 0xff0000, false);

	if (isEnemyDead == false)//　生存時
	{
		if (isHit == false && isPlayingAttackOfEnemy == false)	//攻撃演出(敵)、プレイヤー攻撃中は再生しない。
			DrawExtendGraph(enemyGraphRect.left, enemyGraphRect.top, enemyGraphRect.right, enemyGraphRect.bottom, enemy_GraphHandleList[encounteredEnemy], true);	//	敵画像描画

		if (isPlayingAttackOfPlayer == true || isOutOfMP == true)
			DrawExtendGraph(enemyGraphRect.left, enemyGraphRect.top, enemyGraphRect.right, enemyGraphRect.bottom, enemy_GraphHandleList[encounteredEnemy], true);	//	敵画像描画	
	}
	else//	死亡時
		DrawExtendGraph(enemyGraphRect.left, enemyGraphRect.top, enemyGraphRect.right, enemyGraphRect.bottom, deadEnemy_GraphHandleList[encounteredEnemy], true);	//	敵画像描画

	SetDrawBright(255, 255, 255);

	DrawBoxAA(10, 50, 260, 250, 0xff6347, false);

	//	敵の名前、HP、行動を表示
	DrawFormatString(20, 30, 0xff6347, "%s",enemyStatusList.name[encounteredEnemy]);
	UpdateEnemyStatus();
	DrawEnemyAction();

	//	追加効果アイコン描画
	if (hasAddEffect == true && addEffectGraphIndex != -1)
		DrawExtendGraph(255 - EFFECT_GRAPH_SIZE, 55, 255, 55 + EFFECT_GRAPH_SIZE, effectOfEnemy_GraphHandlesList[addEffectGraphIndex], true);

}

void AttackOfEnemy()	//	敵の行動(攻撃)
{
	executedActionDamage = enemyStatusList.actionInfo[encounteredEnemy][actionNo].Damage;
	isPlayingAttackOfEnemy = true;	//	攻撃演出を再生フラグを立てる
}

void GuardOfEnemy()//	敵の行動(防御)
{
	//	防御値を更新
	guardValue =  enemyStatusList.actionInfo[encounteredEnemy][actionNo].Damage;	
}

void SpecialOfEnemy()//	敵の行動(特殊)
{

}

void ActionOfEnemy()	//	敵の行動
{
	//	倒したら表示しない
	if (enemyStatusList.currentHP[encounteredEnemy] == 0)
		return;

	if (isInAction == true)
	{
		//	現在の行動のtunsUntilActionを確認
		if (turnsUntilCurrentAction == 0)	//	行動時
		{
			//	アクションを実行			
			switch (enemyStatusList.actionInfo[encounteredEnemy][actionNo].actionType)
			{
			case ENEMY_ACTION_TYPE_ATTACK:
				AttackOfEnemy();
				break;
			case ENEMY_ACTION_TYPE_GUARD:
				GuardOfEnemy();
				break;
			case ENEMY_ACTION_TYPE_SPECIAL:
				SpecialOfEnemy();
				break;
			}

			executedActionName = enemyStatusList.actionInfo[encounteredEnemy][actionNo].actionName;

			if (addEffectGraphIndex == 1)
				addEffectGraphIndex = -1;
			
			//	前のログを削除して、新しいログを追加
			if(isLogCleared == false)
				logList.clear();

			std::string enemylog1 = "エネミー:";
			std::string enemylog2 = enemyStatusList.actionInfo[encounteredEnemy][actionNo].actionName;
			logList.push_back(enemylog1 + enemylog2);

			//	フラグを降ろす
			isInAction = false;
		}
		else//	まだ行動しない時
		{
			//	行動までのターンを減らす
			turnsUntilCurrentAction -= 1;
		}
	}
	
	//	行動を決定していない時
	if(isInAction == false)
	{
		actionNo = GetRand(2);
		//	現在の行動を保存して、行動中フラグを立てる
		turnsUntilCurrentAction = enemyStatusList.actionInfo[encounteredEnemy][actionNo].turnsUntilAction;

		isInAction = true;
	}


}

const char* GetEnemyActionName()	//	実行された敵の行動名取得
{
	return executedActionName;
}

int GetEnemyActionDamage()	//	実行された攻撃のダメージを返す
{
	return executedActionDamage;
}


//	敵の行動描画
void DrawEnemyAction()
{
	DrawFormatString(15, 320, 0xffffff, "NEXT\n%s\n%dターン後 : DMG(%d)", enemyStatusList.actionInfo[encounteredEnemy][actionNo].actionName,turnsUntilCurrentAction, enemyStatusList.actionInfo[encounteredEnemy][actionNo].Damage);
}