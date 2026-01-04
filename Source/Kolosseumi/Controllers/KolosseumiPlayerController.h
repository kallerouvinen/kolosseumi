// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
#include "GameFramework/PlayerController.h"
#include "KolosseumiPlayerController.generated.h"

class AGladiator;
class ASpawnPoint;
class UInputAction;
class UInputMappingContext;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API AKolosseumiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AKolosseumiPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	TObjectPtr<UInputAction> SelectAction;

	void OnSelectStarted();
	void OnSelectCompleted();
	ASpawnPoint* GetClosestUnoccupiedSpawnPointWithinRange(EFaction Faction, const FVector& Location, float Range) const;

	ASpawnPoint* ClosestSpawnPoint = nullptr;
	AGladiator* GrabbedGladiator = nullptr;
};
