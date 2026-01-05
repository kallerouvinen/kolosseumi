// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/StructLibrary.h"
#include "GameFramework/PlayerState.h"
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

public:
	void AddGladiatorToRoster(const FGladiatorData& GladiatorData);
	void ChangeMoneyAmount(int32 Amount);

	const FRosterData& GetPlayerRoster() const { return PlayerRoster; }
	int32 GetMoney() const { return Money; }

private:
	void GenerateRoster(int32 NumGladiators = 3);

	FRosterData PlayerRoster;
	int32 Money = 50;
};
