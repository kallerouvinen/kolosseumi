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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	const FRosterData& GetPlayerRoster() const { return PlayerRoster; }

private:
	void GenerateRoster(int32 NumGladiators = 3);

	FRosterData PlayerRoster;

	static const TArray<FString> AllGladiatorNames;
};
