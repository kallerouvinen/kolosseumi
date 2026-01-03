// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/KolosseumiGameState.h"
#include "Kolosseumi/KolosseumiGameMode.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/GladiatorKnockedOutMessage.h"
#include "Kolosseumi/Pawns/Gladiator.h"
#include "GameFramework/GameplayMessageSubsystem.h"

void AKolosseumiGameState::BeginPlay()
{
	Super::BeginPlay();

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	GladiatorKnockedOutListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_GladiatorKnockedOut,
			this,
			&ThisClass::OnGladiatorKnockedOut);
}

void AKolosseumiGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(GladiatorKnockedOutListenerHandle);

	Super::EndPlay(EndPlayReason);
}

void AKolosseumiGameState::InitAliveGladiators(const TArray<TWeakObjectPtr<AGladiator>>& Gladiators)
{
	AliveGladiators = Gladiators;
}

void AKolosseumiGameState::ResetAliveGladiators()
{
	AliveGladiators.Empty();
}

void AKolosseumiGameState::OnGladiatorKnockedOut(FGameplayTag Channel, const FGladiatorKnockedOutMessage& Message)
{
	if (Message.Gladiator)
	{
		AliveGladiators.Remove(Message.Gladiator);

		bool bIsAnyPlayerGladiatorAlive = false;
		bool bIsAnyOpponentGladiatorAlive = false;

		for (const TWeakObjectPtr<AGladiator>& RemainingGladiator : AliveGladiators)
		{
			if (RemainingGladiator.IsValid())
			{
				if (RemainingGladiator->GetFaction() == EFaction::Player)
				{
					bIsAnyPlayerGladiatorAlive = true;
				}
				else
				{
					bIsAnyOpponentGladiatorAlive = true;
				}
			}
		}

		if (bIsAnyPlayerGladiatorAlive && bIsAnyOpponentGladiatorAlive) return;

		FMatchEndMessage MatchEndMessage;

		if (bIsAnyPlayerGladiatorAlive && !bIsAnyOpponentGladiatorAlive)
		{
			MatchEndMessage.WinningFaction = EFaction::Player;
		}
		else if (!bIsAnyPlayerGladiatorAlive && bIsAnyOpponentGladiatorAlive)
		{
			MatchEndMessage.WinningFaction = EFaction::Opponent;
		}
		else if (!bIsAnyPlayerGladiatorAlive && !bIsAnyOpponentGladiatorAlive)
		{
			// TODO: Should draws be handled differently?
			MatchEndMessage.WinningFaction = EFaction::Player; // Treat draw as player
		}

		UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
		MessageSubsystem.BroadcastMessage(
				KolosseumiTags::Message_MatchEnd,
				MatchEndMessage);
	}
}
