// Copyright Epic Games, Inc. All Rights Reserved.

#include "FutureGameExerciseGameMode.h"
#include "../FutureGameExerciseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFutureGameExerciseGameMode::AFutureGameExerciseGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
