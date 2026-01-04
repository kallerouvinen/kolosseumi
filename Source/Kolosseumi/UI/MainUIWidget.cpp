// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/MainUIWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/States/KolosseumiPlayerState.h"
#include "Kolosseumi/Messages/StartFormationEditingMessage.h"
#include "Components/Button.h"

void UMainUIWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StartNextMatchButton->OnClicked.AddDynamic(this, &ThisClass::OnNextMatchClicked);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	ReturnToMainUIListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_ReturnToMainUI,
			this,
			&ThisClass::OnReturnToMainUI);
}

void UMainUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainUIWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(ReturnToMainUIListenerHandle);

	Super::NativeDestruct();
}

void UMainUIWidget::OnNextMatchClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);

	FStartFormationEditingMessage FormationEditingMessage;

	if (AKolosseumiPlayerState* PlayerState = GetOwningPlayerState<AKolosseumiPlayerState>())
	{
		FormationEditingMessage.PlayerTeam = PlayerState->GetPlayerRoster();
		// TODO: Include proper opponent team instead of this placeholder
		FormationEditingMessage.OpponentTeam = PlayerState->GetPlayerRoster();
	}
	// FormationEditingMessage.OpponentTeam = ...
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_StartFormationEditing,
			FormationEditingMessage);
}

void UMainUIWidget::OnReturnToMainUI(FGameplayTag Channel, const FReturnToMainUIMessage& Message)
{
	SetVisibility(ESlateVisibility::Visible);
}
