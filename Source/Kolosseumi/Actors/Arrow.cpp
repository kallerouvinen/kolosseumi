// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Actors/Arrow.h"
#include "Kolosseumi/Pawns/Gladiator.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AArrow::AArrow()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	CollisionComp->InitBoxExtent(FVector(10.0f, 0.5f, 0.5f));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
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
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->IgnoreActorWhenMoving(SourceActor, true);
	ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
}

void AArrow::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor == TargetActor)
	{
		if (AGladiator* TargetGladiator = Cast<AGladiator>(OtherActor))
		{
			// TODO: Replace this placeholder damage application with actual logic
			float DamageAmount = 20.f;
			TargetGladiator->SetHealth(TargetGladiator->GetHealth() - DamageAmount);
		}

		Destroy();
	}
}
