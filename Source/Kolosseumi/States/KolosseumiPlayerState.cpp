// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/States/KolosseumiPlayerState.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/RosterChangedMessage.h"
#include "GameFramework/GameplayMessageSubsystem.h"

const TArray<FString> AKolosseumiPlayerState::AllGladiatorNames = {
	"Murikka",
	"Raknar",
	"Seppo",
};

void AKolosseumiPlayerState::BeginPlay()
{
	Super::BeginPlay();

	GenerateRoster(6);
}

void AKolosseumiPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AKolosseumiPlayerState::GenerateRoster(int32 NumGladiators)
{
	PlayerRoster.Gladiators.Empty();

	for (int32 i = 0; i < NumGladiators; ++i)
	{
		FGladiatorData NewGladiator;
		NewGladiator.Name = AllGladiatorNames[FMath::RandRange(0, AllGladiatorNames.Num() - 1)];
		NewGladiator.Class = static_cast<EGladiatorClass>(FMath::RandRange(0, static_cast<int32>(EGladiatorClass::Rogue)));

		NewGladiator.Health = FMath::RandRange(80, 120);
		NewGladiator.Strength = FMath::RandRange(10, 20);
		NewGladiator.Agility = FMath::RandRange(10, 20);
		NewGladiator.Mana = FMath::RandRange(5, 15);

		PlayerRoster.Gladiators.Add(i, NewGladiator);
	}

	FRosterChangedMessage RosterChangedMessage;
	RosterChangedMessage.NewRoster = PlayerRoster;
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_RosterChanged,
			RosterChangedMessage);
}
