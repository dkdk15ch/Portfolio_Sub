// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Player.h"
#include "UObject/ConstructorHelpers.h"					//	コンストラクタヘルパ
//	Components
#include "PaperFlipbook.h"								//	Flipbook
#include "PaperFlipbookComponent.h"						//	Flipbookコンポーネント
#include "PaperZDAnimationComponent.h"					//	PaperZDAnim
#include "PaperZDAnimInstance.h"						//	PaperZDAnimInstance
#include "Components/CapsuleComponent.h"				//	カプセル
#include "Components/BoxComponent.h"					//	box
#include "GameFramework/SpringArmComponent.h"			//	カメラアーム
#include "Camera/CameraComponent.h"						//	カメラ
//	操作
#include "InputMappingContext.h"						//	IMC
#include "EnhancedInputComponent.h"						//	EnhancedInput
#include "InputAction.h"								//	IA
#include "GameFramework/CharacterMovementComponent.h"	//	移動用コンポーネント
//	World取得
#include "Kismet/GameplayStatics.h"						//	GameModeの取得
#include "CPPGM_MyGameModeBase.h"						//	GameModeBaseの派生
//	UI
//#include "CPPW_HUD.h"									//	HUD Widget
//#include "CPPW_Speech.h"								//	吹き出し用Widget


ACPP_Player::ACPP_Player() {
	//	Sprite 初期設定
	UPaperFlipbookComponent* InitSprite = GetSprite();
	//	SourceFlipbook　設定
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookAsset(TEXT("/Game/Texture/15/15_F.15_F"));
	if (FlipbookAsset.Succeeded()) {
		InitSprite->SetFlipbook(FlipbookAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CPP_Player: Set Flipbook:Failure"));
	}
	//	Transform　設定
	InitSprite->SetRelativeLocation(FVector(0.0f, 0.0f, -120.0f));
	InitSprite->SetCastShadow(true);	//	影を表示
	InitSprite->SetUsingAbsoluteRotation(true); // 回転をワールド空間で
	InitSprite->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	//	Material　設定
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Paper2D/MaskedLitSpriteMaterial.MaskedLitSpriteMaterial"));
	if (MaterialAsset.Succeeded()) {
		InitSprite->SetMaterial(0,MaterialAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CPP_Player: Set Material:Failure"));
	}


	//	CapsuleComponent 初期設定
	GetCapsuleComponent()->SetCapsuleSize(80.0f, 120.0f);

	//	SpringArm　初期設定
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);	//	回転をワールド空間で
	SpringArm->SetWorldRotation(FRotator(-35.0f, -90.0f, 0.0f));	
	SpringArm->TargetArmLength = 1200.0f;	//	長さ調整
	//	角度継承設定
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bEnableCameraLag = true;		//	Camera Lag
	SpringArm->bDoCollisionTest = false;	//	Camera Collision
	
	// Camera 初期設定
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetFieldOfView(120.0f);	//	FOV設定
	Camera->SetConstraintAspectRatio(true);	//	アスペクト比を維持
	Camera->SetAspectRatio(1.777778f);	//	16:9
	//	Depth of Filed　設定
	Camera->PostProcessSettings.bOverride_DepthOfFieldFstop = true;
	Camera->PostProcessSettings.DepthOfFieldFstop = 3.6f;	//	F値（絞り）
	Camera->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true;
	Camera->PostProcessSettings.DepthOfFieldSensorWidth = 6000.0f;	//	センサー幅
	Camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	Camera->PostProcessSettings.DepthOfFieldFocalDistance = 1200.0f;	//	フォーカス距離

	//	BoxCollision 設定
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetBoxExtent(FVector(60.0f, 60.0f, 60.0f));
	BoxCollision->SetRelativeLocation(FVector(120.0f, 0.0f, 0.0f));
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));	//	CollisionPreset
	BoxCollision->SetHiddenInGame(false);	//	ゲーム注非表示

	//	IMC　設定 
	ConstructorHelpers::FObjectFinder<UInputMappingContext> MoveIMC_Asset(TEXT("/Game/Input/IMC_RPG.IMC_RPG"));
	if (MoveIMC_Asset.Succeeded()) {
		MoveIMC = MoveIMC_Asset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CPP_Player: Set MoveIMC:Failure"));
	}
	ConstructorHelpers::FObjectFinder<UInputMappingContext> DialogueIMC_Asset(TEXT("/Game/Input/IMC_Dialogue.IMC_Dialogue"));
	if (DialogueIMC_Asset.Succeeded())
	{
		DialogueIMC = DialogueIMC_Asset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CPP_Player: Set DialogueIMC:Failure"));
	}

	//	IA 設定
	ConstructorHelpers::FObjectFinder<UInputAction> IAMove(TEXT("/Game/Input/IA_Move.IA_Move"));
	if (IAMove.Succeeded())
	{
		MoveAction = IAMove.Object;			//	移動用
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CPP_Player: Set IAMove:Failure"));
	}

	ConstructorHelpers::FObjectFinder<UInputAction> IAInteract(TEXT("/Game/Input/IA_Interact.IA_Interact"));
	if (IAInteract.Succeeded())
	{
		Interaction = IAInteract.Object;	//	インタラクト用	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CPP_Player: Set IAInteract:Failure"));
	}

	ConstructorHelpers::FObjectFinder<UInputAction> IADialogue(TEXT("/Game/Input/IA_Dialogue.IA_Dialogue"));
	if (IADialogue.Succeeded())
	{
		DialogueAction = IADialogue.Object;	//	会話用
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CPP_Player: Set IADialogue:Failure"));
	}


	//	Animation　設定
	ConstructorHelpers::FClassFinder<UPaperZDAnimInstance> AnimaAsset(TEXT("/Game/Animation/ABP_Player"));
	if (AnimaAsset.Succeeded())
	{
		GetAnimationComponent()->SetAnimInstanceClass(AnimaAsset.Class);
	}
	
	//	CharacterMovement 初期設定
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360000.0f, 0.0f);

	//	Pawn 初期設定
	bUseControllerRotationYaw = false;
	
}

void ACPP_Player::BeginPlay() 
{
	Super::BeginPlay();
	
	//	MyPlayerControllerへ通知を送信
	//	EnhancedInputの登録
	OnPlayerBeginPlay.Broadcast(MoveIMC);	
	
	//	AnimInstaneのセット(BeginPlay後にAnimInstanceが有効)
	PlayerAnimInstance = GetAnimationComponent()->GetAnimInstance();
}

//	入力のセットアップ（自動で呼ばれる）
void ACPP_Player::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(playerInputComponent);
	if (EnhancedInput)
	{
		//	IA:MoveActionにバインド
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_Player::OnMove);
		//	IA:Interactionにバインド
		EnhancedInput->BindAction(Interaction, ETriggerEvent::Started, this, &ACPP_Player::ExecuteInteraction);
		//	IA:DialogueActionにバインド
		EnhancedInput->BindAction(DialogueAction, ETriggerEvent::Started, this, &ACPP_Player::SelectAnswer);
	}
}
//	移動処理
void ACPP_Player::OnMove(const FInputActionValue& value)
{
	if (bIsInteractionLoced) return;	//	インタラクト中はロック

	//	FVector2Dとして取得
	FVector2D MoveVector = value.Get<FVector2D>();
	//	移動処理
	AddMovementInput(FVector(0.0f,-1.0f,0.0f), MoveVector.Y);
	AddMovementInput(FVector(1.0f,0.0f,0.0f), MoveVector.X);
	PlayerDir = MoveVector;	//	移動方向(アニメーション再生で使用)

}

//	会話中の回答選択
//	AD押すともう片方に切り替え（今のところ２択なので）
void ACPP_Player::SelectAnswer()
{
	UE_LOG(LogTemp, Warning, TEXT("会話の選択肢変更"));

	// YesNoのEnable切替
	OnChangeAnswer.Broadcast();
}


//	インタラクション用ロック
void ACPP_Player::ToggleInteractionLock()
{
	bIsInteractionLoced = !bIsInteractionLoced;
	if (bIsInteractionLoced)
	{	//	ロック中は一応CharacterMovementも止める
		GetCharacterMovement()->StopMovementImmediately();
	}

	OnChangeIMC.ExecuteIfBound();
}

//	判定内のインタラクション実行
void ACPP_Player::ExecuteInteraction() {
	//	インタラクション用ロックで処理を切り替え
	//	未ロック => インタラクション開始
	//	ロック中 => インタラクション終了（テキストが続く場合継続)
	if (!bIsInteractionLoced) {
		TArray<AActor*> OverlapedActors;
		BoxCollision->GetOverlappingActors(OverlapedActors);
		for (AActor* actor : OverlapedActors) 
		{
			if (actor == this) {
				continue;	//	自身に対しては何もしない
			}

			//	IInteractionを持つか確認してから実行
			if (actor->GetClass()->ImplementsInterface(UCPPI_Interaction::StaticClass()))
			{
				InteractionTarget = TScriptInterface<ICPPI_Interaction>(actor);
				if (InteractionTarget)
				{
					InteractionTarget->Interact();	
					ToggleInteractionLock();			
					break;	//	最初のだけ実行して終了
				}
			}
		}
	}
	else
	{
		if (InteractionTarget->NextText()) return;

		InteractionTarget->FinishInteraction();
		ToggleInteractionLock();
	}

}



