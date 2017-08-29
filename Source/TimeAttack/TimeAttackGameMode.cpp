// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TimeAttack.h"
#include "TimeAttackGameMode.h"
#include "TimeAttackPawn.h"
#include "TimeAttackHud.h"

ATimeAttackGameMode::ATimeAttackGameMode()
{
	DefaultPawnClass = ATimeAttackPawn::StaticClass();
	HUDClass = ATimeAttackHud::StaticClass();
}
