// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "CPPI_Interaction.h"
#include "Delegates/DelegateCombinations.h"

class UPaperZDAnimInstance;
class UWidgetComponent;
class ACPP_Player;
class UCPPW_HUD;

#include "CPP_EnemyBase.generated.h"

//	Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeDialogue, const FText&, Text);


/**
 * 
 */
UCLASS()
class PAPERZD_3D2D_API ACPP_EnemyBase : public APaperZDCharacter, public ICPPI_Interaction
{
	GENERATED_BODY()

public:
	ACPP_EnemyBase();

	//	Delegateプロパティ
	UPROPERTY()
	FOnChangeDialogue OnChangeDialogue;


	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "CPP_Enemy|Dialogue")
	virtual void Interact() override;
	UFUNCTION(BlueprintCallable, Category = "CPP_Enemy|Dialogue")
	virtual void FinishInteraction() override;
	UFUNCTION(BlueprintCallable, Category = "CPP_Enemy|Dialogue")
	virtual bool NextText() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_Enemy|UI")
	TObjectPtr<UWidgetComponent> DialogueWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP_Enemy|Dialogue")
	FText MyName;


	//UPROPERTY(EditAnywhere, Category = "Dialogue")
	//FName CurrentRowName;

	UPROPERTY(BlueprintReadOnly,Category = "CPP_Enemy|Dialogue")
	FName NameOfCurrentRow;

	UPROPERTY(EditAnywhere, Category = "CPP_Enemy|Dialogue", meta=(GetOptions="GetNameRowOptions"))
	FName CurrentNameRow;

protected:
	//	会話用テキストデータ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPP_Enemy|Dialogue")
	TSoftObjectPtr<UDataTable> DialogueDataTableRef;

	//	会話終了後イベント
	UFUNCTION(BlueprintImplementableEvent, Category = "CPP_Enemy|Event")
	void FinishDialogueEvent();
	
	//	会話内容のプルダウン選択肢
	UFUNCTION()
	TArray<FName> GetNameRowOptions() const
	{
		return {
			FName("Hiyoko1gou_Message"),
			FName("Hiyoko2gou_Message")
		};
	}

private:
	UPaperZDAnimInstance* AnimInstance;
	
	UPROPERTY()
	UCPPW_HUD* CachedHUD;	//	HUDのキャッシュ
	
	void UpdateDialogue();

	void ToggleDialogueVisibility();

	UPROPERTY()
	FText DialogueText;
	UPROPERTY()
	FName NextRow;
	UPROPERTY()
	FName FirstRowName;

	UPROPERTY(EditAnywhere, Category = "CPP_Enemy|Dialogue")
	bool bHasQuestion;
	
	void RequestAsyncLoadDataTable();	//	データテーブル非同期ロードをリクエスト
	void OnDataTableLoaded();			//	非同期ロード完了時のコールバック処理
};

