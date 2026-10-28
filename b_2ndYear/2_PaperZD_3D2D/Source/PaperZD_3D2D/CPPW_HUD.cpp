// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPW_HUD.h"		
#include "Components/CanvasPanel.h"				//	CanvasPanel
#include "Blueprint/UserWidget.h"				//	UseWidget
#include "Components/SizeBox.h"					//	SizeBox
#include "Blueprint/WidgetLayoutLibrary.h"

#include "CPPW_Speech.h"
#include "Components/CanvasPanelSlot.h"			//	CanvalSlot
#include "GameFramework/PlayerController.h"		//	playerContorller
#include "Blueprint/WidgetTree.h"

//	Viewportに追加タイミングで初期化	
//	GetWorld()が失敗するので
void UCPPW_HUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (!Speech)	return;
	Speech->OnStopTick.AddDynamic(this, &UCPPW_HUD::DisableTick);

	//	うまくいかなかったので保留
	// 　BPで追加
	////	WBPのロード
	//UClass* SpeechWBP = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/WBP_Speech.WBP_Speech_C"));
	//if (!SpeechWBP) {
	//	UE_LOG(LogTemp, Warning, TEXT("!SpeechWBP"));
	//	return;
	//}
	//
	//
	//SpeechWidget = CreateWidget<UUserWidget>(GetWorld(), SpeechWBP);
	//CanvasComponent = NewObject<UCanvasPanel>(this, UCanvasPanel::StaticClass());
	//SpeechWidget->SetVisibility(ESlateVisibility::Visible);
	//
	//if (!CanvasComponent)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("!CanvasComponent"));
	//	return;
	//}
	//
	//CanvasComponent->AddChild(SpeechWidget);
	//WidgetTree->RootWidget = CanvasComponent;
	//
	//UE_LOG(LogTemp, Warning, TEXT("SpeechWBP: %p"), SpeechWBP);
	//UE_LOG(LogTemp, Warning, TEXT("SpeechWidget: %p"), SpeechWidget.Get());
}

//	有効時のみ、吹き出し位置を更新
void UCPPW_HUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!bEnableTick) return;

	SetPositionBubble();
}

//	吹き出し位置更新
void UCPPW_HUD::SetPositionBubble()
{
	if (!Speech) 
	{
		UE_LOG(LogTemp, Error, TEXT("!Speech"));
		return;
	}
	if (!Speech->GetSpeekingActor())
	{
		UE_LOG(LogTemp, Error, TEXT("!SpeekingActor"));
		return;
	}

	if (!IsValid(Speech)) {
		UE_LOG(LogTemp, Error, TEXT("IsValid False"));
		
		return;
	}

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Speech->Slot);
	if (!CanvasSlot)
	{
		UE_LOG(LogTemp, Error, TEXT("!CanvasSlot"));
		return;
	}
	
	APlayerController* PlayerController = GetOwningPlayer();	
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("!PlayerController"));
		return;
	}
	
	FVector ActorLocation = Speech->GetSpeekingActor()->GetActorLocation();	//	会話者の位置を取得
	//	表示用の画面座標を取得
	FVector2D ScreenPosition;	
	PlayerController->ProjectWorldLocationToScreen(ActorLocation, ScreenPosition, true);	
	//	DPIスケール補正
	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
	ScreenPosition /= ViewportScale;	
	//	ウィンドウサイズ・HUDのサイズを取得
	int32 WindowSizeX, WindowSizeY;
	PlayerController->GetViewportSize(WindowSizeX, WindowSizeY);	
	//	吹き出しサイズ補正
	Speech->ForceLayoutPrepass();	
	FVector2D SpeechSize = Speech->GetDesiredSize();	
	ScreenPosition.X -= SpeechSize.X / 2.0f;
	ScreenPosition.Y -= SpeechSize.Y + 70.0f;

	CanvasSlot->SetPosition(ScreenPosition);

}

