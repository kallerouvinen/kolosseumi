// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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
	virtual void OnPossess(APawn* InPawn) override;

public:
	// TODO: Blackboard key setters
	void SetAttackTargetToClosest();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> AIBehaviorTree;
};
