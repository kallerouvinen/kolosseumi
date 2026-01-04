// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class KOLOSSEUMI_API AArrow : public AActor
{
	GENERATED_BODY()

public:
	AArrow();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetTarget(AActor* Target) { TargetActor = Target; }
	void SetSource(AActor* Source) { SourceActor = Source; }

protected:
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	// TObjectPtr<USceneComponent> RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	// TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	AActor* SourceActor;
	AActor* TargetActor;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
