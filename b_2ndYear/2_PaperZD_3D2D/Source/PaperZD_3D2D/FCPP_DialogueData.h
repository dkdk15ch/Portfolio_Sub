// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FCPP_DialogueData.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct PAPERZD_3D2D_API FCPP_DialogueData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DialogueText = TEXT(" ");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NextRow;
};
