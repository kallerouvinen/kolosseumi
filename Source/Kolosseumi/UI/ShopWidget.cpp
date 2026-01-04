// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/ShopWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/ReturnToMainUIMessage.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "GameFramework/GameplayMessageSubsystem.h"

void UShopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BuyButton->OnClicked.AddDynamic(this, &ThisClass::OnBuyButtonClicked);
	BackButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButtonClicked);
	ItemListView->OnItemSelectionChanged().AddUObject(
			this,
			&ThisClass::OnItemSelected);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MatchEndListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_MatchEnd,
			this,
			&ThisClass::OnMatchEnd);
}

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GenerateNewItemsForSale();
	BuyButton->SetIsEnabled(false);
}

void UShopWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(MatchEndListenerHandle);

	Super::NativeDestruct();
}

void UShopWidget::OnBuyButtonClicked()
{
}

void UShopWidget::OnBackButtonClicked()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_ReturnToMainUI,
			FReturnToMainUIMessage());
}

void UShopWidget::OnItemSelected(UObject* SelectedItem)
{
	UE_LOG(LogTemp, Warning, TEXT("Item selected in ShopWidget"));

	// TODO: Update item info when player selects item

	bool bCanBuy = (SelectedItem != nullptr);
	bool bHasFunds = true; // TODO: Check player funds
	BuyButton->SetIsEnabled(bCanBuy && bHasFunds);
}

void UShopWidget::OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message)
{
	GenerateNewItemsForSale();
}

void UShopWidget::GenerateNewItemsForSale()
{
	// TODO: Get random items from data table?

	// TArray<FItemData> NewItems;
	// int32 NumNewItems = 3;

	// for (int32 i = 0; i < NumNewItems; ++i)
	// {
	// 	FItemData NewItem;

	// 	NewItems.Add(NewItem);
	// }

	// TArray<UItemDataObj*> ItemDataObjects;
	// for (const FItemData& ItemData : NewItems)
	// {
	// 	UItemDataObj* DataObj = NewObject<UItemDataObj>(this);
	// 	DataObj->Init(ItemData);
	// 	ItemDataObjects.Add(DataObj);
	// }

	// ItemListView->SetListItems(ItemDataObjects);
}
