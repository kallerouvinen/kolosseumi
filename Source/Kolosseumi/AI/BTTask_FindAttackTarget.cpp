// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/AI/BTTask_FindAttackTarget.h"
#include "Kolosseumi/Controllers/GladiatorAIController.h"
#include "Kolosseumi/Pawns/Gladiator.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindAttackTarget::UBTTask_FindAttackTarget()
{
	NodeName = TEXT("Find Attack Target");
}

EBTNodeResult::Type UBTTask_FindAttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AGladiatorAIController* AIController = Cast<AGladiatorAIController>(OwnerComp.GetOwner()))
	{
		AIController->SetAttackTargetToClosest();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
