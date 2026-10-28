#include "fonts.h"

//	配列の宣言
int UIFonts[1];

const char uiStrings[1][165] =
{
    "STOP"
};

void InitalizeUIFonts()
{
    UIFonts[UI_FONT_INDEX_STOP] = CreateFontToHandle(NULL, 64, -1);
}

//	テキスト位置計算用関数
void TextPosCalculate(int* textWidth, int* textHeight, int* textLineCount, int fonts, const char uistring[], int centerPosX, int centerPosY, int* textPosX, int* textPosY)
{
	GetDrawFormatStringSizeToHandle(&*textWidth, &*textHeight, &*textLineCount, fonts, uistring);
	*textPosX = centerPosX - (*textWidth / 2.0f);
	*textPosY = centerPosY - (*textHeight / 2.0f);
}
