// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Libraries/Statics.h"

const TArray<FString> UStatics::AllGladiatorNames = {
	"Murikka",
	"Raknar",
	"Seppo",
	"Vilunki-Petteri",
};

FGladiatorData UStatics::GenerateGladiatorData()
{
	FGladiatorData NewGladiator;
	NewGladiator.Name = AllGladiatorNames[FMath::RandRange(0, AllGladiatorNames.Num() - 1)];
	NewGladiator.Class = static_cast<EGladiatorClass>(FMath::RandRange(0, static_cast<int32>(EGladiatorClass::Rogue)));

	NewGladiator.Health = FMath::RandRange(80, 120);
	NewGladiator.Strength = FMath::RandRange(10, 20);
	NewGladiator.Agility = FMath::RandRange(10, 20);
	NewGladiator.Mana = FMath::RandRange(5, 15);

	return NewGladiator;
}
