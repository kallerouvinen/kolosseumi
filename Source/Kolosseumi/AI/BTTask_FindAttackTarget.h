// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindAttackTarget.generated.h"

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API UBTTask_FindAttackTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FindAttackTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
