// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
#include "Kolosseumi/Libraries/StructLibrary.h"
#include "Kolosseumi/Messages/MatchEndMessage.h"
#include "Kolosseumi/Messages/StartMatchMessage.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTagContainer.h"
#include "KolosseumiGameMode.generated.h"

class AGladiator;
class AKolosseumiPlayerState;

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void SpawnGladiators(EFaction Faction, const FRosterData& Roster);
	void RemoveAllGladiatorsFromWorld();
	AKolosseumiPlayerState* GetPlayerState() const;

	UPROPERTY()
	TSubclassOf<AGladiator> GladiatorClass;

	UFUNCTION()
	void OnStartMatch(FGameplayTag Channel, const FStartMatchMessage& Message);
	FGameplayMessageListenerHandle StartMatchListenerHandle;
	UFUNCTION()
	void OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message);
	FGameplayMessageListenerHandle MatchEndListenerHandle;
};
