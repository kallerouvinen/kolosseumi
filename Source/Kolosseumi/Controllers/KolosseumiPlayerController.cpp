// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Controllers/KolosseumiPlayerController.h"
#include "Kolosseumi/Actors/SpawnPoint.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/QuitGameMessage.h"
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

	static ConstructorHelpers::FObjectFinder<UInputAction> SelectActionFinder(TEXT("/Game/Input/IA_Select"));
	if (SelectActionFinder.Succeeded())
	{
		SelectAction = SelectActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> QuitActionFinder(TEXT("/Game/Input/IA_Quit"));
	if (QuitActionFinder.Succeeded())
	{
		QuitAction = QuitActionFinder.Object;
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

	CacheSpawnPoints(true, EFaction::Player);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	StartFormationEditingListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_StartFormationEditing,
			this,
			&ThisClass::OnStartFormationEditing);
	StartMatchListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_StartMatch,
			this,
			&ThisClass::OnStartMatch);
}

void AKolosseumiPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(StartFormationEditingListenerHandle);
	MessageSubsystem.UnregisterListener(StartMatchListenerHandle);

	Super::EndPlay(EndPlayReason);
}

void AKolosseumiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &ThisClass::OnSelectStarted);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Completed, this, &ThisClass::OnSelectCompleted);
		EnhancedInputComponent->BindAction(QuitAction, ETriggerEvent::Started, this, &ThisClass::OnQuit);
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

		ASpawnPoint* NewClosestSpawnPoint = GetClosestUnoccupiedSpawnPointWithinRange(EFaction::Player, TargetLocation, 2000.f);

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
			if (Gladiator->GetFaction() != EFaction::Player) return;

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
		GrabbedGladiator->SetIsAtSidelines(ClosestSpawnPoint->IsAtSidelines());
		ClosestSpawnPoint->Highlight(false);
	}
	else
	{
		// TODO: Handle not finding a spawn point - return to original position?
	}

	GrabbedGladiator = nullptr;
}

void AKolosseumiPlayerController::OnQuit()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.BroadcastMessage(
			KolosseumiTags::Message_QuitGame,
			FQuitGameMessage());
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

void AKolosseumiPlayerController::OnStartFormationEditing(FGameplayTag Channel, const FStartFormationEditingMessage& Message)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AKolosseumiPlayerController::OnStartMatch(FGameplayTag Channel, const FStartMatchMessage& Message)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(DefaultMappingContext);
	}
}
