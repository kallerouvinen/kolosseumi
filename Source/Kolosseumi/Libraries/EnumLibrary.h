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
	Barbarian UMETA(DisplayName = "Barbaari"),
	Knight UMETA(DisplayName = "Ritari"),
	Mage UMETA(DisplayName = "Velho"),
	Ranger UMETA(DisplayName = "Jousimies"),
	Rogue UMETA(DisplayName = "Rosvo"),
};
