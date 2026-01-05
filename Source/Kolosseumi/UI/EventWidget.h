// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Messages/ReturnToMainUIMessage.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "EventWidget.generated.h"

class UButton;
class UTextBlock;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API UEventWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> DescriptionText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;

private:
	UFUNCTION()
	void OnConfirmClicked();

	UFUNCTION()
	void OnReturnToMainUI(FGameplayTag Channel, const FReturnToMainUIMessage& Message);
	FGameplayMessageListenerHandle ReturnToMainUIListenerHandle;

	TArray<FString> Events;
};
