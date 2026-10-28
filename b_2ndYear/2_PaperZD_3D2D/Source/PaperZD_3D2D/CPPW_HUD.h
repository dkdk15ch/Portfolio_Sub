// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

class UCanvasPanel;
class UCPPW_Speech;

#include "CPPW_HUD.generated.h"

/**
 * 
 */
UCLASS()
class PAPERZD_3D2D_API UCPPW_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

	void SetPositionBubble();	//	吹き出し位置更新

	//	吹き出しウィジット(WBP_HUDでバインド)
	//	変数名は同じにする必要あり
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCPPW_Speech> Speech;

	void DisableTick() { bEnableTick = false; }
	void EnableTick() { bEnableTick = true;	}

private:
	bool bEnableTick = false;

};
