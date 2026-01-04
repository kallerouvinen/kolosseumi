// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Actors/SpawnPoint.h"

ASpawnPoint::ASpawnPoint()
{
	SetActorHiddenInGame(false);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultMaterialFinder(TEXT("/Game/Materials/MI_SpawnPoint_Default"));
	if (DefaultMaterialFinder.Succeeded())
	{
		DefaultMaterial = DefaultMaterialFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> HighlightMaterialFinder(TEXT("/Game/Materials/MI_SpawnPoint_Highlight"));
	if (HighlightMaterialFinder.Succeeded())
	{
		HighlightMaterial = HighlightMaterialFinder.Object;
	}

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
		MeshComponent->SetMaterial(0, DefaultMaterial);
	}
	MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -87.f));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);
}

void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	SetIsVisible(Faction == EFaction::Player);
}

void ASpawnPoint::SetIsVisible(bool bIsVisible)
{
	MeshComponent->SetVisibility(bIsVisible);
}

void ASpawnPoint::Highlight(bool bHighlight)
{
	MeshComponent->SetMaterial(0, bHighlight ? HighlightMaterial : DefaultMaterial);
}
