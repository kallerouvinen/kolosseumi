// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Pawns/Gladiator.h"
#include "Kolosseumi/Controllers/GladiatorAIController.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/GladiatorKnockedOutMessage.h"
#include "Kolosseumi/States/KolosseumiGameState.h"
#include "Kolosseumi/UI/HealthBarWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"

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

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

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

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MatchEndListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_MatchEnd,
			this,
			&ThisClass::OnMatchEnd);

	OnBeginCursorOver.AddDynamic(this, &ThisClass::OnHoverStart);
	OnEndCursorOver.AddDynamic(this, &ThisClass::OnHoverEnd);
}

void AGladiator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(MatchEndListenerHandle);

	Super::EndPlay(EndPlayReason);
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

		FGladiatorKnockedOutMessage KnockedOutMessage;
		KnockedOutMessage.Gladiator = this;
		UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
		MessageSubsystem.BroadcastMessage(
				KolosseumiTags::Message_GladiatorKnockedOut,
				KnockedOutMessage);
	}

	RefreshHealthBar();
}

void AGladiator::SetMaxHealth(int32 NewMaxHealth)
{
	MaxHealth = FMath::Max(1, NewMaxHealth);
	Health = FMath::Clamp(Health, 0, MaxHealth);

	RefreshHealthBar();
}

void AGladiator::SetIsAtSidelines(bool bNewIsAtSidelines)
{
	bIsAtSidelines = bNewIsAtSidelines;

	HealthBarWidgetComponent->SetVisibility(!bIsAtSidelines);
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

void AGladiator::OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message)
{
	if (Message.WinningFaction == Faction && !bIsKnockedOut)
	{
		bIsCheering = true;
	}
}

void AGladiator::OnHoverStart(AActor* TouchedActor)
{
	// TODO: Use this or remove
}

void AGladiator::OnHoverEnd(AActor* TouchedActor)
{
	// TODO: Use this or remove
}
