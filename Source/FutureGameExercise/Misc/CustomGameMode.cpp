// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomGameMode.h"
#include "../MainCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACustomGameMode::ACustomGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
