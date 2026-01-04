// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RangedAttack.generated.h"

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API UBTTask_RangedAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_RangedAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
