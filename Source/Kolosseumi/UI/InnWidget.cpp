// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/InnWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Libraries/Statics.h"
#include "Kolosseumi/Messages/ReturnToMainUIMessage.h"
#include "Kolosseumi/UI/RosterInfo/GladiatorDataObj.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "GameFramework/GameplayMessageSubsystem.h"

void UInnWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HireButton->OnClicked.AddDynamic(this, &ThisClass::OnHireButtonClicked);
	BackButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButtonClicked);
	GladiatorListView->OnItemSelectionChanged().AddUObject(
			this,
			&ThisClass::OnGladiatorSelected);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MatchEndListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_MatchEnd,
			this,
			&ThisClass::OnMatchEnd);
}

void UInnWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GenerateNewGladiatorsForHire();
	HireButton->SetIsEnabled(false);
}

void UInnWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(MatchEndListenerHandle);

	Super::NativeDestruct();
}

void UInnWidget::OnHireButtonClicked()
{
}

void UInnWidget::OnBackButtonClicked()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_ReturnToMainUI,
			FReturnToMainUIMessage());
}

void UInnWidget::OnGladiatorSelected(UObject* SelectedItem)
{
	UE_LOG(LogTemp, Warning, TEXT("Gladiator selected in InnWidget"));

	// TODO: Update gladiator info when player selects gladiator

	bool bCanHire = (SelectedItem != nullptr);
	bool bHasFunds = true; // TODO: Check player funds
	HireButton->SetIsEnabled(bCanHire && bHasFunds);
}

void UInnWidget::OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message)
{
	GenerateNewGladiatorsForHire();
}

void UInnWidget::GenerateNewGladiatorsForHire()
{
	TArray<FGladiatorData> NewGladiators;
	int32 NumNewGladiators = 3;

	for (int32 i = 0; i < NumNewGladiators; ++i)
	{
		NewGladiators.Add(UStatics::GenerateGladiatorData());
	}

	TArray<UGladiatorDataObj*> GladiatorDataObjects;
	for (const FGladiatorData& GladiatorData : NewGladiators)
	{
		UGladiatorDataObj* DataObj = NewObject<UGladiatorDataObj>(this);
		DataObj->Init(GladiatorData);
		GladiatorDataObjects.Add(DataObj);
	}

	GladiatorListView->SetListItems(GladiatorDataObjects);
}
