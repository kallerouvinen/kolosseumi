// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KolosseumiGameMode.generated.h"

class AGladiator;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API AKolosseumiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKolosseumiGameMode();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TSubclassOf<AGladiator> GladiatorClass;
};
