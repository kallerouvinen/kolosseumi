// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/EditFormationWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/StartFormationEditingMessage.h"
#include "Kolosseumi/Messages/StartMatchMessage.h"
#include "Components/Button.h"

void UEditFormationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StartMatchButton->OnClicked.AddDynamic(this, &ThisClass::OnStartMatchClicked);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	StartFormationEditingListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_StartFormationEditing,
			this,
			&ThisClass::OnStartFormationEditing);
}

void UEditFormationWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(StartFormationEditingListenerHandle);

	Super::NativeDestruct();
}

void UEditFormationWidget::OnStartMatchClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);

	FStartMatchMessage StartMatchMessage;
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_StartMatch,
			StartMatchMessage);
}

void UEditFormationWidget::OnStartFormationEditing(FGameplayTag Channel, const FStartFormationEditingMessage& Message)
{
	SetVisibility(ESlateVisibility::Visible);
}
