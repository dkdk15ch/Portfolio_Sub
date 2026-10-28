#pragma once
#include "DxLib.h"

//	変数宣言
extern LONGLONG timePrevFrameTime;
extern float deltaTime;
extern bool isInDilay;

//	プロトタイプ宣言
void InitializeDeltaTime(void);
void GetDeltaTime(void);
bool WaitForDelay(float delayTime);
