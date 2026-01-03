// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Controllers/KolosseumiPlayerController.h"
#include "Kolosseumi/Actors/SpawnPoint.h"
#include "Kolosseumi/Pawns/Gladiator.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

#define GRAB_TRACE ECC_GameTraceChannel1

AKolosseumiPlayerController::AKolosseumiPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder(TEXT("/Game/Input/IMC_Default"));
	if (InputMappingContextFinder.Succeeded())
	{
		DefaultMappingContext = InputMappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFinder(TEXT("/Game/Input/IA_Select"));
	if (InputActionFinder.Succeeded())
	{
		SelectAction = InputActionFinder.Object;
	}
}

void AKolosseumiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bEnableMouseOverEvents = true;
	SetShowMouseCursor(true);
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AKolosseumiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &ThisClass::OnSelectStarted);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Completed, this, &ThisClass::OnSelectCompleted);
	}
}

void AKolosseumiPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GrabbedGladiator) return;

	FHitResult HitResult;
	bool bSuccess = GetHitResultUnderCursor(ECollisionChannel::GRAB_TRACE, true, HitResult);

	if (bSuccess)
	{
		FVector TargetLocation = HitResult.Location;
		TargetLocation.Z += 200.0f;
		FVector GladiatorLocation = GrabbedGladiator->GetActorLocation();

		GrabbedGladiator->SetActorLocation(FMath::VInterpTo(GladiatorLocation, TargetLocation, DeltaTime, 10.0f));
	}
}

void AKolosseumiPlayerController::OnSelectStarted()
{
	FHitResult HitResult;
	bool bSuccess = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);

	if (bSuccess)
	{
		if (AGladiator* Gladiator = Cast<AGladiator>(HitResult.GetActor()))
		{
			GrabbedGladiator = Gladiator;
		}
	}
}

void AKolosseumiPlayerController::OnSelectCompleted()
{
	if (!GrabbedGladiator) return;

	FHitResult HitResult;
	bool bSuccess = GetHitResultUnderCursor(ECollisionChannel::GRAB_TRACE, true, HitResult);

	if (bSuccess)
	{
		FVector TargetLocation = HitResult.Location;

		ASpawnPoint* ClosestSpawnPoint = GetClosestUnoccupiedSpawnPointWithinRange(
				GrabbedGladiator->GetFaction(),
				TargetLocation,
				500.0f);

		// TODO: Handle not finding a spawn point
		GrabbedGladiator->SetActorLocation(ClosestSpawnPoint
						? ClosestSpawnPoint->GetActorLocation()
						: TargetLocation);
	}

	GrabbedGladiator = nullptr;
}

ASpawnPoint* AKolosseumiPlayerController::GetClosestUnoccupiedSpawnPointWithinRange(EFaction Faction, const FVector& Location, float Range) const
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), SpawnPoints);

	ASpawnPoint* ClosestSpawnPoint = nullptr;
	float ClosestDistanceSq = TNumericLimits<float>::Max();
	float RangeSq = Range * Range;

	for (AActor* Actor : SpawnPoints)
	{
		if (ASpawnPoint* SpawnPoint = Cast<ASpawnPoint>(Actor))
		{
			if (SpawnPoint->GetFaction() != Faction) continue;
			// TODO: Skip occupied spawn points

			const float DistanceSq = FVector::DistSquared(Location, SpawnPoint->GetActorLocation());
			if (DistanceSq < ClosestDistanceSq && DistanceSq <= RangeSq)
			{
				ClosestDistanceSq = DistanceSq;
				ClosestSpawnPoint = SpawnPoint;
			}
		}
	}

	return ClosestSpawnPoint;
}
