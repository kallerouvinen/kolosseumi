// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/MatchResultsWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/MoneyChangedMessage.h"
#include "Kolosseumi/Messages/ReturnToMainUIMessage.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMatchResultsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ReturnToMainUIButton->OnClicked.AddDynamic(this, &ThisClass::OnReturnToMainUIClicked);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MatchEndListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_MatchEnd,
			this,
			&ThisClass::OnMatchEnd);
	MoneyChangedListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_MoneyChanged,
			this,
			&ThisClass::OnMoneyChanged);
}

void UMatchResultsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMatchResultsWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(MatchEndListenerHandle);
	MessageSubsystem.UnregisterListener(MoneyChangedListenerHandle);

	Super::NativeDestruct();
}

void UMatchResultsWidget::OnReturnToMainUIClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_ReturnToMainUI,
			FReturnToMainUIMessage());
}

void UMatchResultsWidget::OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			FTimerDelegate::CreateLambda([Message, this]() {
				SetVisibility(ESlateVisibility::Visible);

				FString MatchResult = GetMatchResultText(Message.WinningFaction);

				MatchResultText->SetText(FText::FromString(MatchResult));
			}),
			DelayAfterMatchEnd,
			false);
}

void UMatchResultsWidget::OnMoneyChanged(FGameplayTag Channel, const FMoneyChangedMessage& Message)
{
	FString RewardString;

	if (Message.ChangeAmount < 30)
	{
		RewardString = FString::Printf(TEXT("Sait %d rahaa lohdutuspalkintona"), Message.ChangeAmount);
	}
	else
	{
		RewardString = FString::Printf(TEXT("Sait %d rahaa palkintona voitosta"), Message.ChangeAmount);
	}

	MoneyRewardText->SetText(FText::FromString(RewardString));
}

FString UMatchResultsWidget::GetMatchResultText(EFaction WinningFaction) const
{
	switch (WinningFaction)
	{
		case EFaction::Player:
			return TEXT("Voitto!");
		case EFaction::Opponent:
			return TEXT("Tappio!");
		case EFaction::None:
		default:
			return TEXT("Tasapeli!");
	}
}
