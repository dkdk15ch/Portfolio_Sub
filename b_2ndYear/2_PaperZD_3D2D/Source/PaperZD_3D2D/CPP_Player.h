// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Delegates/DelegateCombinations.h"
#include "InputActionValue.h"         
#include "CPPI_Interaction.h"							//	インタラクション用インターフェース

//	前方宣言
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UPaperZDAnimInstance;
class UBoxComponent;

#include "CPP_Player.generated.h"

//	Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerBeginPlay, UInputMappingContext*, MappingContext);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeAnswer);
DECLARE_DELEGATE(FOnChangeIMC);


/**
 * 
 */
UCLASS()
class PAPERZD_3D2D_API ACPP_Player : public APaperZDCharacter
{
	GENERATED_BODY()
	
public:
	ACPP_Player();

	virtual void BeginPlay() override;

	//	Delegateプロパティ
	UPROPERTY()
	FOnPlayerBeginPlay OnPlayerBeginPlay;	//	BeginPlayを通知
	UPROPERTY()
	FOnChangeAnswer OnChangeAnswer;
	FOnChangeIMC OnChangeIMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FVector2D PlayerDir;	//	アニメーション再生で移動方向を使用

	UInputMappingContext* GetMoveIMC() const { return MoveIMC; }
	UInputMappingContext* GetDialogueIMC() const { return DialogueIMC; }

protected:
	//	ControllerのPossess時に呼び出される初期設定
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//	Components
	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArm;	//	スプリングアーム
	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera;		//	カメラ
	UPROPERTY()
	TObjectPtr<UBoxComponent> BoxCollision;		//	ボックス
		
	//	移動関連
	UPROPERTY()
	UInputMappingContext* MoveIMC;		//	移動用IMC	
	UPROPERTY()
	UInputMappingContext* DialogueIMC;	//	会話用IMC

	UPROPERTY()
	UInputAction* MoveAction;				//	IA：移動用
	UFUNCTION()
	void OnMove(const FInputActionValue& value);	//	移動処理

	//	インタラクション関連
	UPROPERTY()
	UInputAction* Interaction;				//	IA:インタラクション
	void ExecuteInteraction();				//	実行関数
	bool bIsInteractionLoced = false;		//	ロックフラグ
	void ToggleInteractionLock();			//	ロック切替
	UPROPERTY()
	TScriptInterface<ICPPI_Interaction> InteractionTarget;	//	対象を保存
	UPROPERTY()
	UInputAction* DialogueAction;		//	IA :　会話用
	void SelectAnswer();	//	回答切替	


	UPROPERTY()
	UPaperZDAnimInstance* PlayerAnimInstance;
	
};
