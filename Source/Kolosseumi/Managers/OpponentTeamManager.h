// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/StructLibrary.h"
#include "GameFramework/Info.h"
#include "OpponentTeamManager.generated.h"

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API AOpponentTeamManager : public AInfo
{
	GENERATED_BODY()

public:
	FRosterData GenerateOpponentRoster(int32 TargetGoldValue) const;
};
