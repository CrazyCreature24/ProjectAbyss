// Copyright Astralwater Interactive. All Rights Reserved.


#include "Components/WaypointComponent.h"
#include "Waypoint.h"
#include "CompassIconWidget.h"
#include "Waypoint.h"
#include "WaypointWidget.h"
#include "Components/Image.h"

// Sets default values for this component's properties
UWaypointComponent::UWaypointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWaypointComponent::BeginPlay()
{
	Super::BeginPlay();

	//Spawn waypoint
	SpawnWaypoint(WaypointName, WaypointType);
}

void UWaypointComponent::SpawnWaypoint(FString name, EWaypointType waypoint)
{
	//Spawns waypoint actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetOwner()->GetInstigator();
	if (WaypointClass)
	{
		StoredWaypoint = GetWorld()->SpawnActor<AWaypoint>(WaypointClass, GetOwner()->GetActorLocation() + FVector(0, 0, 100), GetOwner()->GetActorRotation(), SpawnParams);
		StoredWaypoint->UpdateData(bWaypointActive, name, waypoint);
	}

}

void UWaypointComponent::ToggleVisibility(bool bVisible)
{
	//Toggles waypoint visibility
	StoredWaypoint->ToggleVisibility(bVisible);
}


// Called every frame
void UWaypointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Sets waypoint to the components owner. (Boat & fish)
	if (StoredWaypoint)
	StoredWaypoint->SetActorLocation(GetOwner()->GetActorLocation());

	// ...
}

