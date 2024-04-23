// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/WaypointWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UWaypointWidget::SetWaypointImage(EWaypointType waypointType)
{
	switch (waypointType)
	{
	case EWaypointType::WT_MainMission:
		WaypointImage->SetBrushFromTexture(MainMissionTexture);
		break;
	case EWaypointType::WT_SideMission:
		WaypointImage->SetBrushFromTexture(SideMissionTexture);
		break;
	case EWaypointType::WT_Boat:
		WaypointImage->SetBrushFromTexture(BoatTexture);
		break;
	case EWaypointType::WT_Cave:
		WaypointImage->SetBrushFromTexture(CaveTexture);
		break;
	default:
		break;
	}
}

void UWaypointWidget::NativeConstruct()
{
}
