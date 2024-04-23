// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "OnlineGameMode.h"
//#include "PuzzlePlatformsCharacter.h"
#include "UObject/ConstructorHelpers.h"
//#include "../../../../Source/ProjectAbyss/Actors/Pawns/DivebotController.h"
//#include "../../../../Source/ProjectAbyss/Actors/Pawns/DiverController.h"

AOnlineGameMode::AOnlineGameMode()
{
    //
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }
}

// IGNORE: This was an attempt at swapping between the 2 player controllers
//void AOnlineGameMode::DiverToDivebotControllerSwap()
//{
//
//	//SwapPlayerControllers(Cast<APlayerController>(DiverController), Cast<APlayerController>(DivebotController));
//
//}
