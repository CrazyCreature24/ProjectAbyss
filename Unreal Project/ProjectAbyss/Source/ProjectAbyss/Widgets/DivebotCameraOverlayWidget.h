// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DivebotCameraOverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UDivebotCameraOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

	public:	

	// Constructor
	void NativeConstruct() override;

	// Tick 
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetBatteryLife(float BatteryLife);

	// 5 bound widgets for battery life images
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BatteryLifeImage100;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BatteryLifeImage75;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BatteryLifeImage50;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BatteryLifeImage25;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BatteryLifeImage0;

	
	
};
