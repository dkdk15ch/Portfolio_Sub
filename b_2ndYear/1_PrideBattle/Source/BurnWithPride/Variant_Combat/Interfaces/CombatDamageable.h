// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatDamageable.generated.h"

/**
 *  CombatDamageable interface
 *  Provides functionality to handle damage, healing, knockback and death
 */
UINTERFACE(MinimalAPI, NotBlueprintable)
class UCombatDamageable : public UInterface
{
	GENERATED_BODY()
};

class ICombatDamageable
{
	GENERATED_BODY()

public:

	/** ダメージとノックバックのイベントを処理します */
	UFUNCTION(BlueprintCallable, Category="Damageable")
	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse) = 0;
};
