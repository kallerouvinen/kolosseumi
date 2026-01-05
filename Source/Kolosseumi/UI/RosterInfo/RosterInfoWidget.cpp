// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/RosterInfo/RosterInfoWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/States/KolosseumiPlayerState.h"
#include "Kolosseumi/UI/GladiatorInfoWidget.h"
#include "Kolosseumi/UI/RosterInfo/GladiatorDataObj.h"
#include "Components/ListView.h"

void URosterInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	RosterChangedListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_RosterChanged,
			this,
			&ThisClass::OnRosterChanged);

	RosterListView->OnItemSelectionChanged().AddUObject(
			this,
			&ThisClass::OnGladiatorSelected);
}

void URosterInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AKolosseumiPlayerState* PlayerState = GetOwningPlayerState<AKolosseumiPlayerState>())
	{
		OnRosterChanged(PlayerState->GetPlayerRoster());
	}
}

void URosterInfoWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(RosterChangedListenerHandle);

	Super::NativeDestruct();
}

void URosterInfoWidget::OnGladiatorSelected(UObject* SelectedItem)
{
	if (UGladiatorDataObj* GladiatorDataObj = Cast<UGladiatorDataObj>(SelectedItem))
	{
		GladiatorInfo->SetInfo(GladiatorDataObj);
	}
}

void URosterInfoWidget::OnRosterChanged(FGameplayTag Channel, const FRosterChangedMessage& Message)
{
	OnRosterChanged(Message.NewRoster);
}

void URosterInfoWidget::OnRosterChanged(const FRosterData& Roster)
{
	TArray<UGladiatorDataObj*> GladiatorDataObjects;
	for (const auto& [Index, GladiatorData] : Roster.Gladiators)
	{
		UGladiatorDataObj* DataObj = NewObject<UGladiatorDataObj>(this);
		DataObj->Init(GladiatorData);
		GladiatorDataObjects.Add(DataObj);
	}

	RosterListView->SetListItems(GladiatorDataObjects);
}
