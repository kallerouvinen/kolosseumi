// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Libraries/EnumLibrary.h"
#include "Kolosseumi/Messages/MatchEndMessage.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTagContainer.h"
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int32 GetHealth() const { return Health; }
	int32 GetMaxHealth() const { return MaxHealth; }
	void SetHealth(int32 NewHealth);
	void SetMaxHealth(int32 NewMaxHealth);

	void SetFaction(EFaction NewFaction) { Faction = NewFaction; }
	EFaction GetFaction() const { return Faction; }

	void SetIsAtSidelines(bool bNewIsAtSidelines);
	bool IsAtSidelines() const { return bIsAtSidelines; }

	UFUNCTION(BlueprintCallable)
	bool IsAttacking() const { return bIsAttacking; }
	UFUNCTION(BlueprintCallable)
	bool IsKnockedOut() const { return bIsKnockedOut; }
	UFUNCTION(BlueprintCallable)
	bool IsCheering() const { return bIsCheering; }

	void Attack(AGladiator* TargetGladiator);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;

private:
	void RefreshHealthBar();

	int32 MaxHealth = 100;
	int32 Health = 100;
	int32 AttackDamage = 20;
	EFaction Faction = EFaction::Player;

	bool bIsAttacking = false;
	float AttackDuration = 1.0f;
	bool bIsKnockedOut = false;
	bool bIsCheering = false;
	bool bIsAtSidelines = false;

	UFUNCTION()
	void OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message);
	FGameplayMessageListenerHandle MatchEndListenerHandle;

	UFUNCTION()
	void OnHoverStart(AActor* TouchedActor);
	UFUNCTION()
	void OnHoverEnd(AActor* TouchedActor);
};
