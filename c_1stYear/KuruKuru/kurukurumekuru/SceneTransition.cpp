#include "SceneTransition.h"


ScreenType screenCurrent = ScreenType::Title;

void UpdateScreen(ScreenType next)
{
	screenCurrent = next;
};

