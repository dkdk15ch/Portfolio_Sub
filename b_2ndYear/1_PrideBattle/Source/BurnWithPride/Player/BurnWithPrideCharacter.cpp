// Copyright Epic Games, Inc. All Rights Reserved.

#include "BurnWithPrideCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "BurnWithPride.h"
#include "CombatDamageable.h"
#include "PridePlayerState.h"

ABurnWithPrideCharacter::ABurnWithPrideCharacter()
{
	// 衝突カプセルのサイズを設定します
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// コントローラーの回転ではキャラクターを回転させず、カメラだけに反映します。
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// キャラクターの移動を設定します
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// 注: 反復時間を短縮するため、これらの変数などはCharacter Blueprintで調整できます
	// 再コンパイルせずに調整できます
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// カメラブームを作成します（衝突時はプレイヤーに近づきます）
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// 追従カメラを作成します
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 注: Meshコンポーネント（Characterから継承）のスケルタルメッシュとアニメーションBlueprintの参照は 
	// ThirdPersonCharacterという派生Blueprintアセットで設定します（C++から直接Contentを参照しないため）
}

void ABurnWithPrideCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// アクションの割り当てを設定します
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// ジャンプ
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::Stopジャンプ);

		// 移動
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABurnWithPrideCharacter::MoveGravity);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ABurnWithPrideCharacter::Look);

		// 視点操作
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABurnWithPrideCharacter::Look);
	}
	else
	{
		UE_LOG(LogBurnWithPride, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABurnWithPrideCharacter::Move(const FInputActionValue& Value)
{
	// 入力はVector2Dです
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 入力を振り分けます
	DoMove(MovementVector.X, MovementVector.Y);
}

void ABurnWithPrideCharacter::MoveGravity(const FInputActionValue& value)
{
	MoveGravityBP(value);	//	������BP
}


void ABurnWithPrideCharacter::Look(const FInputActionValue& Value)
{
	// 入力はVector2Dです
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// 入力を振り分けます
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ABurnWithPrideCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// 前方の方向を取得します
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 前方ベクトルを取得します
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// 右方向ベクトルを取得します 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 移動入力を加えます 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ABurnWithPrideCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// コントローラーにヨーとピッチの入力を加えます
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ABurnWithPrideCharacter::DoJumpStart()
{
	// キャラクターにジャンプを指示します
	Jump();
}

void ABurnWithPrideCharacter::DoJumpEnd()
{
	// キャラクターにジャンプ停止を指示します
	Stopジャンプ();
}

void ABurnWithPrideCharacter::DealDamage(AActor* actor)
{
	if (!IsValid(actor)) return;

	//	damageable Actor���ǂ�������
	ICombatDamageable* Damageable = Cast<ICombatDamageable>(actor);

	if (Damageable)
	{
		const FVector Impulse = FVector(0, 0, 0);

		Damageable->ApplyDamage(MeleeDamage, this, actor->GetActorLocation(), Impulse);
	}
}

void ABurnWithPrideCharacter::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
	if (bIsinvincible) return;

	auto state = Cast<APridePlayerState>(GetPlayerState());
	state->DamageHP(Damage);
}
