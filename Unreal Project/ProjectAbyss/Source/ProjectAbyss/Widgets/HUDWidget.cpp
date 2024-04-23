// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/HUDWidget.h"
#include "Actors/Pawns/DiverCharacter.h"
#include "Controllers/DiverController.h"
#include "HealthStateComponent.h"


void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Player = Cast<ADiverController>(GetOwningPlayer());

	if (Cast<ADiverCharacter>(Player->GetPawn()))
	{
		Cast<ADiverCharacter>(Player->GetPawn())->myCrakedVisorWdiget = this;
	}

	ActiveHealthState->SetBrushFromTexture(HealthState0);

	if (HealthState0)
		HealthStates.Add(HealthState0);
	if (HealthState1)
		HealthStates.Add(HealthState1);
	if (HealthState2)
		HealthStates.Add(HealthState2);
	if (HealthState3)
		HealthStates.Add(HealthState3);

}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Player == nullptr)
		return;

	if (Player->myPlayer == nullptr)
		return;


	HealthState = Player->myPlayer->HealthComponent->GetNumStates();

	if(HealthState < HealthStates.Num() - 1)
			ActiveHealthState->SetBrushFromTexture(HealthStates[HealthState]);
	else if (HealthState > HealthStates.Num() - 1)
		ActiveHealthState->SetBrushFromTexture(HealthStates.Last());
	
}

int UHUDWidget::GetHeathState()
{
	return HealthState;
}

void UHUDWidget::ToggleFlashlight(bool b)
{
	if (b)
		FlashlightStatus->SetBrushFromTexture(FlashlightOnTexture);
	else
		FlashlightStatus->SetBrushFromTexture(FlashlightOffTexture);
}

void UHUDWidget::SetWidgetVisible(bool b)
{
	if(false)
		ActiveHealthState->SetBrushFromTexture(HealthStates[0]);
}

void UHUDWidget::ToggleWaypointVisibility(bool b)
{
	if (b)
	{
		WaypointStatusOff->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		WaypointStatusOff->SetVisibility(ESlateVisibility::Visible);
	}
}
