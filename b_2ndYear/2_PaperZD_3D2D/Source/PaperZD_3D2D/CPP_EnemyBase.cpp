// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_EnemyBase.h"
#include "CPP_Player.h"						//	Player
//	Components
#include "PaperZDAnimationComponent.h"		//	ZDAnimComponent
#include "PaperZDAnimInstance.h"			//	ZDAnimInstance
#include "PaperFlipbook.h"					//	Flopbool
#include "PaperFlipbookComponent.h"			//	FlopbookComponent
#include "Components/CapsuleComponent.h"	//	CapsuleCompoent
//	Debug
#include "UObject/ConstructorHelpers.h"		//	コンストラクタヘルパ
#include "Kismet/KismetSystemLibrary.h"		//	PrintString
//	GameMode取得用
#include "Kismet/GameplayStatics.h"			//	GameModeの取得
#include "CPPGM_MyGameModeBase.h"			//	GameModeBaseの派生
//	UI
#include "Components/WidgetComponent.h"		//	Widget
#include "CPPW_HUD.h"						//	HUD Widget
#include "CPPW_Speech.h"					//	吹き出し用Widget
//	DataTable
#include "FCPP_DialogueData.h"				//	DialogueText Structure
#include "Engine/DataTable.h"				//	DataTable
//	非同期ロード
#include "Engine/StreamableManager.h"		
#include "Engine/AssetManager.h"

ACPP_EnemyBase::ACPP_EnemyBase() {
	//	Sprite 設定
	UPaperFlipbookComponent* InitSprite = GetSprite();
	//	SourceFlipbook	設定
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookAssets(TEXT("/Game/Texture/145/hiyoco_F.hiyoco_F"));
	if (FlipbookAssets.Succeeded()) {
		InitSprite->SetFlipbook(FlipbookAssets.Object);
	}
	//	Transform　設定
	InitSprite->SetRelativeLocation(FVector(0.0f, 0.0f, -120.0f));
	InitSprite->SetCastShadow(true);	//	影を表示
	InitSprite->SetUsingAbsoluteRotation(true);	//	回転をワールド空間で
	InitSprite->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	InitSprite->SetWorldScale3D(FVector(1.5f, 1.5, 1.5f));
	//	Matrial 設定
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Paper2D/MaskedLitSpriteMaterial.MaskedLitSpriteMaterial"));
	if (MaterialAsset.Succeeded())
	{
		InitSprite->SetMaterial(0, MaterialAsset.Object);
	}
	//	CapsuleComponent　設定
	GetCapsuleComponent()->SetCapsuleSize(80.0f, 120.0f);
		
	//	WidgetComponent　設定
	DialogueWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DialogueWidget"));
	DialogueWidget->SetupAttachment(RootComponent);
	//	会話アイコン用ウィジット
	ConstructorHelpers::FClassFinder<UUserWidget> DialogueIconClass(TEXT("/Game/UI/Dialogue/WBP_Dialogue.WBP_Dialogue_C"));
	if (DialogueIconClass.Succeeded())
	{
		DialogueWidget->SetWidgetClass(DialogueIconClass.Class);		
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ACPP_EnemeyBase: SetDialogueWidget:Failure"));
	}
	DialogueWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 140.0f));
	DialogueWidget->SetUsingAbsoluteRotation(true);
	DialogueWidget->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	DialogueWidget->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));
	DialogueWidget->SetCastShadow(false);
}

void ACPP_EnemyBase::BeginPlay()
{
	Super::BeginPlay();

	FirstRowName = NameOfCurrentRow;	//	最初の会話行名を保存

	//	HUDウィジットをキャッシュ
	UWorld* World = GetWorld();
	ACPPGM_MyGameModeBase* MyGameMode = Cast<ACPPGM_MyGameModeBase>(UGameplayStatics::GetGameMode(World));
	if (!MyGameMode) {
		UE_LOG(LogTemp, Error, TEXT("CPP_EnemyBase: !MyGameMode"));

		return;
	}
	CachedHUD = MyGameMode->GetHUD_Widget();;
	if (!CachedHUD) {
		UE_LOG(LogTemp, Warning, TEXT("CPP_EnemyBase: !HUD"));
		return;
	}

}

//	インタラクト処理
void ACPP_EnemyBase::Interact() 
{
	if (CachedHUD->Speech->GetbIsFadingOut())	return;	//	FadeOut中は無効

	UpdateDialogue();	// 会話内容を表示

	ToggleDialogueVisibility();	//	会話ウィンドウのVisibility切替
}
void ACPP_EnemyBase::FinishInteraction()
{
	ToggleDialogueVisibility();	//	会話ウィンドウのVisibility切替

	//	会話内容を最初に戻す
	NameOfCurrentRow = FirstRowName;
	UpdateDialogue();

	//	HUDをフェードアウト
	CachedHUD->Speech->FadeOutAll();

	//	YESを選んでいたら、イベント実行
	//	Noを選んでいたら、フラグリセット
	if (CachedHUD->Speech->GetbIsSelectingYES())
	{
		FinishDialogueEvent();	//	子BPで実装
	}
	else 
	{
		bHasQuestion = true;
	}
}
//	次のテキストがあれば切り替える
//	インタラクト時 trueで移行処理停止、falseでFinishInteract
bool ACPP_EnemyBase::NextText()
{
	if (NextRow.IsNone()) 
	{
		//	選択肢があれば回答選択を表示
		//	フラグを切り替える。
		if (bHasQuestion)
		{
			CachedHUD->Speech->ShowAnswer();
			bHasQuestion = false;
			return true;
		}

		//	会話内容を最初に戻す
		NameOfCurrentRow = FirstRowName;
		UpdateDialogue();
		return false;
	}

	NameOfCurrentRow = NextRow;
	UpdateDialogue();	//	会話内容更新

	UE_LOG(LogTemp, Warning, TEXT("Execute NextText"));
	
	return true;
}

//	会話ウィンドウのVisibility切替
void ACPP_EnemyBase::ToggleDialogueVisibility()
{
	bool visibility = DialogueWidget->GetVisibleFlag();
	DialogueWidget->SetVisibility(!visibility);
}

//	会話更新
void ACPP_EnemyBase::UpdateDialogue()
{
	if (!CachedHUD->Speech)
	{
		UE_LOG(LogTemp, Warning, TEXT("CPP_EnemyBase: !HUD->Speech"))
		return;
	}

	//	非同期ロードのリクエスト
	RequestAsyncLoadDataTable();

	CachedHUD->Speech->SetName(this->MyName);		//	吹き出しの名前表示変更
	CachedHUD->Speech->SetSpeekingActor(this);	//	会話者のセット
	CachedHUD->EnableTick();				//	Tickを有効化
	CachedHUD->Speech->FadeInAll();				//	吹き出しをフェードイン
}


//	データテーブルの非同期ロードをリクエスト
void ACPP_EnemyBase::RequestAsyncLoadDataTable()
{
	//	ソフト参照が有効か確認
	if (!DialogueDataTableRef.IsValid())	
	{
		UE_LOG(LogTemp, Warning, TEXT("ACPP_EnemyBase : DialogueDataTableRef is not set"));
		UE_LOG(LogTemp, Warning, TEXT("DataTable Path: %s"), *DialogueDataTableRef.ToSoftObjectPath().ToString());

		return;
	}
	//	ロード済みか確認
	if (DialogueDataTableRef.Get() != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("DataTable is already loaded"))
		OnDataTableLoaded();	//	ロード済みなら即実行
	}

	//	StreamableManagerを取得
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	//	リクエスト
	StreamableManager.RequestAsyncLoad(
		DialogueDataTableRef.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &ACPP_EnemyBase::OnDataTableLoaded)
	);
	//	デバッグ用に通知
	FString Msg = FString::Printf(TEXT("Start async loading DataTable...(%s)"), *DialogueDataTableRef.ToString());
	UKismetSystemLibrary::PrintString(this, Msg, true, true, FLinearColor::Yellow, 2.0f);

}
//	非同期ロード完了時のコールバック処理
void ACPP_EnemyBase::OnDataTableLoaded()
{
	UE_LOG(LogTemp, Log, TEXT("DataTable async load completed"));
	//	UObjectポインタ取得
	UDataTable* LoadDataTable = DialogueDataTableRef.Get();

	//	アセット開いてないとうまくいかないので対策
	//	結局うまくできない…
	//	参考
	//	https://qiita.com/t-okugawa/items/d8f43f6fb2a3f5914b4d
	FString name = LoadDataTable->GetFName().ToString();
	UPackage* Package = LoadPackage(nullptr, *name, LOAD_Verify | LOAD_NoWarn);
	Package->FullyLoad();

	//	ポインタが有効か最終確認
	if (LoadDataTable == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ACPP_EnemyBase: Async load completed, but Get() returned nullptr"));
		return;
	}

	const FString ContextString = TEXT("ACPP_EnemyBase: OnDataTableLoaded");

	UE_LOG(LogTemp, Warning, TEXT("FindRow: NameOfCurrentRow= %s"), *NameOfCurrentRow.ToString());

	const FCPP_DialogueData* RowData = 
		LoadDataTable->FindRow<FCPP_DialogueData>(NameOfCurrentRow, ContextString, false);

	if (!RowData)
	{
		UE_LOG(LogTemp, Error, TEXT("RowData not found!"));
		return;
	}

	//	会話テキスト、次の行を更新
	//	テキスト更新を通知、WBP_Speechで処理実装（バインドは親CPPクラス)
	DialogueText = FText::FromString(RowData->DialogueText);
	NextRow = FName(RowData->NextRow);
	OnChangeDialogue.Broadcast(DialogueText);
}

