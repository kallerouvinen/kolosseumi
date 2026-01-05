// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Messages/RosterChangedMessage.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTagContainer.h"
#include "RosterInfoWidget.generated.h"

class UButton;
class UGladiatorInfoWidget;
class UListView;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API URosterInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UListView> RosterListView;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UGladiatorInfoWidget> GladiatorInfo;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> FireGladiatorButton;

private:
	UFUNCTION()
	void OnGladiatorSelected(UObject* SelectedItem);
	UFUNCTION()
	void OnFireGladiatorClicked();

	UFUNCTION()
	void OnRosterChanged(FGameplayTag Channel, const FRosterChangedMessage& Message);
	FGameplayMessageListenerHandle RosterChangedListenerHandle;

	void OnRosterChanged(const FRosterData& Roster);
};
