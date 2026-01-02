// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UProgressBar;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(EFaction InFaction);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;

public:
	void SetHealthPercent(float Percent);
};
