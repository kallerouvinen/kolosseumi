// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/RosterInfo/RosterInfoListItem.h"
#include "Kolosseumi/UI/RosterInfo/GladiatorDataObj.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void URosterInfoListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UGladiatorDataObj* GladiatorDataObj = Cast<UGladiatorDataObj>(ListItemObject))
	{
		NameText->SetText(FText::FromString(GladiatorDataObj->Name));
	}
}

void URosterInfoListItem::NativeOnItemSelectionChanged(bool bIsSelected)
{
	Border->SetBrushColor(bIsSelected
					? FLinearColor::Yellow
					: FLinearColor::Transparent);
}
