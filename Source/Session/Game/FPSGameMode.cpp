// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "Characters/FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
	// Set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/BP_CPlayer"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// Use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}
