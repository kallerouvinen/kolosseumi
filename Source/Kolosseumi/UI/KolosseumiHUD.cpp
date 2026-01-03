// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/KolosseumiHUD.h"
#include "Kolosseumi/UI/MainUIWidget.h"

AKolosseumiHUD::AKolosseumiHUD()
{
	static ConstructorHelpers::FClassFinder<UMainUIWidget> MainUIWidgetClassFinder(TEXT("/Game/UI/WBP_MainUI"));
	if (MainUIWidgetClassFinder.Succeeded())
	{
		MainUIWidgetClass = MainUIWidgetClassFinder.Class;
	}
}

void AKolosseumiHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainUIWidgetClass)
	{
		if (MainUIWidget = CreateWidget<UMainUIWidget>(GetWorld(), MainUIWidgetClass))
		{
			MainUIWidget->AddToViewport();
		}
	}
}
