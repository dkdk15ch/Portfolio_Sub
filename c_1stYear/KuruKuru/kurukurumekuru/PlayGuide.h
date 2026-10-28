#pragma once
#include <DxLib.h>

//	プレイガイド用
extern bool isViewingPlayGuide;	//	プレイガイド表示状態
extern bool prevFrameIsViewingPlayGuide;	//	前フレームのプレイガイド表示状態
extern int playGuideNo;	//	表示中のプレイガイドのメニューと各項目の番号
extern int prevFramePlayGuideNo;	//	前フレームのプレイガイドの表示番号

//	プロトタイプ宣言
void ViewPlayGuide(void);
