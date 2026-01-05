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
	FGuid ID = FGuid::NewGuid();
	UPROPERTY()
	FString Name;
	UPROPERTY()
	EGladiatorClass Class;

	UPROPERTY()
	int32 Health;
	UPROPERTY()
	int32 AttackDamage;
	UPROPERTY()
	int32 Dodge;
	UPROPERTY()
	int32 Salary;

	bool operator==(const FGladiatorData& Other) const
	{
		return ID == Other.ID;
	}
};

USTRUCT(BlueprintType)
struct FRosterData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<int32, FGladiatorData> Gladiators;

	int32 GetTotalSalary() const
	{
		int32 Total = 0;
		for (const auto& [Index, Gladiator] : Gladiators)
		{
			Total += Gladiator.Salary;
		}
		return Total;
	}
};
