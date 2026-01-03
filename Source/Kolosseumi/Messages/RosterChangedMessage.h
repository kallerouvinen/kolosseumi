// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/StructLibrary.h"
#include "RosterChangedMessage.generated.h"

USTRUCT(BlueprintType)
struct FRosterChangedMessage
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FRosterData NewRoster;
};
