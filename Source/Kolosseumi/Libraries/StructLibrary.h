// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
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
	int32 AttackDamage;
	UPROPERTY(EditAnywhere)
	int32 Dodge;
	UPROPERTY(EditAnywhere)
	int32 Salary;
};

USTRUCT(BlueprintType)
struct FRosterData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
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
