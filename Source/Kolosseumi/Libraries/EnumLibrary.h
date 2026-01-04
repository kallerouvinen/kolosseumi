// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnumLibrary.generated.h"

UENUM(BlueprintType)
enum class EFaction : uint8
{
	Player UMETA(DisplayName = "Player"),
	Opponent UMETA(DisplayName = "Opponent"),
	None UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class EGladiatorClass : uint8
{
	Barbarian UMETA(DisplayName = "Barbarian"),
	Knight UMETA(DisplayName = "Knight"),
	Mage UMETA(DisplayName = "Mage"),
	Ranger UMETA(DisplayName = "Ranger"),
	Rogue UMETA(DisplayName = "Rogue"),
	Skeleton UMETA(DisplayName = "Skeleton"),
};
