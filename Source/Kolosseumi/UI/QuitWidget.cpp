// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/QuitWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UQuitWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CancelButton->OnClicked.AddDynamic(this, &ThisClass::OnCancelClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &ThisClass::OnConfirmClicked);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	QuitListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_QuitGame,
			this,
			&ThisClass::OnQuitGame);
}

void UQuitWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UQuitWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(QuitListenerHandle);

	Super::NativeDestruct();
}

void UQuitWidget::OnCancelClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UQuitWidget::OnConfirmClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UQuitWidget::OnQuitGame(FGameplayTag Channel, const FQuitGameMessage& Message)
{
	SetVisibility(ESlateVisibility::Visible);
}
