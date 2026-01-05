// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Messages/MatchEndMessage.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTagContainer.h"
#include "MatchResultsWidget.generated.h"

class UButton;
class UTextBlock;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API UMatchResultsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MatchResultText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoneyRewardText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ReturnToMainUIButton;

private:
	UFUNCTION()
	void OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message);
	FGameplayMessageListenerHandle MatchEndListenerHandle;
	UFUNCTION()
	void OnMoneyChanged(FGameplayTag Channel, const FMoneyChangedMessage& Message);
	FGameplayMessageListenerHandle MoneyChangedListenerHandle;

	UFUNCTION()
	void OnReturnToMainUIClicked();

	float DelayAfterMatchEnd = 3.0f;

	FString GetMatchResultText(EFaction WinningFaction) const;
};
