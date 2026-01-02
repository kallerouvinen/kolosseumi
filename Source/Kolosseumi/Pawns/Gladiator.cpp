// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Pawns/Gladiator.h"
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
}

void AGladiator::BeginPlay()
{
	Super::BeginPlay();

	if (UUserWidget* Widget = HealthBarWidgetComponent->GetWidget())
	{
		if (UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(Widget))
		{
			// TODO: This requires deferred spawning to initialize faction properly in game mode
			// HealthBarWidget->Init(EFaction::Opponent);
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
