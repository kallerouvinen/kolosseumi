// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Managers/OpponentTeamManager.h"
#include "Kolosseumi/Libraries/Statics.h"

FRosterData AOpponentTeamManager::GenerateOpponentRoster(int32 TargetGoldValue) const
{
	TArray<FGladiatorData> NewGladiators;
	int32 TotalSalary = 0;
	int32 LastAddedGladiatorSalary = 0;

	while (TotalSalary < TargetGoldValue)
	{
		FGladiatorData NewGladiator = UStatics::GenerateGladiatorData();
		TotalSalary += NewGladiator.Salary;
		LastAddedGladiatorSalary = NewGladiator.Salary;
		NewGladiators.Add(NewGladiator);
	}

	float WeightRoll = FMath::RandRange(TotalSalary - LastAddedGladiatorSalary, TotalSalary);
	bool bShouldRemoveGladiator = WeightRoll > TargetGoldValue;

	if (bShouldRemoveGladiator && NewGladiators.Num() > 1)
	{
		NewGladiators.Pop();
	}

	FRosterData RosterData;

	// TODO: Generate positions more intelligently based on gladiator class
	// Front row: 6-12
	// Middle row 13-19
	// Back row: 20-26

	for (const FGladiatorData& GladiatorData : NewGladiators)
	{
		int32 PositionIndex = FMath::RandRange(6, 26);
		while (RosterData.Gladiators.Contains(PositionIndex))
		{
			PositionIndex = FMath::RandRange(6, 26);
		}
		RosterData.Gladiators.Add(PositionIndex, GladiatorData);
	}

	return RosterData;
}
