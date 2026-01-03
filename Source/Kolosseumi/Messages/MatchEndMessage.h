// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
#include "MatchEndMessage.generated.h"

USTRUCT(BlueprintType)
struct FMatchEndMessage
{
	GENERATED_BODY()

public:
	UPROPERTY()
	EFaction WinningFaction;
};
