// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gladiator.generated.h"

class UWidgetComponent;

UCLASS()
class KOLOSSEUMI_API AGladiator : public ACharacter
{
	GENERATED_BODY()

public:
	AGladiator();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int32 GetHealth() const { return Health; }
	int32 GetMaxHealth() const { return MaxHealth; }
	void SetHealth(int32 NewHealth) { Health = FMath::Clamp(NewHealth, 0, MaxHealth); }
	void SetMaxHealth(int32 NewMaxHealth)
	{
		MaxHealth = FMath::Max(1, NewMaxHealth);
		Health = FMath::Clamp(Health, 0, MaxHealth);
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;

private:
	int32 MaxHealth = 100;
	int32 Health = 60;
};
