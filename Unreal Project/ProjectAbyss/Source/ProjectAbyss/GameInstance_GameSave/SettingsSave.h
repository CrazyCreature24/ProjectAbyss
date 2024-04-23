// Copyright Astralwater Interactive. All Rights Reserved.

//CURRENTLY NOT IN USE
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SettingsSave.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API USettingsSave : public USaveGame
{
	GENERATED_BODY()
	
public:

	/*UPROPERTY()
	EWindowMode::Type WindowMode;*/

	UPROPERTY()
	int ResolutionIndex;

	UPROPERTY()
	FIntPoint Resolution;

	UPROPERTY()
	int GraphicsIndex;

	UPROPERTY()
	bool VSync;

	UPROPERTY()
	float FrameRate;

	UPROPERTY()
	int FrameRateIndex;

	UPROPERTY()
	float Sensitivity;

	UPROPERTY()
	float Brightness;
};
