// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Waypoint.h"
#include "WaypointComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTABYSS_API UWaypointComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWaypointComponent();

	// Waypoint class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	TSubclassOf<class AWaypoint> WaypointClass;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Spawn waypoint
	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	void SpawnWaypoint( FString name, EWaypointType waypoint);

	// Toggle waypoint visibility
	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	void ToggleVisibility(bool bVisible);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	bool bWaypointActive = true;

	// Distance to the player
		float DistanceToPlayer = 0.0f;

	// Waypoint name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
		FString WaypointName = "Default Name";

	// Waypoint type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
		EWaypointType WaypointType = EWaypointType::WT_MainMission;

	// Waypoint actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
		class AWaypoint* StoredWaypoint;

};
