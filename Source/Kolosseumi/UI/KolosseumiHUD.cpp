// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/KolosseumiHUD.h"
#include "Kolosseumi/UI/EditFormationWidget.h"
#include "Kolosseumi/UI/EventWidget.h"
#include "Kolosseumi/UI/MainUIWidget.h"
#include "Kolosseumi/UI/MatchResultsWidget.h"
#include "Kolosseumi/UI/QuitWidget.h"

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

	static ConstructorHelpers::FClassFinder<UEventWidget> EventWidgetClassFinder(TEXT("/Game/UI/WBP_Event"));
	if (EventWidgetClassFinder.Succeeded())
	{
		EventWidgetClass = EventWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UQuitWidget> QuitConfirmationWidgetClassFinder(TEXT("/Game/UI/WBP_QuitConfirmation"));
	if (QuitConfirmationWidgetClassFinder.Succeeded())
	{
		QuitConfirmationWidgetClass = QuitConfirmationWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> OnboardingWidgetClassFinder(TEXT("/Game/UI/WBP_Onboarding"));
	if (OnboardingWidgetClassFinder.Succeeded())
	{
		OnboardingWidgetClass = OnboardingWidgetClassFinder.Class;
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

	if (EventWidgetClass)
	{
		if (EventWidget = CreateWidget<UEventWidget>(GetWorld(), EventWidgetClass))
		{
			EventWidget->AddToViewport();
			EventWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (OnboardingWidgetClass)
	{
		if (UUserWidget* OnboardingWidget = CreateWidget<UUserWidget>(GetWorld(), OnboardingWidgetClass))
		{
			OnboardingWidget->AddToViewport();
		}
	}

	if (QuitConfirmationWidgetClass)
	{
		if (QuitConfirmationWidget = CreateWidget<UQuitWidget>(GetWorld(), QuitConfirmationWidgetClass))
		{
			QuitConfirmationWidget->AddToViewport();
			QuitConfirmationWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
