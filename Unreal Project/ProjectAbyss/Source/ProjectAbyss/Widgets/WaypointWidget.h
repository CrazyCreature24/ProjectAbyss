// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Waypoint.h"
#include "WaypointWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UWaypointWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* DistanceText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* WaypointImage;

	// Set waypoint image
	UFUNCTION(BlueprintCallable, Category = "Waypoint")
		void SetWaypointImage(EWaypointType waypointType);

	//Textures for the different waypoint types
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
		UTexture2D* MainMissionTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
		UTexture2D* SideMissionTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
		UTexture2D* BoatTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
		UTexture2D* CaveTexture;

		// Native construct
	virtual void NativeConstruct() override;
	
};
