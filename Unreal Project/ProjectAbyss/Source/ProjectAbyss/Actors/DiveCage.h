// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractableActor.h"
#include "DiveCage.generated.h"

/**
 * 
 */

class USceneCaptureComponent2D;
class USpringArmComponent;
class UTextureRenderTarget2D;

UCLASS()
class PROJECTABYSS_API ADiveCage : public AInteractableActor
{
	GENERATED_BODY()

	public:
		ADiveCage();

		// COMPONENTS

		// Static mesh component for cage door
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components")
			class UStaticMeshComponent* CageDoorMeshComponent;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "2_Components")
			class UBoxComponent* InsideVolume;


	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	
	public:
		// Called every frame
		virtual void Tick(float DeltaTime) override;

		/// <summary>
		/// Virtual function that is called when player interacts with it
		/// </summary>
		/// <param name="Player"> The player that interacted with it.</param>
		virtual void Interacted(ADiverCharacter* Player) override;

		/// <summary>
		/// Returns the distance from dive cage to floor
		/// </summary>
		/// <returns> Returns the distance from dive cage to floor </returns>
		UFUNCTION(BlueprintCallable, Category = "3_Functions")
			float GetDistanceToGround();

		/// <summary>
		/// Sets the distance from dive cage to floor
		/// </summary>
		/// <param name="NewDistance"> The new distance from dive cage to floor </param>
		UFUNCTION(BlueprintCallable, Category = "3_Functions")
			void SetDistanceToGround(float NewDistance);

		/// <summary>
		/// Lowers the dive cage into the water
		/// </summary>
		UFUNCTION(BlueprintCallable, Category = "3_Functions")
			void LowerCage();

		/// <summary>
		/// Raises the dive cage out of the water
		/// </summary>
		UFUNCTION(BlueprintCallable, Category = "3_Functions")
			void RaiseCage();

		/// <summary>
		/// Toggles the door to be open or closed. 
		/// </summary>
		UFUNCTION(BlueprintCallable, Category = "3_Functions")
			void ToggleCageDoor();

		/// <summary>
		/// Function that is called by frame when the door is opened for timer handle
		/// </summary>
		UFUNCTION(BlueprintCallable, Category = "3_Functions")
			void OpenCageDoor();

		/// <summary>
		/// Function that is called by frame when the door is closed for timer handle
		/// </summary>
		UFUNCTION(BlueprintCallable, Category = "3_Functions")
			void CloseCageDoor();


		/// <summary>
		/// Lock/unlock the dive cage door
	    /// </summary>
		UFUNCTION(BlueprintCallable, Category = "3_Functions")
		void ToggleLockCageDoor();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Speed")
        float CageMoveSpeed = 10.0f;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
		int MaxCageDepthMeters = 200;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			USpringArmComponent* SpringArm;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			USceneCaptureComponent2D* Capture;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			UTextureRenderTarget2D* Tex;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<AActor> PlayerSubclass;

		// Boat reference
		class ABoat* myBoat;

		// Is the cage door open
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
			bool bIsDoorOpen = false;

		// Is the cage lower than the boat
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
			bool bIsCageLowered = false;

	private:
		
		// Original location
			FVector OriginalLocation; 
	
			// Distance to ground from cage
			float DistanceToGround;

			// Raycast distance to ground
			UFUNCTION()
				void RaycastToGround();

		// Timer handle for door opening
			FTimerHandle DoorMoveTimerHandle;

			ADiverCharacter* MyPlayer;

};
/*
ChangeLog:
-----------------------------
Date:	 	Oct 18 2023
Name:		Austin Morris
Summary:	Added dive cage base class with basic movement functionality.
-----------------------------
Date:	 	Oct 20 2023
Name:		Austin Morris
Summary: 	Added cage door opening, closing, and distance to ground functionality.
-----------------------------
Date:	 	Nov 9 2023
Name:		Maxime Paradis
Summary: 	Fixed Bug where player was clipping through roof of divecage
-----------------------------
*/
