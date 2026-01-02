// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/AI/BTTask_Attack.h"
#include "Kolosseumi/Controllers/GladiatorAIController.h"
#include "Kolosseumi/Pawns/Gladiator.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UObject* AttackTargetObj = OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()))
	{
		if (AGladiator* AttackTarget = Cast<AGladiator>(AttackTargetObj))
		{
			if (AGladiatorAIController* AIController = Cast<AGladiatorAIController>(OwnerComp.GetOwner()))
			{
				if (AGladiator* ControlledGladiator = Cast<AGladiator>(AIController->GetPawn()))
				{
					if (!ControlledGladiator->IsAttacking())
					{
						ControlledGladiator->Attack(AttackTarget);

						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
