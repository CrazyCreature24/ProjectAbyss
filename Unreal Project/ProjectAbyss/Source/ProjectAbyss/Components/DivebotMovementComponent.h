// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateTransitionInterface.h"
#include "MovementInterface.h"
#include "InputActionValue.h"
#include "DivebotMovementComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTABYSS_API UDivebotMovementComponent : public UCharacterMovementComponent, public IMovementInterface, public IStateTransitionInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDivebotMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	//Functions called by the controller vv
	virtual void HandleInput(const FInputActionValue& Value) override;
	virtual void HandleVerticalInput(int Value) override;

	/// <summary>
	/// Function called by handle input to do math relating to movement
	/// </summary>
	/// <param name="InputVector"> translated vector from the handle input function or any input vector</param>
	virtual void Move(FVector InputVector) override;

	///<summary> When the divebot is not possessed, this function is called. This allows the divebot to hove over the shoulder of the Diver. </summary>
	virtual void FollowDiver(float DeltaTime);

	///<summary> When the divebot is not possessed and the player is over water, this function is called. This moves the divbot to its despawn point </summary>
	virtual void GoToShip(float DeltaTime);
		
	/// <summary>
	/// Transitions the players states and sets related properties
	/// </summary>
	virtual void TransitionMovementStates() override;

	void ChangeEviroment(bool bIsSwimming);

	// Reference to the Divebot this is attached too.
	class ADivebot* Owner;

	USceneComponent* boatPoint = nullptr;
	
private:

	// AI Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DESIGNERS_Specs|AI|Movement", meta = (AllowPrivateAccess = "true"))
	float HoverDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DESIGNERS_Specs|AI|Movement", meta = (AllowPrivateAccess = "true"))
	float AIMoveSpeed;

	// Possessed Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DESIGNERS_Specs|Possessed|Movement", meta = (AllowPrivateAccess = "true"))
	float PossessedForwardMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DESIGNERS_Specs|Possessed|Movement", meta = (AllowPrivateAccess = "true"))
	float PossessedBackwardMoveSpeed;

	FVector TargetVelocity;
	FVector CurrentVelocity;
	bool bIsMovingForward;
	bool bIsMovingBackward;

	bool bIsSwiming = true;

public:

	//Setters
	FORCEINLINE void SetIsMovingForward(bool value) { bIsMovingForward = value; }
	FORCEINLINE void SetIsMovingBackward(bool value) { bIsMovingBackward = value; }
};

/*
-----------------------------
Date:	 	Oct 30th 2023
Name:		Alexander McTernan
Summary:	Initial creation. Movement implemented in Divebot has been moved over and is functional.
-----------------------------
Date:	 	Nov 2nd 2023
Name:		Alexander McTernan
Summary:	Replicated all the variables in an attempt to make client to server movement replicate. It did not work.
-----------------------------
Date:	 	Nov 7th 2023
Name:		Alexander McTernan
Summary:	Changed the class to inherit from UMovementComponent from UActorComponent. Had to remove Tick to fix bug, but it does not affect logic.
			Addded some comments to clarify some code.
			Added an if check in FollowDiver() that checks if a controller is null.
-----------------------------
*/