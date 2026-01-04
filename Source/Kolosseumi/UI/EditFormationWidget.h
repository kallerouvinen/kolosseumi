// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Messages/StartFormationEditingMessage.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "EditFormationWidget.generated.h"

class UButton;

/**
 *
 */
UCLASS()
class KOLOSSEUMI_API UEditFormationWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> StartMatchButton;

private:
	UFUNCTION()
	void OnStartMatchClicked();

	UFUNCTION()
	void OnStartFormationEditing(FGameplayTag Channel, const FStartFormationEditingMessage& Message);
	FGameplayMessageListenerHandle StartFormationEditingListenerHandle;
};
