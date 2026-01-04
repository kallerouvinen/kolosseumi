// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/AI/BTTask_RangedAttack.h"
#include "Kolosseumi/Controllers/GladiatorAIController.h"
#include "Kolosseumi/Pawns/Gladiator.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RangedAttack::UBTTask_RangedAttack()
{
	NodeName = TEXT("RangedAttack");
}

EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
						ControlledGladiator->RangedAttack(AttackTarget);

						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
