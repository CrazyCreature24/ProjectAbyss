// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
//#include "Components/Texture2d.h"
#include "HUDWidget.generated.h"

class ADiverCharacter;

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	class ADiverController* Player;

	UPROPERTY(meta = (BindWidget))
		UImage* ActiveHealthState;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthStates")
	UTexture2D* HealthState0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthStates")
	UTexture2D* HealthState1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthStates")
	UTexture2D* HealthState2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthStates")
	UTexture2D* HealthState3;

	UPROPERTY(meta = (BindWidget))
		UImage* FlashlightStatus;

	UPROPERTY(meta = (BindWidget))
		UImage* WaypointStatusOn;

	UPROPERTY(meta = (BindWidget))
		UImage* WaypointStatusOff;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthStates")
		UTexture2D* FlashlightOnTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthStates")
		UTexture2D* FlashlightOffTexture;

	UFUNCTION(BlueprintCallable)
	int GetHeathState();

	void ToggleFlashlight(bool b);

	void SetWidgetVisible(bool b);

	void ToggleWaypointVisibility(bool b);

	TArray<UTexture2D*> HealthStates;

protected:

	int HealthState;

};


/*
-----------------------------
Date:	 	Feb 2 2024
Name:		Alexander McTernan
Summary:	Health state class created and functional in engine.
-----------------------------

*/
