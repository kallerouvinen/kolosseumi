// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Pawns/Gladiator.h"
#include "Kolosseumi/Controllers/GladiatorAIController.h"
#include "Kolosseumi/KolosseumiGameState.h"
#include "Kolosseumi/UI/HealthBarWidget.h"
#include "Components/WidgetComponent.h"

AGladiator::AGladiator()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	static ConstructorHelpers::FClassFinder<UHealthBarWidget> HealthBarClassFinder(TEXT("/Game/UI/WBP_HealthBar"));
	if (HealthBarClassFinder.Succeeded())
	{
		HealthBarWidgetComponent->SetWidgetClass(HealthBarClassFinder.Class);
	}
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawAtDesiredSize(true);
	HealthBarWidgetComponent->SetupAttachment(RootComponent);

	AIControllerClass = AGladiatorAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AGladiator::BeginPlay()
{
	Super::BeginPlay();

	if (UUserWidget* Widget = HealthBarWidgetComponent->GetWidget())
	{
		if (UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(Widget))
		{
			HealthBarWidget->Init(Faction);
			HealthBarWidget->SetHealthPercent(static_cast<float>(Health) / static_cast<float>(MaxHealth));
		}
	}
}

void AGladiator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGladiator::SetHealth(int32 NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0, MaxHealth);

	if (Health <= 0)
	{
		bIsKnockedOut = true;
		// TODO: Stop any ongoing actions and prevent further actions

		UE_LOG(LogTemp, Warning, TEXT("Gladiator %s knocked out!"), *GetName());

		if (AKolosseumiGameState* GameState = Cast<AKolosseumiGameState>(GetWorld()->GetGameState()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Notifying GameState about knockout of %s"), *GetName());
			GameState->OnGladiatorKnockedOut(this);
		}
	}

	RefreshHealthBar();
}

void AGladiator::SetMaxHealth(int32 NewMaxHealth)
{
	MaxHealth = FMath::Max(1, NewMaxHealth);
	Health = FMath::Clamp(Health, 0, MaxHealth);

	RefreshHealthBar();
}

// void AGladiator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
// }

void AGladiator::Attack(AGladiator* TargetGladiator)
{
	if (bIsAttacking || bIsKnockedOut) return;

	bIsAttacking = true;

	if (TargetGladiator)
	{
		FTimerHandle DamageTimerHandle;
		GetWorldTimerManager().SetTimer(
				DamageTimerHandle,
				[this, TargetGladiator]() {
					float DamageAmount = AttackDamage + FMath::RandRange(-20, 20);
					TargetGladiator->SetHealth(TargetGladiator->GetHealth() - DamageAmount);
				},
				0.5f,
				false);
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
			TimerHandle,
			[this]() { bIsAttacking = false; },
			AttackDuration,
			false);
}

void AGladiator::RefreshHealthBar()
{
	if (UUserWidget* Widget = HealthBarWidgetComponent->GetWidget())
	{
		if (UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(Widget))
		{
			HealthBarWidget->SetHealthPercent(static_cast<float>(Health) / static_cast<float>(MaxHealth));
		}
	}
}
