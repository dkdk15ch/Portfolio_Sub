#include "time.h"
#include "DxLib.h"

LONGLONG timePrevFrameTime = GetNowHiPerformanceCount();

float deltaTime = 0.0f;	//	デルタタイム
bool isInDilay = false;	//	ディレイ状態
float delayElapsed;	//	ディレイの経過時間


void InitializeDeltaTime()
{
	timePrevFrameTime = GetNowHiPerformanceCount();
	deltaTime = 0.0f;
}

void GetDeltaTime()
{
	deltaTime = (GetNowHiPerformanceCount() - timePrevFrameTime) / 1000000.0f;
	timePrevFrameTime = GetNowHiPerformanceCount();
}

bool WaitForDelay(float delayTime)
{
	if (isInDilay == true)
	{
		delayElapsed += deltaTime;	//	経過時間を加算
		if (delayElapsed > delayTime)
		{
			delayElapsed = 0.0f;	//初期化
			isInDilay = false;	//	ディレイフラグを降ろす
			return true;	
		}			
	}
	return false;
}