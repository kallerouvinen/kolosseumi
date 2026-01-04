// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Messages/MatchEndMessage.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "ShopWidget.generated.h"

class UButton;
class UListView;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UListView> ItemListView;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BuyButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BackButton;

private:
	UFUNCTION()
	void OnBuyButtonClicked();
	UFUNCTION()
	void OnBackButtonClicked();
	UFUNCTION()
	void OnItemSelected(UObject* SelectedItem);

	UFUNCTION()
	void OnMatchEnd(FGameplayTag Channel, const FMatchEndMessage& Message);
	FGameplayMessageListenerHandle MatchEndListenerHandle;

	void GenerateNewItemsForSale();
};
