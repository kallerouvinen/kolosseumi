// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "KolosseumiGameState.generated.h"

class AGladiator;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API AKolosseumiGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	void InitAliveGladiators(const TArray<TWeakObjectPtr<AGladiator>>& Gladiators);
	void ResetAliveGladiators();
	void OnGladiatorKnockedOut(TWeakObjectPtr<AGladiator> Gladiator);

private:
	TArray<TWeakObjectPtr<AGladiator>> AliveGladiators;
};
