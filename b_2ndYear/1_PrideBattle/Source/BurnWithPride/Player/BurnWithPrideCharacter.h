// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CombatDamageable.h"
#include "BurnWithPrideCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class ABurnWithPrideCharacter : public ACharacter ,public ICombatDamageable
{
	GENERATED_BODY()

	/** キャラクターの後方にカメラを配置するカメラブーム */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** 追従カメラ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** ジャンプ入力アクション */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** 移動入力アクション */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** 視点入力アクション */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse 視点入力アクション */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DealDamage(AActor* actor);

	UPROPERTY(EditAnywhere, Category = "Melee Attack|Damage", meta = (ClampMin = 0, ClampMax = 100))
	int MeleeDamage = 10;

public:
	/** コンストラクタ */
	ABurnWithPrideCharacter();	

protected:

	/** 入力アクションの割り当てを初期化します */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** 移動入力の処理に使用します */
	void Move(const FInputActionValue& Value);

	void MoveGravity(const FInputActionValue& value);

	/** 視点入力の処理に使用します */
	void Look(const FInputActionValue& Value);

public:

	/** 操作またはUIインターフェースからの移動入力を処理します */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** 操作またはUIインターフェースからの視点入力を処理します */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** 操作またはUIインターフェースからのジャンプ開始入力を処理します */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** 操作またはUIインターフェースからのジャンプ開始入力を処理します */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	UFUNCTION(BlueprintImplementableEvent, Category = "Input")
	void MoveGravityBP(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable, Category = "PrideGauge")
	void SetIsInvincible(bool newValue) { bIsinvincible = newValue; }

	bool bIsinvincible = false;

public:

	/** CameraBoomサブオブジェクトを返します **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** FollowCameraサブオブジェクトを返します **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)override;

};
