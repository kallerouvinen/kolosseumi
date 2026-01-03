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
		// UE_LOG(LogTemp, Warning, TEXT("GameState received GladiatorKnockedOut message for %s"), *Message.Gladiator->GetName());

		AliveGladiators.Remove(Message.Gladiator);

		// TODO: Check for match end conditions here
		bool bPlayerGladiatorAlive = false;
		bool bOpponentGladiatorAlive = false;

		for (const TWeakObjectPtr<AGladiator>& RemainingGladiator : AliveGladiators)
		{
			if (RemainingGladiator.IsValid())
			{
				if (RemainingGladiator->GetFaction() == EFaction::Player)
				{
					bPlayerGladiatorAlive = true;
				}
				else
				{
					bOpponentGladiatorAlive = true;
				}
			}
		}

		// TODO: If match should end here, broadcast match end message with winning faction

		// if (AKolosseumiGameMode* GameMode = Cast<AKolosseumiGameMode>(GetWorld()->GetAuthGameMode()))
		// {
		// 	if (bPlayerGladiatorAlive && !bOpponentGladiatorAlive)
		// 	{
		// 		UE_LOG(LogTemp, Warning, TEXT("Match Ended! Winning Faction: Player"));
		// 		GameMode->EndMatch(EFaction::Player);
		// 	}
		// 	else if (!bPlayerGladiatorAlive && bOpponentGladiatorAlive)
		// 	{
		// 		UE_LOG(LogTemp, Warning, TEXT("Match Ended! Winning Faction: Opponent"));
		// 		GameMode->EndMatch(EFaction::Opponent);
		// 	}
		// 	else if (!bPlayerGladiatorAlive && !bOpponentGladiatorAlive)
		// 	{
		// 		// GameMode->EndMatch(EFaction::Opponent);
		// 		UE_LOG(LogTemp, Warning, TEXT("It's a Draw!"));
		// 		GameMode->EndMatch(EFaction::Player);
		// 	}
		// }
	}
}
