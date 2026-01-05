// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Managers/OpponentTeamManager.h"

AOpponentTeamManager::AOpponentTeamManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Game/Data/DT_OpponentRosters.DT_OpponentRosters"));
	if (DataTableFinder.Succeeded())
	{
		OpponentTeamsTable = DataTableFinder.Object;
	}

	if (!OpponentTeamsTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("OpponentTeamsTable not found!"));
	}
}

FRosterData* AOpponentTeamManager::GetRandomOpponentRoster() const
{
	if (!OpponentTeamsTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("OpponentTeamsTable is null!"));
		return nullptr;
	}

	TArray<FName> RowNames = OpponentTeamsTable->GetRowNames();
	if (RowNames.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("OpponentTeamsTable has no rows!"));
		return nullptr;
	}

	int32 RandomIndex = FMath::RandRange(0, RowNames.Num() - 1);

	FName SelectedRowName = RowNames[RandomIndex];
	FRosterData* RosterData = OpponentTeamsTable->FindRow<FRosterData>(SelectedRowName, TEXT(""));
	return RosterData;
}
