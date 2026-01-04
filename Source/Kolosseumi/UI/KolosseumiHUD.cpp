// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/KolosseumiHUD.h"
#include "Kolosseumi/UI/EditFormationWidget.h"
#include "Kolosseumi/UI/MainUIWidget.h"
#include "Kolosseumi/UI/MatchResultsWidget.h"

AKolosseumiHUD::AKolosseumiHUD()
{
	static ConstructorHelpers::FClassFinder<UMainUIWidget> MainUIWidgetClassFinder(TEXT("/Game/UI/WBP_MainUI"));
	if (MainUIWidgetClassFinder.Succeeded())
	{
		MainUIWidgetClass = MainUIWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UEditFormationWidget> EditFormationWidgetClassFinder(TEXT("/Game/UI/WBP_EditFormation"));
	if (EditFormationWidgetClassFinder.Succeeded())
	{
		EditFormationWidgetClass = EditFormationWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UMatchResultsWidget> MatchResultsWidgetClassFinder(TEXT("/Game/UI/WBP_MatchResults"));
	if (MatchResultsWidgetClassFinder.Succeeded())
	{
		MatchResultsWidgetClass = MatchResultsWidgetClassFinder.Class;
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

	if (EditFormationWidgetClass)
	{
		if (EditFormationWidget = CreateWidget<UEditFormationWidget>(GetWorld(), EditFormationWidgetClass))
		{
			EditFormationWidget->AddToViewport();
			EditFormationWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (MatchResultsWidgetClass)
	{
		if (MatchResultsWidget = CreateWidget<UMatchResultsWidget>(GetWorld(), MatchResultsWidgetClass))
		{
			MatchResultsWidget->AddToViewport();
			MatchResultsWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
