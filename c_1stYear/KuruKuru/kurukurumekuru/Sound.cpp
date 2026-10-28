#include "Sound.h"

int soundHandles[SOUND_NUM];

void LoadSound()//	サウンドロード
{
	soundHandles[SOUND_SHOT] =	LoadSoundMem("sound_shot.mp3");
	soundHandles[SOUND_FIRE] = LoadSoundMem("sound_fire.mp3");
	soundHandles[SOUND_WATER] = LoadSoundMem("sound_water.mp3");
	soundHandles[SOUND_WIND] = LoadSoundMem("sound_wind.mp3");
	soundHandles[SOUND_HIT] = LoadSoundMem("sound_hit.mp3");
	soundHandles[SOUND_OUT_OF_MP] = LoadSoundMem("sound_outOfMP.mp3");
}