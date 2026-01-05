// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/StructLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Statics.generated.h"

// TODO: Move this to StructLibrary.h
USTRUCT(BlueprintType)
struct FClassBaseStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 Health;
	UPROPERTY(EditAnywhere)
	int32 AttackDamage;
	UPROPERTY(EditAnywhere)
	int32 Dodge;
};

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API UStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FGladiatorData GenerateGladiatorData();

private:
	static const TArray<FString> AllGladiatorNames;
	static const TMap<EGladiatorClass, FClassBaseStats> ClassBaseStats;
};
