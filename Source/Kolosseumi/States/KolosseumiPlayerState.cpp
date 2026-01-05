// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/States/KolosseumiPlayerState.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Libraries/Statics.h"
#include "Kolosseumi/Messages/MoneyChangedMessage.h"
#include "Kolosseumi/Messages/RosterChangedMessage.h"
#include "GameFramework/GameplayMessageSubsystem.h"

void AKolosseumiPlayerState::BeginPlay()
{
	Super::BeginPlay();

	ChangeMoneyAmount(0); // To broadcast initial money amount
}

void AKolosseumiPlayerState::AddGladiatorToRoster(const FGladiatorData& GladiatorData)
{
	int32 NewIndex = 0;
	while (PlayerRoster.Gladiators.Contains(NewIndex))
	{
		++NewIndex;
	}
	PlayerRoster.Gladiators.Add(NewIndex, GladiatorData);

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
	MoneyChangedMessage.ChangeAmount = Amount;
	MoneyChangedMessage.NewAmount = Money;
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_MoneyChanged,
			MoneyChangedMessage);
}

void AKolosseumiPlayerState::GenerateRoster(int32 NumGladiators)
{
	PlayerRoster.Gladiators.Empty();

	for (int32 i = 0; i < NumGladiators; ++i)
	{
		PlayerRoster.Gladiators.Add(i, UStatics::GenerateGladiatorData());
	}

	FRosterChangedMessage RosterChangedMessage;
	RosterChangedMessage.NewRoster = PlayerRoster;
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_RosterChanged,
			RosterChangedMessage);
}
