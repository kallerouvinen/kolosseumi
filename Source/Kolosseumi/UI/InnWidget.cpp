// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/InnWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Libraries/Statics.h"
#include "Kolosseumi/Messages/ReturnToMainUIMessage.h"
#include "Kolosseumi/States/KolosseumiPlayerState.h"
#include "Kolosseumi/UI/GladiatorInfoWidget.h"
#include "Kolosseumi/UI/RosterInfo/GladiatorDataObj.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/GameplayStatics.h"

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
	if (UGladiatorDataObj* SelectedGladiator = Cast<UGladiatorDataObj>(GladiatorListView->GetSelectedItem()))
	{
		if (AKolosseumiPlayerState* PlayerState = GetOwningPlayerState<AKolosseumiPlayerState>())
		{
			PlayerState->ChangeMoneyAmount(-SelectedGladiator->Salary);
			PlayerState->AddGladiatorToRoster(SelectedGladiator->GetGladiatorDataAsStruct());

			GladiatorListView->RemoveItem(SelectedGladiator);
			GladiatorListView->ClearSelection();
			GladiatorInfo->SetInfo(nullptr);

			UGameplayStatics::PlaySound2D(this, CoinsSound, 0.2f);
		}
	}
}

void UInnWidget::OnBackButtonClicked()
{
	UGameplayStatics::PlaySound2D(this, CloseDoorSound, 0.2f);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_ReturnToMainUI,
			FReturnToMainUIMessage());
}

void UInnWidget::OnGladiatorSelected(UObject* SelectedItem)
{
	bool bCanHire = (SelectedItem != nullptr);
	bool bHasFunds = false;
	bool bHasMaxGladiators = false;

	if (UGladiatorDataObj* GladiatorDataObj = Cast<UGladiatorDataObj>(SelectedItem))
	{
		GladiatorInfo->SetInfo(GladiatorDataObj);

		if (AKolosseumiPlayerState* PlayerState = GetOwningPlayerState<AKolosseumiPlayerState>())
		{
			bHasFunds = PlayerState->GetMoney() >= GladiatorDataObj->Salary;
			bHasMaxGladiators = PlayerState->GetPlayerRoster().Gladiators.Num() >= 6;
		}
	}

	HireButton->SetIsEnabled(bCanHire && bHasFunds && !bHasMaxGladiators);
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
	GladiatorListView->ClearSelection();
	GladiatorInfo->SetInfo(nullptr);
}
