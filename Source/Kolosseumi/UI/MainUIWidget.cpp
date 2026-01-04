// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/MainUIWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Managers/OpponentTeamManager.h"
#include "Kolosseumi/Messages/StartFormationEditingMessage.h"
#include "Kolosseumi/States/KolosseumiPlayerState.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

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
	}

	if (AActor* ManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AOpponentTeamManager::StaticClass()))
	{
		if (AOpponentTeamManager* Manager = Cast<AOpponentTeamManager>(ManagerActor))
		{
			if (FRosterData* OpponentRoster = Manager->GetRandomOpponentRoster())
			{
				FormationEditingMessage.OpponentTeam = *OpponentRoster;
			}
		}
	}

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_StartFormationEditing,
			FormationEditingMessage);
}

void UMainUIWidget::OnReturnToMainUI(FGameplayTag Channel, const FReturnToMainUIMessage& Message)
{
	SetVisibility(ESlateVisibility::Visible);
}
