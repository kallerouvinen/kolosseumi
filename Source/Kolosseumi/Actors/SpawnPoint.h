// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
#include "Engine/TargetPoint.h"
#include "SpawnPoint.generated.h"

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API ASpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	EFaction GetFaction() const { return Faction; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnPoint")
	EFaction Faction;
};
