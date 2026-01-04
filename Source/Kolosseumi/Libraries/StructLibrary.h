// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
#include "Engine/DataTable.h"
#include "StructLibrary.generated.h"

USTRUCT(BlueprintType)
struct FGladiatorData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Name;
	UPROPERTY(EditAnywhere)
	EGladiatorClass Class;

	UPROPERTY(EditAnywhere)
	int32 Health;
	UPROPERTY(EditAnywhere)
	int32 Strength;
	UPROPERTY(EditAnywhere)
	int32 Agility;
	UPROPERTY(EditAnywhere)
	int32 Mana;
};

USTRUCT(BlueprintType)
struct FRosterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<int32, FGladiatorData> Gladiators;
};
