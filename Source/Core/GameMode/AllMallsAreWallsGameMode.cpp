// Copyright Epic Games, Inc. All Rights Reserved.

#include "AllMallsAreWallsGameMode.h"
#include "../UI/AllMallsAreWallsHUD.h"
#include "../Character/AllMallsAreWallsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAllMallsAreWallsGameMode::AAllMallsAreWallsGameMode()
	: Super()
{
	// use our custom HUD class
	HUDClass = AAllMallsAreWallsHUD::StaticClass();
}
