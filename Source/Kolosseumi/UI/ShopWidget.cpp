// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/ShopWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/ReturnToMainUIMessage.h"
#include "Components/Button.h"
#include "GameFramework/GameplayMessageSubsystem.h"

void UShopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BackButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButtonClicked);
}

void UShopWidget::OnBackButtonClicked()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_ReturnToMainUI,
			FReturnToMainUIMessage());
}
