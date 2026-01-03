// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
#include "StructLibrary.generated.h"

USTRUCT(BlueprintType)
struct FGladiatorData
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;
	UPROPERTY()
	EGladiatorClass Class;

	UPROPERTY()
	int32 Health;
	UPROPERTY()
	int32 Strength;
	UPROPERTY()
	int32 Agility;
	UPROPERTY()
	int32 Mana;
};

USTRUCT(BlueprintType)
struct FRosterData
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<int32, FGladiatorData> Gladiators;
};
