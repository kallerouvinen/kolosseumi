// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/InnWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/ReturnToMainUIMessage.h"
#include "Components/Button.h"
#include "GameFramework/GameplayMessageSubsystem.h"

void UInnWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HireButton->OnClicked.AddDynamic(this, &ThisClass::OnHireButtonClicked);
	BackButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButtonClicked);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MatchEndListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_MatchEnd,
			this,
			&ThisClass::OnMatchEnd);
}

void UInnWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(MatchEndListenerHandle);

	Super::NativeDestruct();
}

void UInnWidget::OnHireButtonClicked()
{
	// TODO: Disable hire button if no funds available
	// TODO: Disable hire button if no gladiator selected
}

void UInnWidget::OnBackButtonClicked()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_ReturnToMainUI,
			FReturnToMainUIMessage());
}

void UInnWidget::OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message)
{
	// TODO: Refresh gladiator list
  // - Generate new gladiators for hire
}
