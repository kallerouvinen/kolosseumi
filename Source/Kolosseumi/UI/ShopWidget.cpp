// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/ShopWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/ReturnToMainUIMessage.h"
#include "Components/Button.h"
#include "GameFramework/GameplayMessageSubsystem.h"

void UShopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BuyButton->OnClicked.AddDynamic(this, &ThisClass::OnBuyButtonClicked);
	BackButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButtonClicked);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MatchEndListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_MatchEnd,
			this,
			&ThisClass::OnMatchEnd);
}

void UShopWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(MatchEndListenerHandle);

	Super::NativeDestruct();
}

void UShopWidget::OnBuyButtonClicked()
{
	// TODO: Disable buy button if no funds available
	// TODO: Disable buy button if no item selected
}

void UShopWidget::OnBackButtonClicked()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_ReturnToMainUI,
			FReturnToMainUIMessage());
}

void UShopWidget::OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message)
{
	// TODO: Refresh item list
	// - Generate new items for sale
}
