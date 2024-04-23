// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Waypoint.generated.h"

// Waypoint enum
UENUM(BlueprintType)
enum class EWaypointType : uint8
{
	WT_MainMission		UMETA(DisplayName = "Main Mission"),
	WT_SideMission		UMETA(DisplayName = "Side Mission"),
	WT_Boat 			UMETA(DisplayName = "Boat"),
	WT_Cave				UMETA(DisplayName = "Cave"),
};


UCLASS()
class PROJECTABYSS_API AWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaypoint();

	// Hitbox component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	class UBoxComponent* HitboxComponent;

	// Widget component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	class UWidgetComponent* WidgetComponent;

	// Waypoint class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
		TSubclassOf<class UUserWidget> WaypointWidgetClass;

	// Waypoint widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
		class UWaypointWidget* WaypointWidget;

	// Tsubclass of compass icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	TSubclassOf<class UUserWidget> CompassIconClass;

	// Compass icon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	class UCompassIconWidget* CompassIcon;

	// Compass icon widget

	// Toggle waypoint visibility
	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	void ToggleVisibility(bool bVisible);

	// Hitbox overlap begin
	UFUNCTION()
	void OnHitboxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void UpdateDistanceToPlayer();

	void UpdateData(bool bVisible, FString name, EWaypointType waypoint);

	void Test();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bWaypointActive;

	// Distance to the player
	float DistanceToPlayer;

	// Waypoint name
	FString WaypointName;

	// Waypoint type
	EWaypointType WaypointType;

	//Timer handle for spawning 
	FTimerHandle SpawnTimerHandle;

};
