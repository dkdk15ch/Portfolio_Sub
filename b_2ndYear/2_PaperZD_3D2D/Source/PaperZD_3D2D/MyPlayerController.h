// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;

#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PAPERZD_3D2D_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UFUNCTION()
	void SetInhancedInput(UInputMappingContext* MappingContext);

	void ToggleMoveToDialgue();

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* InputSubSystem;

	bool bOnDialogue = false;

	UPROPERTY()
	UInputMappingContext* MoveIMC;
	UPROPERTY()
	UInputMappingContext* DialogueIMC;
};
