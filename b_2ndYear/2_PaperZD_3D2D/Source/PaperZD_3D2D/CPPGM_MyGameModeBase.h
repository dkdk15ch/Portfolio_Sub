// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

class UCPPW_HUD;
class UUserWidget;

#include "CPPGM_MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PAPERZD_3D2D_API ACPPGM_MyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void SetHUD_Widget(UCPPW_HUD* widget) { HUD_Widget = widget; }

	UCPPW_HUD* GetHUD_Widget()const { return HUD_Widget; }
private:
	UPROPERTY()
	TObjectPtr<UCPPW_HUD> HUD_Widget;

};

