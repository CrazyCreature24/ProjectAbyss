// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CompassIconWidget.generated.h"

class UImage;
class AWaypoint;
/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UCompassIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//native construct
	virtual void NativeConstruct() override;

	//native tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetCompassIconTexture(UTexture2D* Texture, AWaypoint* waypoint);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* CompassIconImage;

	//Dynamic material instance for the compass
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
		UMaterialInstanceDynamic* CompassMaterialInstance;

	//material for the compass
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
		UMaterialInterface* CompassMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	UTexture2D* CompassIconTexture;

	AWaypoint* ParentWaypoint;
};
