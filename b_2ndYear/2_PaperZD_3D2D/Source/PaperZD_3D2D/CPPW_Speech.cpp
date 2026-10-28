// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPW_Speech.h"
#include "Kismet/GameplayStatics.h"	//	World取得
#include "CPP_EnemyBase.h"
#include "CPP_Player.h"
#include "MyPlayerController.h"

void UCPPW_Speech::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)	return;
	
	APawn* Pawn = PlayerController->GetPawn();
	ACPP_Player* Player = Cast<ACPP_Player>(Pawn);
	if (!Player)	return;

	Player->OnChangeAnswer.AddDynamic(this, &UCPPW_Speech::ToggleAnswerEnabled);


	//	ワールド内のACPP_EnemyBaseを全取得
	TArray<AActor*> EnemyActorList;
	UGameplayStatics::GetAllActorsOfClass(World, ACPP_EnemyBase::StaticClass(), EnemyActorList);
	//	取得したすべてに、会話変更の通知バインド
	for (AActor* Actor : EnemyActorList)
	{
		ACPP_EnemyBase* Enemy = Cast<ACPP_EnemyBase>(Actor);
		if (!Enemy) continue;

		//Enemy->OnChangeDialogue.AddDynamic(this, &UCPPW_Speech::UpdateDialogueText);
		Enemy->OnChangeDialogue.AddDynamic(this, &UCPPW_Speech::UpdateDialogueText);
		UE_LOG(LogTemp, Warning, TEXT("CPPW_Speech.cpp: BindEnemy = %s"), *Enemy->MyName.ToString());
	}
}

void UCPPW_Speech::NativeDestruct()
{
	Super::NativeDestruct();

	UE_LOG(LogTemp, Warning, TEXT("UCPPW_Speech：　デリゲートのバインド解除"));

	//	デリゲートバインド解除
	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController)
	{
		APawn* Pawn = PlayerController->GetPawn();
		ACPP_Player* Player = Cast<ACPP_Player>(Pawn);
		if (Player)
		{
			Player->OnChangeAnswer.RemoveDynamic(this, &UCPPW_Speech::ToggleAnswerEnabled);
		}
	}

	TArray<AActor*> EnemyActorList;
	UGameplayStatics::GetAllActorsOfClass(World, ACPP_EnemyBase::StaticClass(), EnemyActorList);
	for (AActor* Actor : EnemyActorList)
	{
		ACPP_EnemyBase* Enemy = Cast<ACPP_EnemyBase>(Actor);
		if (!Enemy)	continue;
		Enemy->OnChangeDialogue.RemoveDynamic(this, &UCPPW_Speech::UpdateDialogueText);

	}
	
	//	ワールド切り替え時に参照が残るとクラッシュするので、nullptrに
	SetSpeekingActor(nullptr);
}

void UCPPW_Speech::ToggleAnswerEnabled_Implementation()
{
}

void UCPPW_Speech::UpdateDialogueText_Implementation(const FText& Text)
{
}

void UCPPW_Speech::StopUpdatePosition()
{
}
