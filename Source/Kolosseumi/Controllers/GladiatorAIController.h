// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Messages/GladiatorKnockedOutMessage.h"
#include "AIController.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTagContainer.h"
#include "GladiatorAIController.generated.h"

class UBehaviorTree;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API AGladiatorAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGladiatorAIController();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// TODO: Blackboard key setters
	void SetAttackTargetToClosest();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> AIBehaviorTree;

private:
	UFUNCTION()
	void OnGladiatorKnockedOut(FGameplayTag Channel, const FGladiatorKnockedOutMessage& Message);
	FGameplayMessageListenerHandle GladiatorKnockedOutListenerHandle;
};
