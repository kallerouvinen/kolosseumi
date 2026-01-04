// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
#include "Engine/TargetPoint.h"
#include "SpawnPoint.generated.h"

class UMaterialInterface;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API ASpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	ASpawnPoint();

protected:
	virtual void BeginPlay() override;

public:
	void SetIsVisible(bool bIsVisible);
	void Highlight(bool bHighlight);

	EFaction GetFaction() const { return Faction; }
	void SetFaction(EFaction NewFaction) { Faction = NewFaction; }

	void SetIndex(int32 NewIndex) { Index = NewIndex; }
	int32 GetIndex() const { return Index; }

	bool IsOccupied() const { return bIsOccupied; }
	void SetOccupied(bool bNewIsOccupied) { bIsOccupied = bNewIsOccupied; }

	bool IsAtSidelines() const { return bIsAtSidelines; }
	void SetIsAtSidelines(bool bNewIsAtSidelines) { bIsAtSidelines = bNewIsAtSidelines; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnPoint")
	EFaction Faction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnPoint")
	int32 Index;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnPoint")
	bool bIsOccupied = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnPoint")
	bool bIsAtSidelines = false;

	TObjectPtr<UMaterialInterface> DefaultMaterial;
	TObjectPtr<UMaterialInterface> HighlightMaterial;
};
