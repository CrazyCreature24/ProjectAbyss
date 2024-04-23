// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MovementInterface.h"
#include "InputActionValue.h"
#include "StateTransitionInterface.h"
#include "DiverMovementComponent.generated.h"

UCLASS()
class PROJECTABYSS_API UDiverMovementComponent : public UCharacterMovementComponent, public IMovementInterface, public IStateTransitionInterface
{
	GENERATED_BODY()

public:
	UDiverMovementComponent();

	//Functions called by the controller vv
	virtual void HandleInput(const FInputActionValue& Value) override;
	virtual void HandleVerticalInput(int Value) override;

	/// <summary>
	/// Function called by handle input to do math relating to movement
	/// </summary>
	/// <param name="InputVector"> translated vector from the handle input function or any input vector</param>
	virtual void Move(FVector InputVector) override;

	virtual void BeginPlay() override;

	/// <summary>
	/// Transitions the players states and sets related properties
	/// </summary>
	virtual void TransitionMovementStates() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void HandleAnimations(FVector InputVector);

	/// <summary>
	/// Sets the players swim speed.
	/// </summary>
	inline void SetSpeed(float NewSpeed) { SwimmingSpeed = NewSpeed; MaxSwimSpeed = SwimmingSpeed; }

	/// <summary>
	/// Adds to speed
	/// </summary>
	inline void AddToSpeed(float NewSpeed) {
		SwimmingSpeed += NewSpeed; MaxSwimSpeed = SwimmingSpeed;
	}

	/// <summary>
	/// Multiply to speed
	/// </summary>
	inline void MultiplySpeed(float NewSpeed) {
		SwimmingSpeed *= NewSpeed; MaxSwimSpeed = SwimmingSpeed;
	}

	void HitSpeed(float NewSpeed);

	/// <summary>
	/// Resets Speed
	/// </summary>
	inline void ResetSpeed() { SwimmingSpeed = OGSpeed; MaxSwimSpeed = SwimmingSpeed; }


	/// <summary>
	/// Gets the players swim speed.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	inline float GetSpeed() { return SwimmingSpeed; }


	void SetToIsSwimming() override;

	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMovingZ = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSwiming = false;

	// EditCondition = "true", EditConditionHides
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true"))
	float SwimmingSpeed = 850;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true"))
	float VerticalSpeed = 500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true"))
	float SlowDownRate = 250;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true"))
	float HitSpeedBoostActiveTime = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "1"))
	float InterperlationValue = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", ClampMin = "0.1", ClampMax = "1"))
	float BackwardsSpeedMultiplier = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", ClampMin = "0.1", ClampMax = "1"))
	float SidewaysSpeedMultiplier = 0.75;


protected:
	
	FTimerHandle EnteredWaterHandle;
	FTimerHandle HitSpeedIncrease;
	FVector MyInputVector;
	FVector NewTargetVelocity;
	float OGSpeed;

};
