// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Actors/Arrow.h"
#include "Components/BoxComponent.h"

AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	// RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	// RootComponent = RootScene;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	CollisionComp->InitBoxExtent(FVector(10.0f, 0.5f, 0.5f));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->bReturnMaterialOnMove = true;
	RootComponent = CollisionComp;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Game/Equipment/SM_ArrowBow.SM_ArrowBow"));
	if (MeshFinder.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshFinder.Object);
	}
	MeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	// MeshComponent->SetCollisionProfileName(TEXT("Projectile"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	MeshComponent->SetupAttachment(RootComponent);

	// ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	// ProjectileMovement->UpdatedComponent = RootComponent;
	// ProjectileMovement->InitialSpeed = 100.f;
	// ProjectileMovement->MaxSpeed = 100.f;
	// ProjectileMovement->bRotationFollowsVelocity = true;
	// ProjectileMovement->bShouldBounce = false;
	// ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->IgnoreActorWhenMoving(SourceActor, true);
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetActor)
	{
		FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector NewLocation = GetActorLocation() + Direction * 1000.0f * DeltaTime;
		SetActorLocation(NewLocation);

		FRotator NewRotation = Direction.Rotation();
		SetActorRotation(NewRotation);
	}
}

// TODO: Handle collision with the target gladiator and apply damage
// float DamageAmount = AttackDamage + FMath::RandRange(-20, 20);
// TargetGladiator->SetHealth(TargetGladiator->GetHealth() - DamageAmount);

void AArrow::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == TargetActor)
	{
		// TODO: Apply damage to the target gladiator
		Destroy();
	}
}
