// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "KolosseumiHUD.generated.h"

class UEditFormationWidget;
class UMainUIWidget;
class UMatchResultsWidget;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API AKolosseumiHUD : public AHUD
{
	GENERATED_BODY()
public:
	AKolosseumiHUD();

protected:
	virtual void BeginPlay() override;

private:
	TSubclassOf<UMainUIWidget> MainUIWidgetClass;
	TObjectPtr<UMainUIWidget> MainUIWidget;

	TSubclassOf<UEditFormationWidget> EditFormationWidgetClass;
	TObjectPtr<UEditFormationWidget> EditFormationWidget;

	TSubclassOf<UMatchResultsWidget> MatchResultsWidgetClass;
	TObjectPtr<UMatchResultsWidget> MatchResultsWidget;

	TSubclassOf<UUserWidget> OnboardingWidgetClass;
};
