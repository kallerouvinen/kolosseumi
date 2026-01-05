// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/MainUIWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Managers/OpponentTeamManager.h"
#include "Kolosseumi/Messages/MoneyChangedMessage.h"
#include "Kolosseumi/Messages/StartFormationEditingMessage.h"
#include "Kolosseumi/States/KolosseumiPlayerState.h"
#include "Kolosseumi/UI/InnWidget.h"
#include "Kolosseumi/UI/ShopWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UMainUIWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InnButton->OnClicked.AddDynamic(this, &ThisClass::OnInnButtonClicked);
	ShopButton->OnClicked.AddDynamic(this, &ThisClass::OnShopButtonClicked);
	StartNextMatchButton->OnClicked.AddDynamic(this, &ThisClass::OnNextMatchClicked);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	ReturnToMainUIListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_ReturnToMainUI,
			this,
			&ThisClass::OnReturnToMainUI);
	MoneyChangedListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_MoneyChanged,
			this,
			&ThisClass::OnMoneyChanged);
}

void UMainUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AKolosseumiPlayerState* PlayerState = GetOwningPlayerState<AKolosseumiPlayerState>())
	{
		UpdateMoneyText(PlayerState->GetMoney());
	}

	UGameplayStatics::PlaySound2D(this, BackgroundMusic, 0.2f);
}

void UMainUIWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(ReturnToMainUIListenerHandle);
	MessageSubsystem.UnregisterListener(MoneyChangedListenerHandle);

	Super::NativeDestruct();
}

void UMainUIWidget::OnInnButtonClicked()
{
	WidgetSwitcher->SetActiveWidget(InnWidget);

	UGameplayStatics::PlaySound2D(this, OpenDoorSound, 0.2f);
}

void UMainUIWidget::OnShopButtonClicked()
{
	WidgetSwitcher->SetActiveWidget(ShopWidget);

	UGameplayStatics::PlaySound2D(this, OpenDoorSound, 0.2f);
}

void UMainUIWidget::OnNextMatchClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);

	UGameplayStatics::PlaySound2D(this, NextMatchSound, 0.2f);

	FStartFormationEditingMessage FormationEditingMessage;

	if (AKolosseumiPlayerState* PlayerState = GetOwningPlayerState<AKolosseumiPlayerState>())
	{
		FRosterData PlayerRoster = PlayerState->GetPlayerRoster();
		FormationEditingMessage.PlayerTeam = PlayerRoster;

		if (AActor* ManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AOpponentTeamManager::StaticClass()))
		{
			if (AOpponentTeamManager* Manager = Cast<AOpponentTeamManager>(ManagerActor))
			{
				int32 TargetGoldValue = PlayerRoster.GetTotalSalary();
				FRosterData OpponentRoster = Manager->GenerateOpponentRoster(TargetGoldValue);
				FormationEditingMessage.OpponentTeam = OpponentRoster;
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

	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UMainUIWidget::OnMoneyChanged(FGameplayTag Channel, const FMoneyChangedMessage& Message)
{
	UpdateMoneyText(Message.NewAmount);
}

void UMainUIWidget::UpdateMoneyText(int32 NewAmount)
{
	FString MoneyString = FString::Printf(TEXT("Kultaa: %d"), NewAmount);

	MoneyText->SetText(FText::FromString(MoneyString));
}
