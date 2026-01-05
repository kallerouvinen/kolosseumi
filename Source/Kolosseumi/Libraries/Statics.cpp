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
	{ EGladiatorClass::Barbarian, FClassBaseStats(100, 30, 0) },
	{ EGladiatorClass::Knight, FClassBaseStats(200, 10, 0) },
	{ EGladiatorClass::Mage, FClassBaseStats(100, 20, 0) },
	{ EGladiatorClass::Ranger, FClassBaseStats(60, 20, 0) },
	{ EGladiatorClass::Rogue, FClassBaseStats(80, 20, 30) },
};

FGladiatorData UStatics::GenerateGladiatorData()
{
	FGladiatorData NewGladiator;
	NewGladiator.Name = AllGladiatorNames[FMath::RandRange(0, AllGladiatorNames.Num() - 1)];
	EGladiatorClass Class = static_cast<EGladiatorClass>(FMath::RandRange(0, static_cast<int32>(EGladiatorClass::Rogue)));
	NewGladiator.Class = Class;

	int32 ExtraHealth = FMath::RandRange(0, 20);
	int32 ExtraAttack = FMath::RandRange(0, 10);
	int32 ExtraDodge = FMath::RandRange(0, 10);

	NewGladiator.Health = ClassBaseStats[Class].Health + ExtraHealth;
	NewGladiator.AttackDamage = ClassBaseStats[Class].AttackDamage + ExtraAttack;
	NewGladiator.Dodge = ClassBaseStats[Class].Dodge + ExtraDodge;
	NewGladiator.Salary = 5 + (ExtraHealth + ExtraAttack + (ExtraDodge / 2));

	return NewGladiator;
}
