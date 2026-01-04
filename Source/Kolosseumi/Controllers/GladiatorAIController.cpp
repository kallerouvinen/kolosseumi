// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Controllers/GladiatorAIController.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/GladiatorKnockedOutMessage.h"
#include "Kolosseumi/Pawns/Gladiator.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
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

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	StartMatchListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_StartMatch,
			this,
			&ThisClass::OnStartMatch);
	GladiatorKnockedOutListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_GladiatorKnockedOut,
			this,
			&ThisClass::OnGladiatorKnockedOut);
	MatchEndListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_MatchEnd,
			this,
			&ThisClass::OnMatchEnd);
}

void AGladiatorAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(StartMatchListenerHandle);
	MessageSubsystem.UnregisterListener(GladiatorKnockedOutListenerHandle);
	MessageSubsystem.UnregisterListener(MatchEndListenerHandle);

	Super::EndPlay(EndPlayReason);
}

void AGladiatorAIController::SetAttackTargetToClosest()
{
	if (AGladiator* ControlledGladiator = Cast<AGladiator>(GetPawn()))
	{
		AGladiator* ClosestOpponentGladiator = nullptr;
		float ClosestDistanceSq = TNumericLimits<float>::Max();

		TArray<AActor*> Gladiators;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGladiator::StaticClass(), Gladiators);
		for (AActor* Actor : Gladiators)
		{
			if (AGladiator* Gladiator = Cast<AGladiator>(Actor))
			{
				if (Gladiator == ControlledGladiator) continue;
				if (Gladiator->GetFaction() == ControlledGladiator->GetFaction()) continue;
				if (Gladiator->IsKnockedOut()) continue;
				if (Gladiator->IsAtSidelines()) continue;

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

void AGladiatorAIController::OnStartMatch(FGameplayTag Channel, const FStartMatchMessage& Message)
{
	if (!ensureMsgf(AIBehaviorTree, TEXT("AIBehaviorTree is not found in GladiatorAIController")))
	{
		return;
	}

	if (AGladiator* Gladiator = Cast<AGladiator>(GetPawn()))
	{
		if (Gladiator->IsAtSidelines()) return;

		RunBehaviorTree(AIBehaviorTree);
	}
}

void AGladiatorAIController::OnGladiatorKnockedOut(FGameplayTag Channel, const FGladiatorKnockedOutMessage& Message)
{
	if (!GetBlackboardComponent()) return;

	UObject* CurrentAttackTarget = GetBlackboardComponent()->GetValueAsObject(TEXT("AttackTarget"));
	if (CurrentAttackTarget == Message.Gladiator)
	{
		SetAttackTargetToClosest();
	}
	else if (GetPawn() == Message.Gladiator)
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("AttackTarget"), nullptr);
	}
}

void AGladiatorAIController::OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message)
{
	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("AttackTarget"), nullptr);
	}
}
