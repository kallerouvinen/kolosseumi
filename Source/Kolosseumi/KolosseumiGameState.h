// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Messages/GladiatorKnockedOutMessage.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTagContainer.h"
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void InitAliveGladiators(const TArray<TWeakObjectPtr<AGladiator>>& Gladiators);
	void ResetAliveGladiators();

private:
	TArray<TWeakObjectPtr<AGladiator>> AliveGladiators;

	UFUNCTION()
	void OnGladiatorKnockedOut(FGameplayTag Channel, const FGladiatorKnockedOutMessage& Message);
	FGameplayMessageListenerHandle GladiatorKnockedOutListenerHandle;
};
