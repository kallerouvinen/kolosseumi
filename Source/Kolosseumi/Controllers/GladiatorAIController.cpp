// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Controllers/GladiatorAIController.h"
#include "Kolosseumi/Pawns/Gladiator.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AGladiatorAIController::AGladiatorAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeFinder(TEXT("/Game/AI/BT_Gladiator"));
	if (BehaviorTreeFinder.Succeeded())
	{
		AIBehaviorTree = BehaviorTreeFinder.Object;
	}
}

void AGladiatorAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!ensureMsgf(AIBehaviorTree, TEXT("AIBehaviorTree is not found in GladiatorAIController")))
	{
		return;
	}

	RunBehaviorTree(AIBehaviorTree);
}

void AGladiatorAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AGladiatorAIController::SetAttackTargetToClosest()
{
	if (AGladiator* ControlledGladiator = Cast<AGladiator>(GetPawn()))
	{
		AGladiator* ClosestOpponentGladiator = nullptr;
		float ClosestDistanceSq = TNumericLimits<float>::Max();

		TArray<AActor*> Gladiators;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGladiator::StaticClass(), Gladiators);
		UE_LOG(LogTemp, Warning, TEXT("GladiatorAIController possessed pawn"));
		UE_LOG(LogTemp, Warning, TEXT("Found %d gladiators in the world"), Gladiators.Num());

		for (AActor* Actor : Gladiators)
		{
			if (AGladiator* Gladiator = Cast<AGladiator>(Actor))
			{
				if (Gladiator == ControlledGladiator) continue;
				if (Gladiator->GetFaction() == ControlledGladiator->GetFaction()) continue;

				const float DistanceSq = FVector::DistSquared(
						ControlledGladiator->GetActorLocation(),
						Gladiator->GetActorLocation());

				if (DistanceSq < ClosestDistanceSq)
				{
					ClosestDistanceSq = DistanceSq;
					ClosestOpponentGladiator = Gladiator;
				}
			}
		}

		if (ClosestOpponentGladiator)
		{
			GetBlackboardComponent()->SetValueAsObject(
					TEXT("AttackTarget"),
					ClosestOpponentGladiator);
		}
	}
}
