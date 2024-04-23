// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/CompassWidget.h"
#include "Components/Image.h"



void UCompassWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CompassMaterialInstance = UMaterialInstanceDynamic::Create(CompassMaterial, this);
	CompassMaterialInstance->SetScalarParameterValue(FName(TEXT("Offset")), 0.0f);
	CompassImage->SetBrushFromMaterial(CompassMaterialInstance);
}

void UCompassWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//Get the player controller
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	CompassMaterialInstance->SetScalarParameterValue(FName(TEXT("Offset")), PlayerController->GetControlRotation().Yaw/360);
}


