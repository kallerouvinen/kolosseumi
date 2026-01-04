// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/StructLibrary.h"
#include "GameFramework/Info.h"
#include "OpponentTeamManager.generated.h"

class UDataTable;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API AOpponentTeamManager : public AInfo
{
	GENERATED_BODY()

public:
	AOpponentTeamManager();

	FRosterData* GetRandomOpponentRoster() const;

private:
	UPROPERTY()
	TObjectPtr<UDataTable> OpponentTeamsTable;
};
