// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Actors/SpawnPointFactory.h"
#include "Kolosseumi/Actors/SpawnPoint.h"
#include "Kismet/GameplayStatics.h"

void ASpawnPointFactory::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	DestroyAllSpawnPoints();
	GenerateSpawnPoints();
}

void ASpawnPointFactory::DestroyAllSpawnPoints()
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), SpawnPoints);

	for (AActor* SpawnPoint : SpawnPoints)
	{
		SpawnPoint->Destroy();
	}
}

void ASpawnPointFactory::GenerateSpawnPoints()
{
	CurrentPlayerSpawnPointIndex = 0;
	CurrentOpponentSpawnPointIndex = 0;

	GeneratePlayerSidelineSpawnPoints();
	GeneratePlayerFieldSpawnPoints();
	GenerateOpponentSidelineSpawnPoints();
	GenerateOpponentFieldSpawnPoints();
}

void ASpawnPointFactory::GeneratePlayerSidelineSpawnPoints()
{
	for (int32 i = 0; i < SidelineSlotNum; ++i)
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.X += PlayerSidelineOffset.X;
		SpawnLocation.Y += PlayerSidelineOffset.Y;
		SpawnLocation.Y += i * SidelineSpacing;
		SpawnLocation.Z += SidelineHeight;
		FRotator SpawnRotation = FRotator::ZeroRotator;

		ASpawnPoint* SpawnPoint = GetWorld()->SpawnActor<ASpawnPoint>(
				SpawnLocation, SpawnRotation);

		if (SpawnPoint)
		{
			SpawnPoint->SetFaction(EFaction::Player);
			SpawnPoint->SetIsAtSidelines(true);
			SpawnPoint->SetIndex(CurrentPlayerSpawnPointIndex);
		}

		CurrentPlayerSpawnPointIndex++;
	}
}

void ASpawnPointFactory::GeneratePlayerFieldSpawnPoints()
{
	for (int32 Row = 0; Row < FieldRows; ++Row)
	{
		for (int32 Col = 0; Col < FieldColumns; ++Col)
		{
			FVector SpawnLocation = GetActorLocation();
			SpawnLocation.X += PlayerFieldOffset;
			SpawnLocation.X += (Row + 0.5f) * (FieldSpawnAreaSizeY / FieldRows) - (FieldSpawnAreaSizeY / 2);
			SpawnLocation.Y += (Col + 0.5f) * (FieldSpawnAreaSizeX / FieldColumns) - (FieldSpawnAreaSizeX / 2);
			SpawnLocation.Z += FieldHeight;
			FRotator SpawnRotation = FRotator::ZeroRotator;

			ASpawnPoint* SpawnPoint = GetWorld()->SpawnActor<ASpawnPoint>(
					SpawnLocation, SpawnRotation);

			if (SpawnPoint)
			{
				SpawnPoint->SetFaction(EFaction::Player);
				SpawnPoint->SetIsAtSidelines(false);
				SpawnPoint->SetIndex(CurrentPlayerSpawnPointIndex);
			}

			CurrentPlayerSpawnPointIndex++;
		}
	}
}

void ASpawnPointFactory::GenerateOpponentSidelineSpawnPoints()
{
	for (int32 i = 0; i < SidelineSlotNum; ++i)
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.X += OpponentSidelineOffset.X;
		SpawnLocation.Y += OpponentSidelineOffset.Y;
		SpawnLocation.Y -= i * SidelineSpacing;
		SpawnLocation.Z += SidelineHeight;
		FRotator SpawnRotation = FRotator(0.f, 180.f, 0.f);

		ASpawnPoint* SpawnPoint = GetWorld()->SpawnActor<ASpawnPoint>(
				SpawnLocation, SpawnRotation);

		if (SpawnPoint)
		{
			SpawnPoint->SetFaction(EFaction::Opponent);
			SpawnPoint->SetIsAtSidelines(true);
			SpawnPoint->SetIndex(CurrentOpponentSpawnPointIndex);
		}

		CurrentOpponentSpawnPointIndex++;
	}
}

void ASpawnPointFactory::GenerateOpponentFieldSpawnPoints()
{
	for (int32 Row = 0; Row < FieldRows; ++Row)
	{
		for (int32 Col = 0; Col < FieldColumns; ++Col)
		{
			FVector SpawnLocation = GetActorLocation();
			SpawnLocation.X += OpponentFieldOffset;
			SpawnLocation.X += (Row + 0.5f) * (FieldSpawnAreaSizeY / FieldRows) - (FieldSpawnAreaSizeY / 2);
			SpawnLocation.Y += (Col + 0.5f) * (FieldSpawnAreaSizeX / FieldColumns) - (FieldSpawnAreaSizeX / 2);
			SpawnLocation.Z += FieldHeight;
			FRotator SpawnRotation = FRotator(0.f, 180.f, 0.f);

			ASpawnPoint* SpawnPoint = GetWorld()->SpawnActor<ASpawnPoint>(
					SpawnLocation, SpawnRotation);

			if (SpawnPoint)
			{
				SpawnPoint->SetFaction(EFaction::Opponent);
				SpawnPoint->SetIsAtSidelines(false);
				SpawnPoint->SetIndex(CurrentOpponentSpawnPointIndex);
			}

			CurrentOpponentSpawnPointIndex++;
		}
	}
}
