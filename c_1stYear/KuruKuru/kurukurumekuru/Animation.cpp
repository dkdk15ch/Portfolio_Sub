#include <math.h>
#include "animation.h"
#include "book.h"
#include "time.h"
#include "fonts.h"
#include "Battle.h"
#include "ScreenInfomation.h"
#include "button_detection.h"
#include "magic.h"
#include "Enemy.h"
#include "player.h"
#include "Story.h"

//	変数初期化
Line pageLAnimation = { 29.5,360,272.5,360 };	//	捲っている紙左の線
Line pageRAnimation = { 272.5, 360,525,360 };	//	捲っている紙右の線
float pageLength = 252.5f;	//	ページ長
AnimaRect bookLeft = { 19.5,360,272.5,410 };	//	本の厚さ左の各座標
//Rect bookRight = { 277.5,360,530,360 };
AnimaRect bookRight = { 272.5,410,525,410 };	//	本の厚さ右の各座標

Line coverLeft = { 14.5,410,277.5,410 };	//	表紙左の線
//Line coverRight = { 277.5,360,530,360 };
Line coverRight = { 277.5,410,530,410 };	//	表紙右の線
AnimaRect stopButton = { 5, 10,530, 420 };	//	停止ボタンの各座標

//	捲りアニメーション用
static const double pi = 3.141592653589793;	//	π
float angle =0;	//角度
float pageAnimationTime = 0;	//	アニメーション時間
float amountOfmovement = 0.0f;	//	本の厚さの変化量

//	点滅用
float timeBlinkCountDown = -1.0f;	//	点滅用カウントダウン

//	ドラッグアニメーション用　三角形の各座標
int trianglePoint1X = 0;	
int trianglePoint1Y = 0;
int trianglePoint2X = 0;
int trianglePoint2Y = 0;
int trianglePoint3X = 0;
int trianglePoint3Y = 0;

bool isPlayingBattleAnima = false;//	戦闘用アニメーション再生状態

//	ヒット演出
float countOfHitEffect = 0.5f;	//	ヒット演出全体時間
float elapsedOfHitEffect = 0.0f;	//	経過時間
bool isHit = false;	//	ヒット状態
bool isFirstOfHit = true;	//	初回処理用
int startRight = 0;	//	初期位置左の画像の右位置(中心)
int graphSize = 0;	//	画像幅

//	攻撃演出用
bool isPlayingAttackOfPlayer = false;	//	攻撃演出再生状態(プレイヤー)
bool isFistOfAttackAnima = true;
int magicType = 0;	//	属性	
int magicLevel = 0;	//	階級
int centerX = START_POSX_OF_ATTACK;	//	中心座標x
int centerY = START_POSY_OF_ATTACK;	//	中心座標y
int cirecleR = CIRECLE_R;	//	半径
unsigned int color = 0xff6347;		//	橙色（火）
int angleOfAttackEffect = 0;	//	球内描画のの角度
double hypotenuse = sqrt(110.0 * 110.0 + 440.0 * 440.0);	//	斜辺
double moveAngle = atan(110.0 / 440.0);	//	角度
float elapsedOFAttackPlayer = 0.0f;	//	経過時間
float ratio = 0.0f;	//	経過時間の割合
float easedRatio = 0.0f;	//	イージング比率

//	攻撃演出（敵)
bool isPlayingAttackOfEnemy = false;	//	攻撃演出再生状態(敵)
double extRate = 0;	//	拡大率
float angleOfEnemy = 0;	//	回転用角度
bool isFirstOfAttackEnemy = false;	//	初回処理用
float countOfAttackEnemy = 0.0f;	//	攻撃演出（敵)の全体時間
float elapsedOfAttackEnemy = 0.0f;	//	攻撃演出（敵)の経過時間

int animationFonts[1];	//	アニメーション用フォント

//	ダメージ演出
int playCount = 0; //	再生回数
bool isPlayingDamageEffect = false;	//	再生状態
float timeDamageEffectCountDown = -1.0f;	//	点滅用カウントダウン

//	フェードアウト
float countOfFadeOut = 0.0f;		//	フェードアウト時間カウント
bool isPlayingFadeOut = false;	//	フェードアウト再生状態

void InitalizeAnimationFonts()
{
	animationFonts[0] = CreateFontToHandle(NULL, 32, -1);
}


//	捲りアニメーション
void pageAnimation()
{
	//	捲る紙の角度を変更して動かす
	pageLAnimation.startX = pageLAnimation.endX - pageLength * cos(pi * (angle / 180));	
	pageLAnimation.startY = pageLAnimation.endY - pageLength * sin(pi * (angle / 180));

	//	目押しアニメーション
	if (isMoving == true)
	{
		//	ページを捲る動き
		if (PAGE_ANIMATION_TIME > pageAnimationTime)
			pageAnimationTime += deltaTime;
		else
		{	//	アニメーションの切り替え時間を経過したら角度を変更する
			pageAnimationTime = 0;	//	切り替え時間初期化
			if (barDir == true)	//	捲る方向で回転方向を変更
			{
				if (angle < 180)
					angle += 20;	//	20度ずつ動かす
				else
					angle = 0;
			}
			else
			{
				if (angle > 0)
					angle -= 20;	//	20度ずつ動かす
				else
					angle = 180;
			}
		}

		bookLeft.top = 360 + 50 * timingTime / TIMING_TIME;	//	止めるタイミングの割合で本の厚さを上下させる(左)
		bookRight.top = 410 - 50 * timingTime / TIMING_TIME;//	止めるタイミングの割合で本の厚さを上下させる(右)
		pageLAnimation.endY = 360 + amountOfmovement;	//	捲る部分が本から離れないように上下させる

		if (barDir == true) // 捲る方向で上下の動きを変更
			timingTime += deltaTime;
		else
			timingTime -= deltaTime;

		if (timingTime < TIMING_TIME/2)	//	全体時間の半分になったら切り替える
			amountOfmovement = 50 * timingTime / TIMING_TIME;
		else
			amountOfmovement = 50 *(1 - timingTime / TIMING_TIME);

	}
	else//	動いていない時
	{
		angle = 0;
		amountOfmovement = 0;
	}


	//	描画
	//	本
	DrawBoxAA(bookLeft.left, bookLeft.top, bookLeft.right, bookLeft.bottom, 0xffffff, true);	//	本のメイン部分（左）
	DrawBoxAA(bookRight.left, bookRight.top, bookRight.right, bookRight.bottom, 0xffffff, true);	//	本のメイン部分（右）
	for(int i=0;bookLeft.top + i*5< bookLeft.bottom;i++)
		DrawLineAA(bookLeft.left+5, bookLeft.top + i * 5, bookLeft.right-5,bookLeft.top + i * 5, 0x777777, 1.0f);	//	ページの模様（左）
	for (int i = 0; bookRight.top + i * 5 < bookRight.bottom; i++)
		DrawLineAA(bookRight.left + 5, bookRight.top + i * 5, bookRight.right - 5, bookRight.top + i * 5, 0x777777, 1.0f);	//	ページの模様（右）

	DrawLineAA(coverLeft.startX, coverLeft.startY, coverLeft.endX, coverLeft.endY, 0xff0000,3.0f);	//	表紙（左）
	DrawLineAA(coverRight.startX, coverRight.startY, coverRight.endX, coverRight.endY, 0xff0000, 3.0f);	//	表紙（右）

	DrawLineAA(pageLAnimation.startX, pageLAnimation.startY, pageLAnimation.endX, pageLAnimation.endY, 0x777777, 3.0f);		//	ページを捲る部分
	//DrawLineAA(pageRAnimation.startX, pageRAnimation.startY, pageRAnimation.endX, pageRAnimation.endY, 0xEEEEEE, 3.0f);

	//	停止ボタン描画
	DrawBoxAA(stopButton.left,stopButton.top,stopButton.right,stopButton.bottom,0xffff00, false);
	DrawFormatStringFToHandle(stopButton.left + 20, stopButton.top + 20, 0xffff00, UIFonts[UI_FONT_INDEX_STOP], uiStrings[UI_FONT_INDEX_STOP]);
}

//	点滅（チュートリアル用）
void BlinkOfBox(int left,int top, int right, int bottom, int color)
{
	//	矩形を点滅させる
	if (timeBlinkCountDown > UI_DURATION_OF_HIDE)
	{	//	αを変更して描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBoxAA(left, top, right, bottom, color, true, 2.0f);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 252);
	}
	
	if (timeBlinkCountDown <= 0.0f)
		timeBlinkCountDown = UI_DURATION_OF_BLINK;	//	繰り返し点滅するように時間の再設定
	timeBlinkCountDown -= deltaTime;
}

//	ドラッグアニメーション
void DrugAnimation(int cursorx, int cursory,int startx , int starty, int animationType)
{
	//	アニメーション三角形の各頂点座標
	int trianglePoint1X = pageRight.right;
	int trianglePoint2X = pageRight.right + (cursorx - startx);
	int trianglePoint3X = trianglePoint1X;
	switch (animationType)//	NEXTかPageChangeで処理を切り替える
	{
		case ANI_TYPE_NEXT:
		{
			int trianglePoint1Y = pageRight.bottom;
			int trianglePoint2Y = trianglePoint1Y;
			int trianglePoint3Y = pageRight.bottom + ((cursorx - startx));
		
			if (pageRight.right + cursorx - startx < pageRight.left + 10)
			{//	アニメーション制限位置
				trianglePoint2X = pageRight.left + 10;
				trianglePoint3Y = pageRight.bottom - (pageRight.right - (pageRight.left + 10));
			}

			if (cursorx - startx < 0 && cursory - starty < 0 )	//	左上にドラッグしたらアニメーション描画
			{
				DrawTriangleAA(trianglePoint1X, trianglePoint1Y, trianglePoint2X, trianglePoint2Y, trianglePoint3X, trianglePoint3Y, 0xffff00, true);
				DrawTriangleAA(trianglePoint2X, trianglePoint3Y, trianglePoint2X, trianglePoint2Y, trianglePoint3X, trianglePoint3Y, 0xffff00, false);
			}

			if (cursorx - startx < -100 && cursory - starty < 0) //	NEXT描画
				DrawFormatStringToHandle(pageRight.right - 110, pageRight.bottom - 60, 0x000000, storyFonts[FONT_STORY_BIG_TEXT], "NEXT");
			else
				DrawFormatStringToHandle(pageRight.right - 110, pageRight.bottom - 60, 0xffff00, storyFonts[FONT_STORY_BIG_TEXT], "NEXT");
		}
		break;
		case ANI_TYPE_CHANGE:
		{
			int trianglePoint1Y = pageRight.top;
			int trianglePoint2Y = trianglePoint1Y;
			int trianglePoint3Y = pageRight.top - ((cursorx - startx));
		
			if (pageRight.right + cursorx - startx < pageRight.right - 90)
			{//	アニメーション制限位置
				trianglePoint2X = pageRight.right - 90;
				trianglePoint3Y = pageRight.top + 90;
			}

			if (cursorx - startx < 0 && cursory - starty > 0)	//	左下にドラッグしたらアニメーション描画
			{
				DrawTriangleAA(trianglePoint1X, trianglePoint1Y, trianglePoint2X, trianglePoint2Y, trianglePoint3X, trianglePoint3Y, 0xffff00, true);
				DrawTriangleAA(trianglePoint2X, trianglePoint3Y, trianglePoint2X, trianglePoint2Y, trianglePoint3X, trianglePoint3Y, 0x000000, true);
				DrawTriangleAA(trianglePoint2X, trianglePoint3Y, trianglePoint2X, trianglePoint2Y, trianglePoint3X, trianglePoint3Y, 0xffff00, false);
			}

			if (cursorx - startx < -100 && cursory - starty > 0) //	PageChange描画
			{
				//	PageChange
				DrawCircleAA(SCREEN_WIDTH - 135, 35, 15, 16, 0x000000, false, 8.0f);
			}
			else
			{
				//	PageChange
				DrawCircleAA(SCREEN_WIDTH - 135, 35, 15, 16, 0xffffff, false, 8.0f);
			}
		}
			break;
	}
}

//	ヒット演出
void HitEffect(int *left, int *right ,bool isEnemy)
{
	//if (WaitForDelay(0.3f) == false && isInDilay == true)
	//	return;

	if (isPlayingBattleAnima == false)
		isPlayingBattleAnima = true;	//	バトルアニメション開始

	if (isFirstOfHit == true)	//	初回処理
	{
		isFirstOfHit = false;	//	ヒット状態
		startRight = *right;	//	初期位置左の画像の右位置(中心)
		graphSize = *right - *left;	//	画像幅
		countOfHitEffect = COUNT_OF_HIT_EFFECT;	//	ヒット演出再生時間
		elapsedOfHitEffect = 0.0f;	//	ヒット演出経過時間
		if (addEffectGraphIndex != -1)
			hasAddEffect = true;
	}
		
	countOfHitEffect -= deltaTime;	//	再生時間を減らす
	
	if (countOfHitEffect < 0)	//	再生が終わったら抜ける
	{
		//初期位置に戻す
		*left = startRight - graphSize;	
		*right = startRight;
		//	状態初期化
		isHit = false;	
		isFirstOfHit = true;
		isInDilay = true;	//	ディレイフラグを立てる


		if (isEnemy == true)
		{
			DamageEnemyByAttack(selectedMagicNo);		//	攻撃のダメージ
			isPlayingFadeOut = true;
		}
		else
		{
			isOutOfMP = false;
			isPlayingFadeOut = true;

			//ResetUpdateTurnPam();
		}
		return;
	}

	int speed = 50;	//	揺れの速度調整用
	int amplitude = 10;	//	振幅

	elapsedOfHitEffect += deltaTime * speed;	//	経過時間
	float shakeOffset = amplitude * sin(elapsedOfHitEffect);	//	基準からの揺れ

	*right = startRight + (int)shakeOffset;	//	中心を基準に揺らす
	*left = *right - graphSize;	//	揺れを元に位置調整
}

void AttackAnimationOfPlayer()//	攻撃演出(プレイヤー)
{		
	if (isPlayingBattleAnima == false)
		isPlayingBattleAnima = true;

	if (isFistOfAttackAnima == true)	//	初回処理
	{
		isFistOfAttackAnima = false;
		//	描画	

		magicType = selectedMagicNo / 3;
		magicLevel = selectedMagicNo % 3;
	}

	elapsedOFAttackPlayer += deltaTime;	//	経過時間
	ratio = elapsedOFAttackPlayer / COUNT_OF_ATTACK_MOVETIME;	//	経過時間の割合

	//	再生が終わった抜ける
	if (ratio > 1.0)	
	{
		ratio = 0.0;	//	初期化
		elapsedOFAttackPlayer = 0.0f;	//	初期化
		isFistOfAttackAnima = true;	//	初期化
		isPlayingAttackOfPlayer = false;	//	再生状態切り替え	
		if (isHit == false)	//	ヒット状態切り替え
			isHit = true;
		isInDilay = true;	//	ディレイフラグを立てる
		return;
	}
	
	//	攻撃弾の移動
	easedRatio = 1.0 - (1.0 - ratio) * (1.0 - ratio);	//	2乗で減速させる
	centerX = START_POSX_OF_ATTACK - hypotenuse * easedRatio * cos(moveAngle);	//	弾の移動x
	centerY = START_POSY_OF_ATTACK + hypotenuse * easedRatio * sin(moveAngle);	//	弾の移動y

	switch (magicType)	//	属性によって色を変える
	{
		case 0:	//	火炎
		{
			color = 0xff6347;//	橙色（火）
		}
		break;
		case 1:	//	風
		{
			color = 0x90ee90;//	緑（風）
		}
		break;
		case 2:	//	水
		{
			color = 0x87cefa;//	水色（水）
		}
		break;
	}
	DrawCircleAA(centerX, centerY, cirecleR,32, color,true,5.0F);

	for (int i = 0; i < 6; i++)	//	円内の線
	{
		DrawLineAA(centerX, centerY, centerX + cirecleR * cos(pi * (angleOfAttackEffect / 180.0)), centerY + cirecleR * sin(pi * (angleOfAttackEffect / 180.0)), 0x000000, 1.0F);
		DrawLineAA(centerX, centerY, centerX - cirecleR * cos(pi * (angleOfAttackEffect / 180.0)), centerY - cirecleR * sin(pi * (angleOfAttackEffect / 180.0)), 0x000000, 1.0F);
		angleOfAttackEffect += 30;
	}
	

	switch (magicLevel)		//  階級ごとに描画追加
	{
	case 2: //  第三
		DrawCircleAA(centerX, centerY, 10, 32, 0x000000, false, 2.0F);
	case 1: //  第二
		DrawCircleAA(centerX, centerY, 15, 32, 0x000000, false, 2.0F);
	case 0: //  第一
		DrawCircleAA(centerX, centerY, 20, 32, 0x000000, false, 2.0F);
		break;
	}
}

void AttackAnimationOfEnemy()	//	攻撃演出(敵)
{
	if (isPlayingBattleAnima == false)
		isPlayingBattleAnima = true;//	バトルアニメション開始

	ChangeBrightOfEnemy();

	if (WaitForDelay(1.0f) == false && isInDilay == true)	//	再生までのディレイ
	{
		DrawExtendGraph(enemyGraphRect.left, enemyGraphRect.top, enemyGraphRect.right, enemyGraphRect.bottom, enemy_GraphHandleList[encounteredEnemy], true);	//	敵画像描画
		SetDrawBright(255, 255, 255);//	輝度を戻す
		return;
	}

	if (isFirstOfAttackEnemy == false)	//	初回処理
	{
		isFirstOfAttackEnemy = true;
		countOfAttackEnemy = COUNT_OF_HIT_EFFECT;
		elapsedOfAttackEnemy = 0.0f;
	}

	countOfAttackEnemy -= deltaTime;	//	再生時間を減らす

	if (countOfAttackEnemy < 0)	//	再生終了時、初期化、プレイヤーへのダメージ
	{
		//	初期化
		isFirstOfAttackEnemy = false;
		isPlayingAttackOfEnemy = false;
		isInDilay = true;	

		//	攻撃実行			
		int damage = GetEnemyActionDamage();
		if (damage > 0)
		{
			DamagePlayerByAttack(damage);
			//	ダメージ演出
			isPlayingDamageEffect = true;
		}
			
		return;
	}

	//	スケール取得
	extRate = GetAverageScale(enemy_GraphHandleList[encounteredEnemy]);
	//	回転角度を変更、sinで±15度で振る
	int speed = 25;	//	速度調整用
	elapsedOfAttackEnemy += deltaTime;
	angleOfEnemy = pi * 15* sin(elapsedOfAttackEnemy*speed) / 180;
	//	敵の描画
	DrawRotaGraph(enemyGraphRect.left + 100, enemyGraphRect.top+80, extRate, angleOfEnemy, enemy_GraphHandleList[encounteredEnemy], true);
	SetDrawBright(255, 255, 255);//	輝度を戻す

	//	敵の行動描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);//	半透明な背景
	DrawBoxAA(enemyGraphRect.left, enemyGraphRect.top + 50, enemyGraphRect.right + 50, enemyGraphRect.bottom - 50, 0xaaaaaa, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);//	αを戻す
	const char* enemyAction = GetEnemyActionName();	//	行動名
	int textWidtgh, textHegith, textLineCount;
	int centerX = (enemyGraphRect.left + enemyGraphRect.right + 50) / 2;
	int centerY = (enemyGraphRect.top + 50 + enemyGraphRect.bottom - 50) / 2;
	int textPosX, textPosY;
	TextPosCalculate(&textWidtgh, &textHegith, &textLineCount, animationFonts[0], enemyAction, centerX, centerY, &textPosX, &textPosY);
	DrawStringFToHandle(textPosX,textPosY, enemyAction, 0x000000, animationFonts[0]);	//	行動名描画
}

void ResetDamageEffectParm()//	ダメージ演出の初期化
{
	isPlayingDamageEffect = false;	//	三回行ったら終了
	playCount = 0;	//	カウント初期化
	timeDamageEffectCountDown = DAMAGAE_EFFECT_DURATION_OF_BLINK;	//	次回用再設定
}

void DamageEffect(int x1, int y1, int x2, int y2,int GHandle,int TransFlag)
{
	//	画像を点滅させる
	if (timeDamageEffectCountDown > DAMAGE_EFFECT_DURATION_OF_HIDE)
	{	
		SetDrawBright(255,150,150);
		DrawExtendGraph(x1, y1, x2, y2, GHandle, TransFlag);
		SetDrawBright(255,255,255);
	}

	if (timeDamageEffectCountDown <= 0.0f)
	{
		if(playCount < 3)	//	3回未満なら再点滅
			timeDamageEffectCountDown = DAMAGAE_EFFECT_DURATION_OF_BLINK;	//	点滅するように時間の再設定
		else
		{	
			ResetDamageEffectParm();//	初期化
		}
		playCount++;
	}
	timeDamageEffectCountDown -= deltaTime;
}

int FadeOut(int alpha)//	フェードアウト
{
	if (alpha == 0)	//	αが0なら抜ける
	{
		countOfFadeOut = 0.0f;	//	カウント初期化
		isPlayingFadeOut = false;	//	フェードアウト状態初期化
		return 0;
	}
	countOfFadeOut += deltaTime;	//	カウントを加算

	float ratio = countOfFadeOut / COUNT_OF_FADE_OUT;	//	カウントの割合で減らしていく

	alpha = 255 * (1.0f - ratio)*(1.0f - ratio);	//	αを変更
	if (alpha < 0)	//	αの下限
		alpha = 0;
	return alpha;
}

bool CheckIsPlayingAnima()	//	再生状態のチェック
{//	いずれかのアニメーションが再生中=>true、再生されていない=>false
	if (isHit == true ||
		isPlayingAttackOfPlayer == true ||
		isPlayingAttackOfEnemy == true ||
		isOutOfMP == true )
	{
		return true;
	}
	return false;
}


