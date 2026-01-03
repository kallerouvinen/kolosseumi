// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/KolosseumiGameState.h"
#include "Kolosseumi/KolosseumiGameMode.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
#include "Kolosseumi/Pawns/Gladiator.h"

void AKolosseumiGameState::BeginPlay()
{
	Super::BeginPlay();
}

void AKolosseumiGameState::InitAliveGladiators(const TArray<TWeakObjectPtr<AGladiator>>& Gladiators)
{
	AliveGladiators = Gladiators;
}

void AKolosseumiGameState::ResetAliveGladiators()
{
	AliveGladiators.Empty();
}

void AKolosseumiGameState::OnGladiatorKnockedOut(TWeakObjectPtr<AGladiator> Gladiator)
{
	AliveGladiators.Remove(Gladiator);

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

	if (AKolosseumiGameMode* GameMode = Cast<AKolosseumiGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (bPlayerGladiatorAlive && !bOpponentGladiatorAlive)
		{
			UE_LOG(LogTemp, Warning, TEXT("Match Ended! Winning Faction: Player"));
			GameMode->EndMatch(EFaction::Player);
		}
		else if (!bPlayerGladiatorAlive && bOpponentGladiatorAlive)
		{
			UE_LOG(LogTemp, Warning, TEXT("Match Ended! Winning Faction: Opponent"));
			GameMode->EndMatch(EFaction::Opponent);
		}
		else if (!bPlayerGladiatorAlive && !bOpponentGladiatorAlive)
		{
			// GameMode->EndMatch(EFaction::Opponent);
			UE_LOG(LogTemp, Warning, TEXT("It's a Draw!"));
			GameMode->EndMatch(EFaction::Player);
		}
	}
}
