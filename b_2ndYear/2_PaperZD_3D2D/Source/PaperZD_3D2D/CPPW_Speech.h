// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPPW_Speech.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopTick);

/**
 * 
 */
UCLASS()
class PAPERZD_3D2D_API UCPPW_Speech : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

	//	デリゲートプロパティ
	UPROPERTY()
	FOnStopTick OnStopTick;

	//	BPで実装イベント
	UFUNCTION(BlueprintImplementableEvent, Category = "CPPW_Speech|Fade")
	void FadeOutAll();
	UFUNCTION(BlueprintImplementableEvent, Category = "CPPW_Speech|Fade")
	void FadeInAll();
	UFUNCTION(BlueprintImplementableEvent, Category = "CPPW_Speech|Question")
	void ShowAnswer();	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = "CPPW_Speech|Question")
	void HideAnswer();
	
	//	BlueprintNativeEvent	
	//	デリゲートバインドにするため
	UFUNCTION(BlueprintNativeEvent, Category = "CPPW_Speech|Qustion")
	void ToggleAnswerEnabled();
	UFUNCTION(BlueprintNativeEvent, Category = "CPPW_Speech|DialogueText")
	void UpdateDialogueText(const FText& Text);	


	// Setter/Getter
	void SetName(FText nameText) { Name = nameText; }
	void SetDialogueText(FText text) { DialogueText = text; }
	UFUNCTION(BlueprintPure, Category = "CPPW_Speech|DialogueText")
	FText GetNameText()const { return Name; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CPPW_Speech|DialogueText")
	FText GetDialogueText()const { return DialogueText; }
	void SetSpeekingActor(AActor* Actor) { SpeekingActor = Actor; }
	AActor* GetSpeekingActor()const { return SpeekingActor; }
	bool GetbIsSelectingYES()const { return bIsSelectedYES; }
	bool GetbIsFadingOut()const { return bIsFadingOut; }

protected:
	//UPROPERTY(BlueprintReadWrite, Category = "CPPW_Speech|Question")
	//bool bIsSelectingYES = false;

	UPROPERTY(BlueprintReadWrite, Category = "CPPW_Speech|Question")
	bool bIsSelectedYES = false;


	UPROPERTY(BlueprintReadWrite, Category = "CPPW_Speech|Question")
	bool bIsFadingOut = false;

private:
	FText Name = FText::FromString(TEXT("Name"));

	FText DialogueText = FText::FromString(TEXT("会話内容はデータテーブルから"));

	UPROPERTY()
	TObjectPtr<AActor> SpeekingActor;	//	HUDで吹き出し位置の変更に使用

	UFUNCTION(BlueprintCallable, Category = "CPPW_Speech|Dialogue")
	void StopUpdatePosition();
};
