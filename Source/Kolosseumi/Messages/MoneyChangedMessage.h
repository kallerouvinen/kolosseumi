// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MoneyChangedMessage.generated.h"

USTRUCT(BlueprintType)
struct FMoneyChangedMessage
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 ChangeAmount;
	UPROPERTY()
	int32 NewAmount;
};
