// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KolosseumiGameMode.generated.h"

class AGladiator;
class UMainUIWidget;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API AKolosseumiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKolosseumiGameMode();

protected:
	virtual void BeginPlay() override;

public:
	void StartNextMatch();

private:
	void SpawnGladiatorsAtSpawnPoints();
	void AssignAIControllersTargets();

	UPROPERTY()
	TSubclassOf<AGladiator> GladiatorClass;

	UPROPERTY()
	TSubclassOf<UMainUIWidget> MainUIWidgetClass;

	UPROPERTY()
	TObjectPtr<UMainUIWidget> MainUIWidget;
};
