#include "DxLib.h"
#include "mapList.h"
#include "Player.h"
#include "InputManager.h"
#include "imgHandle.h"
#include "color.h"
#include "rootManager.h"
#include "mapInfo.h"
#include "IRoot.h"
#include "box.h"

#include <Windows.h>
#include <string>

#include "Test.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    


    // ウィンドウモード無効（＝フルスクリーン）
    ChangeWindowMode(TRUE);

    
    SetGraphMode(static_cast<int>(screenWidth), static_cast<int>(screenHeight), 32);

    // DxLib 初期化
    if (DxLib_Init() == -1) return -1;

	// マップ読み込み
    RoadBlockImage();
	MapManager mapmgr;
    mapmgr.LoadMapCSV("roots_MapList.csv");
    RoadTreeImage(mapmgr);
	//// 画面を読み込み
	//ImageHandles imgHandles;
	//std::string sand = "image/blockSand.jpg";
	//imgHandles.Load(ImgName::BLOCK_SAND, sand);
	//std::string nutrient = "image/blockNutrient.jpg";
	//imgHandles.Load(ImgName::BLOCK_NUTRIENT, nutrient);
	//std::string water = "image/blockWater.jpg";
	//imgHandles.Load(ImgName::BLOCK_WATER, water);
	//std::string oxygen = "image/blockOxygen.jpg";
	//imgHandles.Load(ImgName::BLOCK_OXYGEN, oxygen);


    Player player(0,0);

    RootManager rootmgr;
    bool RootFlag = false;

    int imgH = LoadGraph("image/blockSand.jpg");

    // 裏画面に描画
    SetDrawScreen(DX_SCREEN_BACK);

    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        //  マウス取得
        MouseInput::Update();
        
 		//  全マップの実行
        mapmgr.RunMapAll(player);

		//　根仮
        if (!RootFlag) {
            RootFlag = true;
			Test::Test_Root(mapmgr, rootmgr, 7, 22);    //  開始位置
        }

        ////////////////////////////
        //Test_PlayerCreateRootのテスト
		//複数回createしないように注意する。=> 対応
        //1回目は成功。2回目で伸ばそうとするとassertになる 
        ///////////////////////////////
        rootmgr.RunAllRoots(player);

        if(MouseInput::IsLeftTriggerd()) {
            if(player.GetIsSelecting())
			    Test::Test_PlayerCreateRoot(player, mapmgr, rootmgr);
        }
        


        //  プレイヤー
        player.Run();

        if (MouseInput::IsRightPressed()) {
			player.SetIsSelecting(false);
        }
		//DrawGraph(offsetX -40, offsetY, imgH, false);

		bool isSelecting = player.GetIsSelecting();
		std::string selectState = isSelecting ? "true" : "false";
        std::string test = "選択状態 : ";
		Test::Test_Print(test+selectState);

        MouseInput::SetPrevInput();

        ScreenFlip();

    }

    DxLib_End();
    return 0;
}