// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/MainUIWidget.h"
#include "Kolosseumi/KolosseumiGameMode.h"
#include "Components/Button.h"

void UMainUIWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StartNextMatchButton->OnClicked.AddDynamic(this, &ThisClass::OnStartNextMatchClicked);
}

void UMainUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainUIWidget::OnStartNextMatchClicked()
{
	if (AKolosseumiGameMode* GameMode = Cast<AKolosseumiGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->StartNextMatch();
	}
}
