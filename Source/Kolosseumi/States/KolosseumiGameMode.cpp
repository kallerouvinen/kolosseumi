// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/States/KolosseumiGameMode.h"
#include "Kolosseumi/Actors/SpawnPoint.h"
#include "Kolosseumi/Controllers/GladiatorAIController.h"
#include "Kolosseumi/Controllers/KolosseumiPlayerController.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/MatchEndMessage.h"
#include "Kolosseumi/Pawns/CameraPawn.h"
#include "Kolosseumi/Pawns/Gladiator.h"
#include "Kolosseumi/States/KolosseumiGameState.h"
#include "Kolosseumi/States/KolosseumiPlayerState.h"
#include "Kolosseumi/UI/KolosseumiHUD.h"
#include "Kismet/GameplayStatics.h"

AKolosseumiGameMode::AKolosseumiGameMode()
{
	DefaultPawnClass = ACameraPawn::StaticClass();
	HUDClass = AKolosseumiHUD::StaticClass();
	GameStateClass = AKolosseumiGameState::StaticClass();
	PlayerControllerClass = AKolosseumiPlayerController::StaticClass();
	PlayerStateClass = AKolosseumiPlayerState::StaticClass();

	static ConstructorHelpers::FClassFinder<AGladiator> GladiatorClassFinder(TEXT("/Game/Characters/BP_Gladiator"));
	if (GladiatorClassFinder.Succeeded())
	{
		GladiatorClass = GladiatorClassFinder.Class;
	}
}

void AKolosseumiGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	StartFormationEditingListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_StartFormationEditing,
			this,
			&ThisClass::OnStartFormationEditing);
	MatchEndListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_MatchEnd,
			this,
			&ThisClass::OnMatchEnd);
}

void AKolosseumiGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(StartFormationEditingListenerHandle);
	MessageSubsystem.UnregisterListener(MatchEndListenerHandle);

	Super::EndPlay(EndPlayReason);
}

void AKolosseumiGameMode::SpawnGladiators(EFaction Faction, const FRosterData& Roster)
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), SpawnPoints);

	TArray<TWeakObjectPtr<AGladiator>> AliveGladiators;

	for (AActor* Actor : SpawnPoints)
	{
		if (ASpawnPoint* SpawnPoint = Cast<ASpawnPoint>(Actor))
		{
			if (SpawnPoint->GetFaction() != Faction) continue;
			if (!Roster.Gladiators.Contains(SpawnPoint->GetIndex())) continue;

			FTransform SpawnTransform = SpawnPoint->GetActorTransform();

			if (AGladiator* SpawnedGladiator = GetWorld()->SpawnActorDeferred<AGladiator>(
							GladiatorClass,
							SpawnTransform))
			{
				SpawnedGladiator->SetFaction(SpawnPoint->GetFaction());
				SpawnedGladiator->SetIsAtSidelines(SpawnPoint->IsAtSidelines());
				// TODO: Set gladiator data from roster
				SpawnedGladiator->FinishSpawning(SpawnTransform);

				AliveGladiators.Add(SpawnedGladiator);
			}
		}
	}

	if (AKolosseumiGameState* KolosseumiGameState = Cast<AKolosseumiGameState>(GameState))
	{
		KolosseumiGameState->InitAliveGladiators(AliveGladiators);
	}
}

void AKolosseumiGameMode::RemoveAllGladiatorsFromWorld()
{
	TArray<AActor*> Gladiators;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGladiator::StaticClass(), Gladiators);

	for (AActor* Actor : Gladiators)
	{
		if (AGladiator* Gladiator = Cast<AGladiator>(Actor))
		{
			Gladiator->Destroy();
		}
	}
}

void AKolosseumiGameMode::OnStartFormationEditing(FGameplayTag Channel, const FStartFormationEditingMessage& Message)
{
	RemoveAllGladiatorsFromWorld();

	SpawnGladiators(EFaction::Player, Message.PlayerTeam);
	SpawnGladiators(EFaction::Opponent, Message.OpponentTeam);
}

void AKolosseumiGameMode::OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message)
{
	// TODO: Is this needed?
}

AKolosseumiPlayerState* AKolosseumiGameMode::GetPlayerState() const
{
	AKolosseumiPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<AKolosseumiPlayerController>();
	if (!PlayerController) return nullptr;

	return PlayerController->GetPlayerState<AKolosseumiPlayerState>();
}
