// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectAbyssGameInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMapLoadComplete);

UCLASS()
class PROJECTABYSS_API UProjectAbyssGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	virtual void Init() override;

	//Gets called when a new map is fully loaded
	UFUNCTION()
	virtual void LoadComplete( const float LoadTime, const FString& MapName);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int TotalPhotosTaken = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bGameStarted = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString PreviousMap;

	UPROPERTY(BlueprintAssignable, Category = "MapFinishedEvent")
	FMapLoadComplete MapLoadComplete;

	// Saving settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bInvertedControls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (ClampMin = "0.2", ClampMax = "2.0", UIMin = "0.2", UIMax = "2.0"))
	float MouseSensitivityX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (ClampMin = "0.2", ClampMax = "2.0", UIMin = "0.2", UIMax = "2.0"))
	float MouseSensitivityY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (ClampMin = "0.5", ClampMax = "5.0", UIMin = "0.5", UIMax = "5.0"))
	float BrightnessLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (ClampMin = "0.5", ClampMax = "5.0", UIMin = "0.5", UIMax = "5.0"))
	float VolumeLevel;
};
