// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "SpawnPointFactory.generated.h"

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API ASpawnPointFactory : public AInfo
{
	GENERATED_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	void DestroyAllSpawnPoints();
	void GenerateSpawnPoints();

	void GeneratePlayerSidelineSpawnPoints();
	void GeneratePlayerFieldSpawnPoints();
	void GenerateOpponentSidelineSpawnPoints();
	void GenerateOpponentFieldSpawnPoints();

	int32 CurrentPlayerSpawnPointIndex = 0;
	int32 CurrentOpponentSpawnPointIndex = 0;

	UPROPERTY(EditAnywhere, Category = "SpawnPointFactory", meta = (AllowPrivateAccess = "true"))
	int32 SidelineSlotNum = 6;
	UPROPERTY(EditAnywhere, Category = "SpawnPointFactory", meta = (AllowPrivateAccess = "true"))
	int32 FieldRows = 3;
	UPROPERTY(EditAnywhere, Category = "SpawnPointFactory", meta = (AllowPrivateAccess = "true"))
	int32 FieldColumns = 7;

	UPROPERTY(EditAnywhere, Category = "SpawnPointFactory", meta = (AllowPrivateAccess = "true"))
	FVector2D PlayerSidelineOffset = FVector2D(-650.f, -700.f);
	UPROPERTY(EditAnywhere, Category = "SpawnPointFactory", meta = (AllowPrivateAccess = "true"))
	FVector2D OpponentSidelineOffset = FVector2D(650.f, 700.f);
	UPROPERTY(EditAnywhere, Category = "SpawnPointFactory", meta = (AllowPrivateAccess = "true"))
	float SidelineSpacing = 200.f;
	UPROPERTY(EditAnywhere, Category = "SpawnPointFactory", meta = (AllowPrivateAccess = "true"))
	float SidelineHeight = 148.f;

	UPROPERTY(EditAnywhere, Category = "SpawnPointFactory", meta = (AllowPrivateAccess = "true"))
	float PlayerFieldOffset = -300.f;
	UPROPERTY(EditAnywhere, Category = "SpawnPointFactory", meta = (AllowPrivateAccess = "true"))
	float OpponentFieldOffset = 300.f;
	UPROPERTY(EditAnywhere, Category = "SpawnPointFactory", meta = (AllowPrivateAccess = "true"))
	float FieldSpawnAreaSizeX = 1400.f;
	UPROPERTY(EditAnywhere, Category = "SpawnPointFactory", meta = (AllowPrivateAccess = "true"))
	float FieldSpawnAreaSizeY = 400.f;
	UPROPERTY(EditAnywhere, Category = "SpawnPointFactory", meta = (AllowPrivateAccess = "true"))
	float FieldHeight = 88.f;
};
