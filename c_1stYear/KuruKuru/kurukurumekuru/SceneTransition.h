#pragma once

enum class ScreenType
{
	Title,
	Battle,
	BookTurnOver,
	Story,
	StoryChoice,
	GameOver,
	GameClear
};

//画面関連変数
extern ScreenType screenCurrent;

//	プロトタイプ宣言
void UpdateScreen(ScreenType next);
