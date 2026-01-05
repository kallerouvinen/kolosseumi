// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Libraries/Statics.h"

const TArray<FString> UStatics::AllGladiatorNames = {
	"Hirmar",
	"Isku-Seppo",
	"Iskumies",
	"Kalmarinpoika",
	"Kallo-Pekka",
	"Kallo-Reijo",
	"Kallonkantaja",
	"Kallu",
	"Kanki",
	"Kaatumaton",
	"King Kong",
	"Kivisusi",
	"Kivor",
	"Kuolonhirvi",
	"Leksa",
	"Make",
	"Mikko Murskain",
	"Murhaaja",
	"Murikka",
	"Murikka II",
	"Murskaaja",
	"Mursu-Matti",
	"Raknar",
	"Rauta-Reijo",
	"Rautakarhu",
	"Rautanyrkki",
	"Rautasusi",
	"Rautaukko",
	"Rautnar",
	"Robin Huppu",
	"Seppo",
	"Shemeikka",
	"Torvinen",
	"Ukrom",
	"Veri-Make",
	"Veri-Seppo",
	"Veri-Ukkonen",
	"Verikoira",
	"Vilunki-Petteri",
};

const TMap<EGladiatorClass, FClassBaseStats> UStatics::ClassBaseStats = {
	{ EGladiatorClass::Barbarian, FClassBaseStats(100, 20, 0) },
	{ EGladiatorClass::Knight, FClassBaseStats(120, 10, 0) },
	{ EGladiatorClass::Mage, FClassBaseStats(80, 10, 0) },
	{ EGladiatorClass::Ranger, FClassBaseStats(90, 10, 0) },
	{ EGladiatorClass::Rogue, FClassBaseStats(85, 10, 20) },
};

FGladiatorData UStatics::GenerateGladiatorData()
{
	FGladiatorData NewGladiator;
	NewGladiator.Name = AllGladiatorNames[FMath::RandRange(0, AllGladiatorNames.Num() - 1)];
	EGladiatorClass Class = static_cast<EGladiatorClass>(FMath::RandRange(0, static_cast<int32>(EGladiatorClass::Rogue)));
	NewGladiator.Class = Class;

	int32 ExtraHealth = FMath::RandRange(0, 10);
	int32 ExtraAttack = FMath::RandRange(0, 5);
	int32 ExtraDodge = FMath::RandRange(0, 5);

	NewGladiator.Health = ClassBaseStats[Class].Health + ExtraHealth;
	NewGladiator.AttackDamage = ClassBaseStats[Class].AttackDamage + ExtraAttack;
	NewGladiator.Dodge = ClassBaseStats[Class].Dodge + ExtraDodge;
	// TODO: Add dodge to salary calculation if it gets implemented
	NewGladiator.Salary = 5 + (2 * (ExtraHealth + ExtraAttack + (ExtraDodge / 2)));

	return NewGladiator;
}
