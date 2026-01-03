// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/MatchResultsWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
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
}

void UMatchResultsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMatchResultsWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(MatchEndListenerHandle);

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

				FString MatchResult = Message.WinningFaction == EFaction::Player
						? TEXT("Victory!")
						: TEXT("Defeat!");

				MatchResultText->SetText(FText::FromString(MatchResult));
			}),
			DelayAfterMatchEnd,
			false);
}
