// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Pawns/Gladiator.h"
#include "Kolosseumi/Actors/Arrow.h"
#include "Kolosseumi/Controllers/GladiatorAIController.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/Messages/GladiatorKnockedOutMessage.h"
#include "Kolosseumi/States/KolosseumiGameState.h"
#include "Kolosseumi/UI/HealthBarWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/GameplayStatics.h"

const TMap<EGladiatorClass, FString> AGladiator::GladiatorClassToMeshPath = {
	{ EGladiatorClass::Barbarian, "/Game/Characters/Meshes/SK_Barbarian.SK_Barbarian" },
	{ EGladiatorClass::Knight, "/Game/Characters/Meshes/SK_Knight.SK_Knight" },
	{ EGladiatorClass::Mage, "/Game/Characters/Meshes/SK_Mage.SK_Mage" },
	{ EGladiatorClass::Ranger, "/Game/Characters/Meshes/SK_Ranger.SK_Ranger" },
	{ EGladiatorClass::Rogue, "/Game/Characters/Meshes/SK_Rogue.SK_Rogue" },
};

AGladiator::AGladiator()
{
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	static ConstructorHelpers::FClassFinder<UHealthBarWidget> HealthBarClassFinder(TEXT("/Game/UI/WBP_HealthBar"));
	if (HealthBarClassFinder.Succeeded())
	{
		HealthBarWidgetComponent->SetWidgetClass(HealthBarClassFinder.Class);
	}
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawAtDesiredSize(true);
	HealthBarWidgetComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<USoundBase> HitSoundFinder(TEXT("/Game/Audio/MS_Hit.MS_Hit"));
	if (HitSoundFinder.Succeeded())
	{
		HitSound = HitSoundFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> DodgeSoundFinder(TEXT("/Game/Audio/MS_Dodge.MS_Dodge"));
	if (DodgeSoundFinder.Succeeded())
	{
		DodgeSound = DodgeSoundFinder.Object;
	}

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	bUseControllerRotationYaw = false;

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

void AGladiator::SetData(const FGladiatorData& Data)
{
	GladiatorData = Data;

	SetMaxHealth(Data.Health);
	SetHealth(Data.Health);

	GetMesh()->SetSkeletalMesh(
			Cast<USkeletalMesh>(StaticLoadObject(
					USkeletalMesh::StaticClass(),
					nullptr,
					*GladiatorClassToMeshPath[Data.Class])));
}

void AGladiator::SetHealth(int32 NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0, MaxHealth);

	if (Health <= 0)
	{
		bIsKnockedOut = true;

		GetCharacterMovement()->StopMovementImmediately();

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

void AGladiator::MeleeAttack(AGladiator* TargetGladiator)
{
	if (bIsAttacking || bIsKnockedOut) return;

	bIsAttacking = true;

	if (TargetGladiator)
	{
		FTimerHandle DamageTimerHandle;
		GetWorldTimerManager().SetTimer(
				DamageTimerHandle,
				[this, TargetGladiator]() {
					int32 DodgeChance = TargetGladiator->GetData().Dodge;
					float DodgeRoll = FMath::FRandRange(0.0f, 100.0f);
					if (DodgeRoll < static_cast<float>(DodgeChance))
					{
						// TODO: Spawn dodge effect
						UGameplayStatics::PlaySound2D(this, DodgeSound, 0.1f);
						return;
					}

					float VarianceCoefficient = FMath::FRandRange(0.8f, 1.2f);
					float DamageAmount = GladiatorData.AttackDamage * VarianceCoefficient;
					// TODO: Spawn damage effect
					UGameplayStatics::PlaySound2D(this, HitSound, 0.1f);
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

void AGladiator::RangedAttack(AGladiator* TargetGladiator)
{
	if (bIsAttacking || bIsKnockedOut) return;

	bIsAttacking = true;

	if (TargetGladiator)
	{
		FTimerHandle DamageTimerHandle;
		GetWorldTimerManager().SetTimer(
				DamageTimerHandle,
				[this, TargetGladiator]() {
					FTransform SpawnTransform;
					SpawnTransform.SetLocation(GetActorLocation());
					SpawnTransform.SetRotation(
							(TargetGladiator->GetActorLocation() - GetActorLocation()).Rotation().Quaternion());

					AArrow* SpawnedArrow = GetWorld()->SpawnActorDeferred<AArrow>(
							AArrow::StaticClass(), SpawnTransform);
					SpawnedArrow->SetTarget(TargetGladiator);
					SpawnedArrow->SetSource(this);
					SpawnedArrow->FinishSpawning(SpawnTransform);

					GetMesh()->IgnoreActorWhenMoving(SpawnedArrow, true);
					GetCapsuleComponent()->IgnoreActorWhenMoving(SpawnedArrow, true);
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
