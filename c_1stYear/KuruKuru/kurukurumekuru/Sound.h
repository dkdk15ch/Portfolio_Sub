#pragma once
#include <DxLib.h>

#define SOUND_NUM 6
#define SOUND_SHOT 1
#define SOUND_FIRE 2
#define SOUND_WATER 3
#define SOUND_WIND 4
#define SOUND_HIT 5
#define SOUND_OUT_OF_MP 6


extern int soundHandles[SOUND_NUM];

void LoadSound();	//	サウンドロード