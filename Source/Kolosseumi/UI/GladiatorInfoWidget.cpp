// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/GladiatorInfoWidget.h"
#include "Kolosseumi/UI/RosterInfo/GladiatorDataObj.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"

void UGladiatorInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Grid->SetVisibility(ESlateVisibility::Hidden);
}

void UGladiatorInfoWidget::SetInfo(UGladiatorDataObj* GladiatorDataObj)
{
	if (GladiatorDataObj)
	{
		NameText->SetText(FText::FromString(GladiatorDataObj->Name));
		ClassText->SetText(StaticEnum<EGladiatorClass>()->GetDisplayNameTextByValue(static_cast<int64>(GladiatorDataObj->Class)));
		HealthText->SetText(FText::AsNumber(GladiatorDataObj->Health));
		AttackDamageText->SetText(FText::AsNumber(GladiatorDataObj->AttackDamage));
		FString DodgeString = FString::Printf(TEXT("%d %%"), GladiatorDataObj->Dodge);
		DodgeText->SetText(FText::FromString(DodgeString));
		FString SalaryString = FString::Printf(TEXT("%d kultaa"), GladiatorDataObj->Salary);
		SalaryText->SetText(FText::FromString(SalaryString));

		Grid->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Grid->SetVisibility(ESlateVisibility::Hidden);
	}
}
