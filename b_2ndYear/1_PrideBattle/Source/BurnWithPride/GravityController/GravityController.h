// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BurnWithPridePlayerController.h"
#include "GravityController.generated.h"

/**
 * 
 */
UCLASS()
class BURNWITHPRIDE_API AGravityController : public ABurnWithPridePlayerController
{
	GENERATED_BODY()
	
public:
	virtual void UpdateRotation(float DeltaTime)override;

	//	ワールド空間の回転を重力基準へ変換します
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityRelativeRotation(FRotator Rotation, FVector GravityDirection);

	//	重力基準の回転をワールド空間へ変換します
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityWorldRotation(FRotator Rotation, FVector GravityDirection);
private:
	FVector LastFrameGravity = FVector::ZeroVector;

};
