// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipmentBase.h"
#include "PhotographyCamera.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuestComplete);

class UMissionLogSubsystem;

UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	Normal,
	NightVision,
	ThermalVision, 
	Count
};

UCLASS()
class PROJECTABYSS_API APhotographyCamera : public AEquipmentBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhotographyCamera();

	// COMPONENTS

	// Spring arm component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components")
		class USpringArmComponent* SpringArmComponent;

	// Camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components")
		class UCameraComponent* CameraComponent;
    
	// Photography camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components")
        class UPhotographyComponent* PhotographyCameraComponent;

	// Focus hitbox component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components")
        class UBoxComponent* FocusHitboxComponent;

	// Scene capture component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components")
		class USceneCaptureComponent2D* SceneCaptureComponent;

	// Photo taken sound effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Sound")
		class USoundBase* PhotoTakenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		class UPostProcessComponent* PostProcessNightVision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		class UPostProcessComponent* PostProcessThermalVision;

     
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Begin overlap for hitbox
	UFUNCTION()
	void OnFocusHitboxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

	  // End overlap for hitbox
    UFUNCTION()
	void OnFocusHitboxEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Set focus hitbox collision to enabled
	UFUNCTION(BlueprintCallable, Category = "3_Functions")
		void SetFocusHitboxCollision(bool bIsEnabled);



public:	
	//Override EquipmentBase functions
	virtual void UsePrimary() override;
	virtual void UseSecondary() override;
	virtual void ReleaseSecondary() override;
	virtual void Equip(AActor* NewOwner) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Sets camera to aiming mode
	/// </summary>
	/// <param name="bIsAiming">What to set aiming bool to</param>
	UFUNCTION(BlueprintCallable, Category = "3_Functions")
		void SetAimingMode(bool bIsAiming);

	/// <summary>
	/// Gets if the player is aiming or not
	/// </summary>
	/// <returns> Returns bIsAimingCam </returns>
	UFUNCTION(BlueprintCallable, Category = "3_Functions")
		bool GetAimingMode();

	/// <summary>
	/// Sets if the camera can take a picture
	/// </summary>
	/// <param name="bCanTake"></param>
	UFUNCTION(BlueprintCallable, Category = "3_Functions")
		void SetCanTakePicture(bool bCanTake);

	/// <summary>
	/// Gets if the camera can take a picture
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, Category = "3_Functions")
		bool GetCanTakePicture();

	/// <summary>
	/// Gets the mode of the camera
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, Category = "3_Functions")
		ECameraMode GetCameraMode();

	/// <summary>
	/// Sets the mode of the camera
	/// </summary>
	/// <param name="mode"></param>
	UFUNCTION(BlueprintCallable, Category = "3_Functions")
		void SetCameraMode(ECameraMode mode);

	// Change camera mode
	UFUNCTION(BlueprintCallable, Category = "3_Functions")
		void ChangeCameraMode();
	

	/// <summary>
	/// Updates the fish in focus
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "3_Functions")
		void UpdateFishInFocus();


	/// <summary>
	/// Reset picture cooldown
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "3_Functions")
		void ResetPictureCooldown();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UMissionLogSubsystem> quests;

	UPROPERTY()
	FQuestComplete QuestComplete;

	// Picture cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Picture")
		float PictureCooldown = 3.0f;


	ADiverCharacter* DiverRef;

	class UCameraViewfinder* Viewfinder;
private: 
	// Camera mode
		ECameraMode CameraMode = ECameraMode::Normal;
	
	// Default rotation
		FRotator DefaultRotation;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TSubclassOf<UMissionLogSubsystem> quests;
	// Is player aiming camera
		bool bIsAimingCam = false;

	// Can camera take picture
		bool bCanTakePicture = true;


	TSubclassOf<class UCameraViewfinder> ViewfinderClass;


	FVector DefaultCameraLocation;

	FTimerHandle PictureTimerHandleCooldown;
};

//ADD A TOGGLE TO TURN OFF THE FOCUS.

/*
ChangeLog:
-----------------------------
Date:	 	Oct 18 2023
Name:		Austin Morris
Summary:	Added photography camera class which spawns on player begin play. Most functionality implemented.
-----------------------------
-----------------------------
Date:	 	Oct 20 2023
Name:		Austin Morris
Summary:	Added focus hitbox component to camera, which is used to detect if a fish is in focus. Added functionality to take picture, and added cooldown.
-----------------------------
-----------------------------
Date:	 	Oct 22 2023
Name:		Austin Morris
Summary:	Added zoom in/out functionality with q/e input, and passed in controller to properly set the view target with blend.
-----------------------------
-----------------------------
Date:	 	Oct 30 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	Modified take picture method to use grab the fish in focus and use its QuestIDComponent to broadcast QuestID.
-----------------------------
-----------------------------
Date:	 	Nov 11 2023
Name:		Austin Morris
Summary:	Added post processing attached to camera with night vision and thermal vision. Added functionality to change camera mode.
-----------------------------
Date:	 	Feb 22 2024
Name:		Alexander McTernan
Summary:	Stopped the code from overwritting the PictureTakingSound
-----------------------------
Date : March 14th
Name : Alexander McTernan
Summary : Added Shutter to ViewFinder widget, had to call ActivateShutter on it.
---------------------------- -
*/

