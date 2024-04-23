// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/DivebotCameraOverlayWidget.h"
#include "Components/Image.h"

void UDivebotCameraOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDivebotCameraOverlayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UDivebotCameraOverlayWidget::SetBatteryLife(float BatteryLife)
{
	// Set the battery life images based on the battery life
	if (BatteryLife >= 100.0f)
	{
		BatteryLifeImage100->SetVisibility(ESlateVisibility::Visible);
		BatteryLifeImage75->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage50->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage25->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage0->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (BatteryLife >= 75.0f)
	{
		BatteryLifeImage100->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage75->SetVisibility(ESlateVisibility::Visible);
		BatteryLifeImage50->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage25->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage0->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (BatteryLife >= 50.0f)
	{
		BatteryLifeImage100->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage75->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage50->SetVisibility(ESlateVisibility::Visible);
		BatteryLifeImage25->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage0->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (BatteryLife >= 25.0f)
	{
		BatteryLifeImage100->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage75->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage50->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage25->SetVisibility(ESlateVisibility::Visible);
		BatteryLifeImage0->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (BatteryLife >= 0.0f)
	{
		BatteryLifeImage100->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage75->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage50->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage25->SetVisibility(ESlateVisibility::Hidden);
		BatteryLifeImage0->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BatteryLifeImage100->SetVisibility(ESlateVisibility::Hidden);
	}
}
