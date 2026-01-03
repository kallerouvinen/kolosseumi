// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GladiatorKnockedOutMessage.generated.h"

class AGladiator;

USTRUCT(BlueprintType)
struct FGladiatorKnockedOutMessage
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AGladiator* Gladiator;
};
