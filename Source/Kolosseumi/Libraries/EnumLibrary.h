// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnumLibrary.generated.h"

UENUM(BlueprintType)
enum class EFaction : uint8
{
	Player UMETA(DisplayName = "Player"),
	Opponent UMETA(DisplayName = "Opponent"),
};
