// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OnlineGameMode.generated.h"

UCLASS(minimalapi)
class AOnlineGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOnlineGameMode();

	// IGNORE: This was an attempt at swapping between the 2 player controllers
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SwitchController)
	TSubclassOf<APlayerController> DiverController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SwitchController)
	TSubclassOf<APlayerController> DivebotController;


	void DiverToDivebotControllerSwap();*/
};



