// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/StructLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Statics.generated.h"

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
};
