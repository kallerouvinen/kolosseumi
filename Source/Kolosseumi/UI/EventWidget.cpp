// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/UI/EventWidget.h"
#include "Kolosseumi/Libraries/KolosseumiGameplayTags.h"
#include "Kolosseumi/States/KolosseumiPlayerState.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UEventWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConfirmButton->OnClicked.AddDynamic(this, &ThisClass::OnConfirmClicked);

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	ReturnToMainUIListenerHandle = MessageSubsystem.RegisterListener(
			KolosseumiTags::Message_ReturnToMainUI,
			this,
			&ThisClass::OnReturnToMainUI);

	Events = {
		TEXT("Tuntematon gladiaattori kipittää ohitsesi puolikkaassa haarniskassa\nKysyt häneltä, onko hänellä varmasti kaikki varusteet kunnossa.\n'On mulla!', hän vastaa nopeasti ja jatkaa matkaansa."),
		TEXT("Huhut kertovat, että velhot ovat turhia gladiaattoreita.\nJokin on vienyt heidän taikavoimansa kaupunkiin saapuessa."),
		TEXT("Vihreä peikko saapuu huoneesi ovelle. Hän seisoo siinä\nhetken sanomatta mitään, kunnes lopulta nyökkää ja poistuu."),
		TEXT("Yksi gladiaattoreistasi kertoo saaneensa kirjeen kotoaan.\nHän taittelee sen huolellisesti taskuunsa kertomatta enempää."),
		TEXT("Kruunuharju on täynnä turnausväkeä.\nMajatalot ovat täynnä ja hinnat nousseet.\nKukaan ei tunnu pitävän sitä ongelmana."),
		TEXT("Vietät illan sopimusten ja muistiinpanojen parissa.\nKaikkien merkintöjen tarkoitus ei ole enää täysin selvä."),
		TEXT("Uni ei tule. Mielessäsi pyörivät seuraava ottelu\nja täysin epäolennaiset asiat."),
		TEXT("Hetken aikaa kaikki on rauhallista.\nAreenalta ei kuulu ääntäkään."),
		TEXT("Kilpikylän Kovanaamojen joukkue marssii huoneesi ikkunan ohi.\nHeidän valmentajansa vilkaisee sinua nopeasti ja jatkaa matkaa."),
	};
}

void UEventWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(ReturnToMainUIListenerHandle);

	Super::NativeDestruct();
}

void UEventWidget::OnConfirmClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UEventWidget::OnReturnToMainUI(FGameplayTag Channel, const FReturnToMainUIMessage& Message)
{
	if (Message.bTriggerEvent)
	{
		SetVisibility(ESlateVisibility::Visible);

		int32 EventIndex = FMath::RandRange(0, Events.Num() - 1);
		DescriptionText->SetText(FText::FromString(Events[EventIndex]));
	}
}
