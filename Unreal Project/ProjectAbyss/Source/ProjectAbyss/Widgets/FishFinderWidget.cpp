// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/FishFinderWidget.h"
#include "Components/TextBlock.h"

void UFishFinderWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FishCountText->SetText(FText::FromString("0"));
}

void UFishFinderWidget::SetFishCount(int FishCount)
{
	FishCountText->SetText(FText::FromString(FString::FromInt(FishCount)));
}
