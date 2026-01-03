// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "KolosseumiHUD.generated.h"

class UMainUIWidget;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API AKolosseumiHUD : public AHUD
{
	GENERATED_BODY()
public:
	AKolosseumiHUD();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TSubclassOf<UMainUIWidget> MainUIWidgetClass;

	UPROPERTY()
	TObjectPtr<UMainUIWidget> MainUIWidget;
};
