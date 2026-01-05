// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/States/KolosseumiPlayerState.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/MoneyChangedMessage.h"
#include "Kolosseumi/Messages/RosterChangedMessage.h"
#include "GameFramework/GameplayMessageSubsystem.h"

const TArray<FString> AKolosseumiPlayerState::AllGladiatorNames = {
	"Murikka",
	"Raknar",
	"Seppo",
	"Vilunki-Petteri",
};

void AKolosseumiPlayerState::BeginPlay()
{
	Super::BeginPlay();

	GenerateRoster(6);
	ChangeMoneyAmount(0); // To broadcast initial money amount

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MatchEndListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_MatchEnd,
			this,
			&ThisClass::OnMatchEnd);
}

void AKolosseumiPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(MatchEndListenerHandle);

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

void AKolosseumiPlayerState::ChangeMoneyAmount(int32 Amount)
{
	Money += Amount;

	FMoneyChangedMessage MoneyChangedMessage;
	MoneyChangedMessage.NewAmount = Money;
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_MoneyChanged,
			MoneyChangedMessage);
}

void AKolosseumiPlayerState::OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message)
{
	int32 BaseRewardAmount = Message.WinningFaction == EFaction::Player ? 40 : 20;
	int32 Variance = FMath::RandRange(-5, 5);
	int32 FinalAmount = BaseRewardAmount + Variance;

	ChangeMoneyAmount(FinalAmount);
}
