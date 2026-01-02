// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/KolosseumiGameMode.h"
#include "Kolosseumi/Actors/SpawnPoint.h"
#include "Kolosseumi/Controllers/GladiatorAIController.h"
#include "Kolosseumi/Controllers/KolosseumiPlayerController.h"
#include "Kolosseumi/Pawns/CameraPawn.h"
#include "Kolosseumi/Pawns/Gladiator.h"
#include "Kolosseumi/UI/MainUIWidget.h"
#include "Kismet/GameplayStatics.h"

AKolosseumiGameMode::AKolosseumiGameMode()
{
	DefaultPawnClass = ACameraPawn::StaticClass();
	// HUDClass = nullptr;
	PlayerControllerClass = AKolosseumiPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<AGladiator> GladiatorClassFinder(TEXT("/Game/Characters/BP_Gladiator"));
	if (GladiatorClassFinder.Succeeded())
	{
		GladiatorClass = GladiatorClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UMainUIWidget> MainUIWidgetClassFinder(TEXT("/Game/UI/WBP_MainUI"));
	if (MainUIWidgetClassFinder.Succeeded())
	{
		MainUIWidgetClass = MainUIWidgetClassFinder.Class;
	}
}

void AKolosseumiGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (MainUIWidgetClass)
	{
		MainUIWidget = CreateWidget<UMainUIWidget>(GetWorld(), MainUIWidgetClass);
		if (MainUIWidget)
		{
			MainUIWidget->AddToViewport();
		}
	}
}

void AKolosseumiGameMode::StartNextMatch()
{
	if (MainUIWidget)
	{
		MainUIWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	SpawnGladiatorsAtSpawnPoints();
	AssignAIControllersTargets();
}

void AKolosseumiGameMode::SpawnGladiatorsAtSpawnPoints()
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), SpawnPoints);

	for (AActor* Actor : SpawnPoints)
	{
		if (ASpawnPoint* SpawnPoint = Cast<ASpawnPoint>(Actor))
		{
			FTransform SpawnTransform = SpawnPoint->GetActorTransform();

			if (AGladiator* SpawnedGladiator = GetWorld()->SpawnActorDeferred<AGladiator>(
							GladiatorClass,
							SpawnTransform))
			{
				SpawnedGladiator->SetFaction(SpawnPoint->GetFaction());
				SpawnedGladiator->FinishSpawning(SpawnTransform);
			}
		}
	}
}

void AKolosseumiGameMode::AssignAIControllersTargets()
{
	TArray<AActor*> AIControllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGladiatorAIController::StaticClass(), AIControllers);

	for (AActor* Actor : AIControllers)
	{
		if (AGladiatorAIController* AIController = Cast<AGladiatorAIController>(Actor))
		{
			AIController->SetAttackTargetToClosest();
		}
	}
}
