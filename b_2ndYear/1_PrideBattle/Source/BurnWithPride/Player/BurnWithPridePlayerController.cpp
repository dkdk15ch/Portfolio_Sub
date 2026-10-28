// Copyright Epic Games, Inc. All Rights Reserved.


#include "BurnWithPridePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "BurnWithPride.h"
#include "Widgets/Input/SVirtualJoystick.h"

void ABurnWithPridePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ローカルプレイヤーコントローラーでのみタッチ操作を生成します
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// モバイル操作ウィジェットを生成します
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// 操作ウィジェットをプレイヤー画面に追加します
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogBurnWithPride, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void ABurnWithPridePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// ローカルプレイヤーコントローラーにのみIMCを追加します
	if (IsLocalPlayerController())
	{
		// 入力マッピングコンテキストを追加します
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// モバイルタッチ入力を使用していない場合のみIMCを追加します
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}
