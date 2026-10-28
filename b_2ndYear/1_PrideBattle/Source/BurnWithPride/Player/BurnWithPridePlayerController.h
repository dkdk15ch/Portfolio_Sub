// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BurnWithPridePlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 *  三人称ゲーム用の基本PlayerControllerクラス
 *  入力マッピングを管理します
 */
UCLASS(abstract)
class ABurnWithPridePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** 入力マッピングコンテキスト */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** 入力マッピングコンテキスト */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** 生成するモバイル操作ウィジェット */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** モバイル操作ウィジェットへの参照 */
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** ゲームプレイの初期化 */
	virtual void BeginPlay() override;

	/** 入力マッピングコンテキストの設定 */
	virtual void SetupInputComponent() override;

};
