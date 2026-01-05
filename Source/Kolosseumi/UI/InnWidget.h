// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Messages/MatchEndMessage.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "InnWidget.generated.h"

class UButton;
class UGladiatorInfoWidget;
class UListView;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API UInnWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UListView> GladiatorListView;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UGladiatorInfoWidget> GladiatorInfo;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> HireButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BackButton;

private:
	UFUNCTION()
	void OnHireButtonClicked();
	UFUNCTION()
	void OnBackButtonClicked();
	UFUNCTION()
	void OnGladiatorSelected(UObject* SelectedItem);

	UFUNCTION()
	void OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message);
	FGameplayMessageListenerHandle MatchEndListenerHandle;

	void GenerateNewGladiatorsForHire();
};
