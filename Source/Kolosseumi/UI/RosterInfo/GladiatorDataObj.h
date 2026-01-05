// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
#include "Kolosseumi/Libraries/StructLibrary.h"
#include "UObject/NoExportTypes.h"
#include "GladiatorDataObj.generated.h"

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API UGladiatorDataObj : public UObject
{
	GENERATED_BODY()

public:
	void Init(const FGladiatorData& GladiatorData)
	{
		Name = GladiatorData.Name;
		Class = GladiatorData.Class;
		Health = GladiatorData.Health;
		AttackDamage = GladiatorData.AttackDamage;
		Dodge = GladiatorData.Dodge;
		Salary = GladiatorData.Salary;
	}

	FString Name;
	EGladiatorClass Class;
	int32 Health;
	int32 AttackDamage;
	int32 Dodge;
	int32 Salary;
};
