// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPPI_Interaction.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UCPPI_Interaction : public UInterface
{
	GENERATED_BODY()
};

class PAPERZD_3D2D_API ICPPI_Interaction
{
public:
	GENERATED_BODY()

public:
	virtual void Interact() = 0;
	virtual void FinishInteraction() = 0;
	virtual bool NextText() = 0;
};
