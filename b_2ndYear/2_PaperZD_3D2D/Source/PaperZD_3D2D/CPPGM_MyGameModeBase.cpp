#include "CPPGM_MyGameModeBase.h"
#include "Blueprint/UserWidget.h"				//	UseWidget
#include "CPPW_HUD.h"

void ACPPGM_MyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//	HUDの表示
	UClass* WBPClass = StaticLoadClass(UCPPW_HUD::StaticClass(), nullptr, TEXT("/Game/UI/Dialogue/WBP_HUD.WBP_HUD_C"));
	if (WBPClass)
	{
		HUD_Widget = CreateWidget<UCPPW_HUD>(GetWorld(), WBPClass);
		HUD_Widget->AddToViewport();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("!WBPClass"));
	}

}