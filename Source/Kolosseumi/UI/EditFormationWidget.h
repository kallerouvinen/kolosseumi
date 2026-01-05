// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kolosseumi/Messages/StartFormationEditingMessage.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "EditFormationWidget.generated.h"

class UButton;
class USoundBase;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> StartMatchSound;

private:
	UFUNCTION()
	void OnStartMatchClicked();

	UFUNCTION()
	void OnStartFormationEditing(FGameplayTag Channel, const FStartFormationEditingMessage& Message);
	FGameplayMessageListenerHandle StartFormationEditingListenerHandle;
};
