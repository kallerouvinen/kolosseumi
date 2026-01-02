// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UHealthBarWidget::Init(EFaction InFaction)
{
	HealthProgressBar->SetFillColorAndOpacity(InFaction == EFaction::Player
					? FLinearColor::Green
					: FLinearColor::Red);
}

void UHealthBarWidget::SetHealthPercent(float Percent)
{
	HealthProgressBar->SetPercent(Percent);
}
