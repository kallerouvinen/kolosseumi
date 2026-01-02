// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KolosseumiPlayerController.generated.h"

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API AKolosseumiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AKolosseumiPlayerController();

protected:
	virtual void BeginPlay() override;
};
