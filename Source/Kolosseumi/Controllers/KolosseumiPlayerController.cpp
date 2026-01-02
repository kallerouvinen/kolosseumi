// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Controllers/KolosseumiPlayerController.h"

AKolosseumiPlayerController::AKolosseumiPlayerController()
{
}

void AKolosseumiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}
