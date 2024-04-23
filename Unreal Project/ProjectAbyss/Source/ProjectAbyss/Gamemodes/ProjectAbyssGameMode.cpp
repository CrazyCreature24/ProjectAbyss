// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectAbyssGameMode.h"
#include "UObject/ConstructorHelpers.h"

AProjectAbyssGameMode::AProjectAbyssGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

}
