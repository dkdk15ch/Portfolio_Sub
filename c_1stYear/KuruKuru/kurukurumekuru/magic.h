#pragma once
#include "DxLib.h"

#define NEED_MP_FIRST 10	//	第一呪文の消費MP
#define NEED_MP_SECOND 30	//	第二呪文の消費MP
#define NEED_MP_THIRD 50	//	第三呪文の消費MP
#define FONTS_NUM 4//	fontsの要素数
#define FONT_INDEX_MAGIC_NAME 0	//	呪文名のフォント
#define FONT_INDEX_MAGIC_SPEL 1//	詠唱文のフォント
#define FONT_INDEX_DMG_MP 2 //	MPのフォント
#define FONT_INDEX_PAGE_NO 3 //	ページ数のフォント
#define MAX_DOT_SECOND 15	//	第二呪文の継続ダメージ
#define MAX_DOT_THIRD 35	//	第三呪文の継続ダメージ
#define INDEX_OF_FIRE_EFFECT_GRAPH 0	//	追加効果描画用(火）
#define INDEX_OF_WATER_EFFECT_GRAPH 1	//	追加効果描画用(水）



//	魔法用構造体
struct Magic{
	int no;	//	呪文No
	char name[22];	//	呪文名
	char spell[111];	//	詠唱文
	int damage;	//	ダメージ
	int mp;	//	消費MP
	bool isComplete;	//	コンプリート状態
};

//	共有宣言
extern Magic magic[9];	//	原文の詠唱文
extern Magic memorizedMagic[9];	//	記憶している詠唱文
extern int pageList[256];	//	ページ内容の決定用の呪文No
extern int fonts[FONTS_NUM];//	フォントハンドル用
extern int countOfFoundChar;//	詠唱文比較用
extern int DoTDamage;//	追加効果用
extern int selectedMagicNo;	//	選択された呪文No

//プロトタイプ宣言
void InitializeFonts(void);	//	フォントハンドル初期化
void ResetMemory(void);	//	記憶してる詠唱文の初期化
int ContrastSpell(int);	//	原文と詠唱文比較
void CheckComplete(int);		//	詠唱文がコンプリートしたかチェック
int GetMagicDamage(int magicNo);
void AdditionalEffect(int index);	//	追加効果
void DoT(void);	//	継続ダメージ
void DrawMagicIcon(int posX, int posY, int index);	//	アイコン描画





