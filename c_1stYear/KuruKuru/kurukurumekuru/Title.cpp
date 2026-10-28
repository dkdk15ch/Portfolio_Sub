#include "Title.h"
#include "button_detection.h"

int Titlefonts[2];

const char TitleStrings[][36] =
{
	"くるくるめくる",
	"START"
};

Point uiTitleLogoTextPosition;
Point uiTitleButtonTextPosition;
Rect uiTitleButtonFrameRect;
Rect TitleGraphRect = { 230.0f, 400.0f, 200.0f, 400.0f };

float timeTextBlinkCountDown = 1.5f;
int TitleGraph;

void LoadGraphTitle()	//	画像のロード
{
	TitleGraph = LoadGraph("Title.png", true);	//	最初のイベント画像
}


void CreateTitleFonts() {
	Titlefonts[FONT_INDEX_TITLE_LOGO] = CreateFontToHandle(NULL, 64, -1);	//	タイトルロゴ用フォント
	Titlefonts[FONT_INDEX_TITLE_BUTTON] = CreateFontToHandle(NULL, 32, -1);	//	タイトル画面のボタン用フォント


	TextPosCalculate(&textWidth, &textHeight, &textLineCount, Titlefonts[FONT_INDEX_TITLE_LOGO], TitleStrings[FONT_INDEX_TITLE_LOGO], SCREEN_HORI_CENTER, SCREEN_LOGO_POSY - 50, &uiTitleLogoTextPosition.posX, &uiTitleLogoTextPosition.posY);

	ButtonPosCalculate(&textWidth, &textHeight, &textLineCount,
		Titlefonts[FONT_INDEX_TITLE_BUTTON], 
		TitleStrings[FONT_INDEX_TITLE_BUTTON],
		SCREEN_HORI_CENTER, 
		SCREEN_VERT_CENTER - 80, 
		&uiTitleButtonTextPosition.posX, 
		&uiTitleButtonTextPosition.posY, 
		&uiTitleButtonFrameRect.rectleft, 
		&uiTitleButtonFrameRect.recttop, 
		&uiTitleButtonFrameRect.rectright, 
		&uiTitleButtonFrameRect.rectbottom);

}

bool DrawTitle(float deltaTime) {
	//	タイトルの描画
	DrawFormatStringToHandle(uiTitleLogoTextPosition.posX, uiTitleLogoTextPosition.posY, COLOR_WHITE, Titlefonts[FONT_INDEX_TITLE_LOGO], TitleStrings[FONT_INDEX_TITLE_LOGO]);
	{
		unsigned int textColor = COLOR_GRAY;	//	ボタンの色。初期値は灰色

		//	ボタンとの当たり判定
		if (true == DetectButtonCollision(uiTitleButtonFrameRect.rectleft, uiTitleButtonFrameRect.recttop, uiTitleButtonFrameRect.rectright, uiTitleButtonFrameRect.rectbottom, &textColor, COLOR_WHITE))
		{
			//screenCurrent = ScreenType::LevelCelect;
			timeTextBlinkCountDown = -1.0f;
			return true;
		}

		//	テキスト

		//	ボタンのテキストの描画
		//	テキストは1.5秒間の内、1秒描画し、0.5秒は描画しない
		if (timeTextBlinkCountDown >= UI_DURATION_OF_HIDE)
		{
			DrawFormatStringFToHandle(uiTitleButtonTextPosition.posX, uiTitleButtonTextPosition.posY, textColor, Titlefonts[FONT_INDEX_TITLE_BUTTON], TitleStrings[FONT_INDEX_TITLE_BUTTON]);
		}
		if (timeTextBlinkCountDown < 0.0f)
		{
			timeTextBlinkCountDown = UI_DURATION_OF_BLINK;	//	繰り返し点滅するように時間の再設定
		}
		
		timeTextBlinkCountDown -= deltaTime;

		//	ボタンのフレーム(枠)の描画
		DrawBoxAA(uiTitleButtonFrameRect.rectleft, uiTitleButtonFrameRect.recttop, uiTitleButtonFrameRect.rectright, uiTitleButtonFrameRect.rectbottom, textColor, false, 2.0f);

		DrawBoxAA(TitleGraphRect.rectleft, TitleGraphRect.recttop, TitleGraphRect.rectright, TitleGraphRect.rectbottom, 0xffff00, true);	//	開いてるページ（右）

		SetDrawBright(0, 255, 255);
		DrawExtendGraph(TitleGraphRect.rectleft, TitleGraphRect.recttop , TitleGraphRect.rectright, TitleGraphRect.rectbottom, TitleGraph, true);
		SetDrawBright(255, 255, 255);
	}
		
	return false;
}