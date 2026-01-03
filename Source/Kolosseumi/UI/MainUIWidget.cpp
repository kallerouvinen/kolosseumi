// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/MainUIWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/StartMatchMessage.h"
#include "Components/Button.h"

void UMainUIWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StartNextMatchButton->OnClicked.AddDynamic(this, &ThisClass::OnStartNextMatchClicked);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MatchEndListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_MatchEnd,
			this,
			&ThisClass::OnMatchEnd);
}

void UMainUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainUIWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(MatchEndListenerHandle);

	Super::NativeDestruct();
}

void UMainUIWidget::OnStartNextMatchClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);

	FStartMatchMessage StartMatchMessage;
	// TODO: Include teams to message
	// StartMatchMessage.PlayerTeam = ...
	// StartMatchMessage.OpponentTeam = ...
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_StartMatch,
			StartMatchMessage);
}

void UMainUIWidget::OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message)
{
	SetVisibility(ESlateVisibility::Visible);
	// TODO: Show match result instead of moving to main UI right away
	// MainUIWidget->ShowMatchResult(Message.WinningFaction);
}
