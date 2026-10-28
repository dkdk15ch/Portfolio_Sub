// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatDamageable.h"
#include "NPCBase.generated.h"

UCLASS()
class BURNWITHPRIDE_API ANPCBase : public ACharacter, public ICombatDamageable
{
	GENERATED_BODY()

public:
	// このキャラクターのプロパティに初期値を設定します
	ANPCBase();

protected:
	// ゲーム開始時または生成時に呼び出されます
	virtual void BeginPlay() override;

public:	
	// 毎フレーム呼び出されます
	virtual void Tick(float DeltaTime) override;

	// 入力に機能を割り当てるために呼び出されます
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "NPC|HP")
	void DamageHP(float damgae);
};
