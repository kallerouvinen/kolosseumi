// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Pawns/Gladiator.h"
#include "Kolosseumi/Controllers/GladiatorAIController.h"
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

// void AGladiator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
// }
