// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Libraries/Statics.h"

const TArray<FString> UStatics::AllGladiatorNames = {
	"Hirmar",
	"Isku-Seppo",
	"Iskumies",
	"Jyrä",
	"Kalmarinpoika",
	"Kallo-Reijo",
	"Kallonkantaja",
	"Kallu",
	"Kanki",
	"Kaatumaton",
	"King Kong",
	"Kivisusi",
	"Kivor",
	"Kuolinhärkä",
	"Kuolonhirvi",
	"Leksa",
	"Make",
	"Mikko Murskain",
	"Murhaaja",
	"Murikka",
	"Murikka II",
	"Murskaaja",
	"Mursu-Matti",
	"Pääkallo-Pekka",
	"Pönttö-Juha",
	"Raknar",
	"Rauta-Reijo",
	"Rautakarhu",
	"Rautanyrkki",
	"Rautasusi",
	"Rautaukko",
	"Rautnar",
	"Ripa Rähinä",
	"Robin Lähiö",
	"Rysäys",
	"Seppo",
	"Shemeikka",
	"Torvinen",
	"Ukrom",
	"Vasaranperä",
	"Veri-Make",
	"Veri-Seppo",
	"Veri-Ukkonen",
	"Verikoira",
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
