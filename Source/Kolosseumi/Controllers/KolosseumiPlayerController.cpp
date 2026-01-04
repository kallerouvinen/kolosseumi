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

	CacheSpawnPoints(true, EFaction::Player);
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

		ASpawnPoint* NewClosestSpawnPoint = GetClosestUnoccupiedSpawnPointWithinRange(EFaction::Player, TargetLocation, 500.f);

		if (NewClosestSpawnPoint)
		{
			if (ClosestSpawnPoint)
			{
				ClosestSpawnPoint->Highlight(false);
			}
			ClosestSpawnPoint = NewClosestSpawnPoint;
			ClosestSpawnPoint->Highlight(true);
		}

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

	if (ClosestSpawnPoint)
	{
		GrabbedGladiator->SetActorLocation(ClosestSpawnPoint->GetActorLocation());
		ClosestSpawnPoint->Highlight(false);
	}
	else
	{
		// TODO: Handle not finding a spawn point - return to original position?
	}

	GrabbedGladiator = nullptr;
}

ASpawnPoint* AKolosseumiPlayerController::GetClosestUnoccupiedSpawnPointWithinRange(EFaction Faction, const FVector& Location, float Range) const
{
	ASpawnPoint* NewClosestSpawnPoint = nullptr;
	float ClosestDistanceSq = TNumericLimits<float>::Max();
	float RangeSq = Range * Range;

	for (TWeakObjectPtr<ASpawnPoint> SpawnPointPtr : CachedSpawnPoints)
	{
		if (ASpawnPoint* SpawnPoint = SpawnPointPtr.Get())
		{
			if (SpawnPoint->IsOccupied()) continue;

			const float DistanceSq = FVector::DistSquared(Location, SpawnPoint->GetActorLocation());
			if (DistanceSq < ClosestDistanceSq && DistanceSq <= RangeSq)
			{
				ClosestDistanceSq = DistanceSq;
				NewClosestSpawnPoint = SpawnPoint;
			}
		}
	}

	return NewClosestSpawnPoint;
}

void AKolosseumiPlayerController::CacheSpawnPoints(bool bFilterByFaction, EFaction Faction)
{
	CachedSpawnPoints.Empty();

	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), SpawnPoints);

	for (AActor* Actor : SpawnPoints)
	{
		if (ASpawnPoint* SpawnPoint = Cast<ASpawnPoint>(Actor))
		{
			if (bFilterByFaction && SpawnPoint->GetFaction() != Faction) continue;

			CachedSpawnPoints.Add(SpawnPoint);
		}
	}
}
