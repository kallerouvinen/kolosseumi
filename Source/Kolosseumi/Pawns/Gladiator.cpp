// Copyright 2026 Kalle Rouvinen. All Rights Reserved.

#include "Kolosseumi/Pawns/Gladiator.h"

AGladiator::AGladiator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGladiator::BeginPlay()
{
	Super::BeginPlay();
}

void AGladiator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// void AGladiator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
// }
