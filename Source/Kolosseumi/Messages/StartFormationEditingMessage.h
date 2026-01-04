// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/StructLibrary.h"
#include "StartFormationEditingMessage.generated.h"

USTRUCT(BlueprintType)
struct FStartFormationEditingMessage
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FRosterData PlayerTeam;
	UPROPERTY()
	FRosterData OpponentTeam;
};
