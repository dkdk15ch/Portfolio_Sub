// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "CPP_Player.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "InputMappingContext.h"

void AMyPlayerController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	//	Eventをバインド
	auto* MyPawn = Cast<ACPP_Player>(InPawn);
	if (MyPawn) 
	{
		MyPawn->OnPlayerBeginPlay.AddDynamic(this, &AMyPlayerController::SetInhancedInput);
		MyPawn->OnChangeIMC.BindUObject(this, &AMyPlayerController::ToggleMoveToDialgue);
		//	IAじゃなくてIMC
		this->MoveIMC	= MyPawn->GetMoveIMC();
		this->DialogueIMC	= MyPawn->GetDialogueIMC();
	}

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (LocalPlayer)
	{
		InputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	}
}

void AMyPlayerController::SetInhancedInput(UInputMappingContext* MappingContext) 
{
	//ULocalPlayer* LocalPlayer = GetLocalPlayer();
	//if (LocalPlayer)
	//{
		//UEnhancedInputLocalPlayerSubsystem* InputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		if (InputSubSystem)
		{
			InputSubSystem->AddMappingContext(MappingContext, 0);
		}

	//}
}

void AMyPlayerController::ToggleMoveToDialgue()
{
	bOnDialogue = !bOnDialogue;

	//	会話用IMCと通常(移動用)のIMCを切り替える
	if (bOnDialogue) 
	{
		InputSubSystem->AddMappingContext(DialogueIMC,0);			
		InputSubSystem->RemoveMappingContext(MoveIMC);	
		UE_LOG(LogTemp, Warning, TEXT("IMC = Dialogue"));
	}
	else
	{
		InputSubSystem->AddMappingContext(MoveIMC, 0);
		InputSubSystem->RemoveMappingContext(DialogueIMC);
		UE_LOG(LogTemp, Warning, TEXT("IMC = Moving"));
	}
}
