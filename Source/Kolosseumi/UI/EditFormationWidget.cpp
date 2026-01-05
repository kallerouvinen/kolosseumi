// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/EditFormationWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/GladiatorMovedMessage.h"
#include "Kolosseumi/Messages/StartFormationEditingMessage.h"
#include "Kolosseumi/Messages/StartMatchMessage.h"
#include "Kolosseumi/Pawns/Gladiator.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UEditFormationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StartMatchButton->OnClicked.AddDynamic(this, &ThisClass::OnStartMatchClicked);
	StartMatchButton->SetIsEnabled(false);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	StartFormationEditingListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_StartFormationEditing,
			this,
			&ThisClass::OnStartFormationEditing);
	GladiatorMovedListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_GladiatorMoved,
			this,
			&ThisClass::OnGladiatorMoved);
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

	UGameplayStatics::PlaySound2D(this, StartMatchSound, 0.2f);

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

void UEditFormationWidget::OnGladiatorMoved(FGameplayTag Channel, const FGladiatorMovedMessage& Message)
{
	TArray<AActor*> Gladiators;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGladiator::StaticClass(), Gladiators);
	for (AActor* Actor : Gladiators)
	{
		if (AGladiator* Gladiator = Cast<AGladiator>(Actor))
		{
			if (Gladiator->GetFaction() != EFaction::Player) continue;
			if (!Gladiator->IsAtSidelines())
			{
				StartMatchButton->SetIsEnabled(true);
				return;
			}
		}
	}

	StartMatchButton->SetIsEnabled(false);
}
