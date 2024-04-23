// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Boat.generated.h"

class UInputAction;
class ATriggerBox;

UCLASS()
class PROJECTABYSS_API ABoat : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoat();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1_Components")
	class UBoxComponent* RootBox;

	// COMPONENTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1_Components")
	class UStaticMeshComponent* BoatMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1_Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1_Components")
	class UCameraComponent* BoatCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1_Components")
	class UBoxComponent* TriggerBox;

	// INPUT
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4_Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4_Input")
	UInputAction* UnPossessBoatAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "4_Input")
	UInputAction* PauseAction;

	// RESPAWN

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1_Components")
	USceneComponent* respawnPoint;

	// Divebot SPAWN

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1_Components")
	USceneComponent* DivebotSpawnPoint;


	ATriggerBox* StoredMapBox;
	FVector MapBoxOrigin;
	FVector MapBoxExtent;

	// BoatSounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* BoatSounds;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Used by the player to respawn
	/// </summary>
	/// <returns>The location of the repsawn point scenecomponent</returns>
	inline FVector GetRespawnPointLocation() { return respawnPoint->GetComponentLocation(); }

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	/// <summary>
	/// Used by the player to respawn
	/// </summary>
	/// <returns>The location of the repsawn point scenecomponent</returns>
	inline USceneComponent* GetDivebotPointLocation() { return DivebotSpawnPoint; }

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// FUNCTIONS

	/// <summary>
	/// Function called by handle input to do math relating to movement
	/// </summary>
	/// <param name="InputVector"> translated vector from the handle input function or any input vector</param>
	void HandleMovement(const struct FInputActionValue& Value);

	/// <summary>
	/// Function called by controller to decelerate boat.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "2_Functions")
	void DecelerateBoat();

	/// <summary>
	/// Function to possess the boat. 
	/// </summary>
	/// <param name="Player"> Player is passed in to see who is currently possessing the boat. Stored in a member variable. </param>
	UFUNCTION(BlueprintCallable, Category = "2_Functions")
	void PossessBoat(ADiverCharacter* Player);

	/// <summary>
	/// Function to unpossess the boat.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "2_Functions")
	void UnPossessBoat();

	/// <summary>
	/// Returns wether the boat is moving or not.
	/// </summary>
	/// <returns>Is the boat moving or not</returns>
	FORCEINLINE bool GetIsBoatMoving() { return bIsBoatMoving; }

	/// <summary>
	/// Pauses the game
	/// </summary>
	void OpenOptions();
	

	// VARIABLES
	bool bIsMoving;
	bool bIsAccelerating;
	bool bIsDecelerating;
	UUserWidget* OptionsWidgetRef;

	// Current speed of the boat
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "3_Variables")
	float CurrentSpeed = 0.0f;

	// Multiplier speed of the boat
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "3_Variables")
	float BoatSpeedMultiplier = 2.0f;

	// Max speed of the boat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3_Variables")
	float MaxSpeed = 5.0f;

	// Acceleration of the boat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3_Variables")
	float Acceleration = 1.5f;

	// Deceleration of the boat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3_Variables")
	float Deceleration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3_Variables")
	float TurnSpeed = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3_Variables")
	float CurrentTurnSpeed = 0.0f;

	//Holds the target speed (Y) and target rotational speed (X)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3_Variables")
	FVector2D TargetVelocity = {0,0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3_Variables")
	bool bIsBoatMoving = false;

	// Dive cage subclass
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3_Variables")
	TSubclassOf<class ADiveCage> DiveCageSubClass;


private:
	ADiverCharacter* CurrentPlayerCharacter;

};
/*
ChangeLog:
-----------------------------
Date:	 	Oct 10 2023
Name:		Austin Morris
Summary:	Added boat base class with basic movement functionality.
-----------------------------
Date:	 	Oct 30 2023
Name:		Austin Morris/Jarod Beach
Summary:	Fixed on possession issue
-----------------------------
Date:	 	Nov 11 2023
Name:		Austin Morris
Summary:	Small changes to boat movement, added dive cage ref and subclass, added boat wheel ref.
-----------------------------
Date:	 	Dec 6 2023
Name:		Alexander McTernan
Summary:	Controller support added.
-----------------------------
Date:	 	March 11th
Name:		Alexander McTernan
Summary:	Added a Box component as the root so the boat now collides with the in game landscape
-----------------------------
*/
