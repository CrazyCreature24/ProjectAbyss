// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CompassWidget.generated.h"

class UOverlay;
class UImage;
class UMaterialInstanceDynamic;
class UMaterialInterface;
/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UCompassWidget : public UUserWidget
{
	GENERATED_BODY()

	public:

	//native construct
	virtual void NativeConstruct() override;

	//native tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* CompassOverlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* CompassImage;

	//Dynamic material instance for the compass
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	UMaterialInstanceDynamic* CompassMaterialInstance;

	//material for the compass
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	UMaterialInterface* CompassMaterial;

	private:

	
};
