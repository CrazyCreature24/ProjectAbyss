// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StateTransitionInterface.h"
#include "DiverCharacter.generated.h"

class UHUDWidget;

UCLASS()
class PROJECTABYSS_API ADiverCharacter : public ACharacter, public IStateTransitionInterface
{
	GENERATED_BODY()

	bool isCameraSetup = false;
	bool bIsFirstTick = true;

public:
	// Sets default values for this character's properties
	ADiverCharacter(const FObjectInitializer& ObjectInitializer);

	//Properties:
	class UDiverMovementComponent* MyMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components")
	class UInteractComponent* InteractComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components")
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components")
	class UInventoryComponent* CamInventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components", meta = (AllowPrivateAccess = "true"))
	class UStressComponent* StressComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components", meta = (AllowPrivateAccess = "true"))
	class UVOIPTalker* VOIPTalkerComponent;

	//Flashlight on players head
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight", meta = (AllowPrivateAccess = "true"))
	class USpotLightComponent* FlashlightComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components")
	class UHealthStateComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components")
	class USkeletalMeshComponent* DeathMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// Handles the transition from walking to swimming
	/// </summary>
	void TransitionMovementStates() override;

	void SetToIsSwimming() override;

	/// <summary>
	/// Setup camera for the player
	/// </summary>
	void SetupCamera();

	/// <summary>
	/// Called before respawning, loads the main level if in a cave
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, Category = "PlayerDeath")
	void OnRespawn();

	UFUNCTION(Category = "Sounds")
	void PlayFootsteps();

	void SetIsPossessed(bool b);

	void SwapPlayerToDeathMesh(bool bIsDead);


public:

	//TSubclass of Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components")
	TSubclassOf<class APhotographyCamera> CameraClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* Footsteps;

	UPROPERTY()
	FTimerHandle FootstepIntervalHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class UAudioComponent* BreathingAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* UnderwaterBreathing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* ExitWaterSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* EnterWaterSound;

	// Camera actor reference
	class APhotographyCamera* CameraActor = nullptr;

	bool bSoundDecoyPlaying = false;
	bool bIsSwiming = false;
	bool bIsPossessed = true;

	UHUDWidget* myCrakedVisorWdiget;

	class UAnimSequence* CurrentAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UAnimSequence* RunAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UAnimSequence* IdleLandAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UAnimSequence* TreadWaterAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UAnimSequence* SwimForwardAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UAnimSequence* SwimBackwardAnim;

};
/*
Date:	 	Feb 22 2024
Name :		Alexander McTernan
Summary :	Added underwater breathing sounds
---------------------------- -
Date:	 	March 4th 2024
Name :		Alexander McTernan
Summary :	Added jumping in/out of water sound logic
---------------------------- -
Date:	 	March 16 2024
Name:		Alexander Mcternan
Summary:	Added in player movement animations
-----------------------------
Date:	 	April 4th
Name:		Alexander McTernan
Summary:	Added a Desh Skeletal mesh and added swap player mesh method.
-----------------------------
*/
