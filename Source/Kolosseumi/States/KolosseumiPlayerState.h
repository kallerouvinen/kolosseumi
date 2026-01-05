// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/StructLibrary.h"
#include "Kolosseumi/Messages/MatchEndMessage.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTagContainer.h"
#include "KolosseumiPlayerState.generated.h"

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API AKolosseumiPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	const FRosterData& GetPlayerRoster() const { return PlayerRoster; }

	int32 GetMoney() const { return Money; }

private:
	void GenerateRoster(int32 NumGladiators = 3);

	FRosterData PlayerRoster;
	int32 Money = 100;

	void ChangeMoneyAmount(int32 Amount);

	UFUNCTION()
	void OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message);
	FGameplayMessageListenerHandle MatchEndListenerHandle;

	static const TArray<FString> AllGladiatorNames;
};
