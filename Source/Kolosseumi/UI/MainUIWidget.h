// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Messages/ReturnToMainUIMessage.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTagContainer.h"
#include "MainUIWidget.generated.h"

class UButton;

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
	TObjectPtr<UButton> StartNextMatchButton;

private:
	UFUNCTION()
	void OnNextMatchClicked();

	UFUNCTION()
	void OnReturnToMainUI(FGameplayTag Channel, const FReturnToMainUIMessage& Message);
	FGameplayMessageListenerHandle ReturnToMainUIListenerHandle;
};
