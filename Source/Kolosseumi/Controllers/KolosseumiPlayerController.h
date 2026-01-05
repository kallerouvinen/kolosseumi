// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
#include "GameFramework/GameplayMessageSubsystem.h"
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	TObjectPtr<UInputAction> SelectAction;
	TObjectPtr<UInputAction> QuitAction;

	void OnSelectStarted();
	void OnSelectCompleted();
	void OnQuit();

	ASpawnPoint* GetClosestUnoccupiedSpawnPointWithinRange(EFaction Faction, const FVector& Location, float Range) const;
	void CacheSpawnPoints(bool bFilterByFaction = false, EFaction Faction = EFaction::Player);

	ASpawnPoint* ClosestSpawnPoint = nullptr;
	AGladiator* GrabbedGladiator = nullptr;
	TArray<TWeakObjectPtr<ASpawnPoint>> CachedSpawnPoints;

	UFUNCTION()
	void OnStartFormationEditing(FGameplayTag Channel, const FStartFormationEditingMessage& Message);
	FGameplayMessageListenerHandle StartFormationEditingListenerHandle;
	UFUNCTION()
	void OnStartMatch(FGameplayTag Channel, const FStartMatchMessage& Message);
	FGameplayMessageListenerHandle StartMatchListenerHandle;
};
