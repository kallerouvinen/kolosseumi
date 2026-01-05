// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Messages/ReturnToMainUIMessage.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTagContainer.h"
#include "MainUIWidget.generated.h"

class UButton;
class UInnWidget;
class UShopWidget;
class UTextBlock;
class UWidgetSwitcher;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API UMainUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> InnButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ShopButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UInnWidget> InnWidget;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UShopWidget> ShopWidget;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoneyText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> StartNextMatchButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> QuitGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> BackgroundMusic;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> OpenDoorSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> NextMatchSound;

private:
	UFUNCTION()
	void OnInnButtonClicked();
	UFUNCTION()
	void OnShopButtonClicked();
	UFUNCTION()
	void OnNextMatchClicked();
	UFUNCTION()
	void OnQuitGameClicked();

	UFUNCTION()
	void OnReturnToMainUI(FGameplayTag Channel, const FReturnToMainUIMessage& Message);
	FGameplayMessageListenerHandle ReturnToMainUIListenerHandle;
	UFUNCTION()
	void OnRosterChanged(FGameplayTag Channel, const FRosterChangedMessage& Message);
	FGameplayMessageListenerHandle RosterChangedListenerHandle;
	UFUNCTION()
	void OnMoneyChanged(FGameplayTag Channel, const FMoneyChangedMessage& Message);
	FGameplayMessageListenerHandle MoneyChangedListenerHandle;

	void UpdateMoneyText(int32 NewAmount);
};
