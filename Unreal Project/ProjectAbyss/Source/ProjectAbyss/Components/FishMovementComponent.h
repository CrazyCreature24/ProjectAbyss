// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FishMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EMovementType : uint8 {
	E_Constant				UMETA(DisplayName = "ConstantSpeed"),
	E_Impulse				UMETA(DisplayName = "Impulse"),
	E_Acceleration			UMETA(DisplayName = "AccelerationSpeed"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTABYSS_API UFishMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFishMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector TargetLocation;
	float CurrentSpeed = 0;
	APawn* MyPawn = nullptr;
	APawn* DiverPlayer = nullptr;

	bool bIsSwimming = true;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetTaregtLocation(FVector Target);

	/// Remove
	void PickRandomLocation();

	void TransitionStates( bool b );

	FTimerHandle Spoopee;
	///
	typedef void (UFishMovementComponent::* MovementFunction)(float DeltaTime);
	MovementFunction MyMovementFunction;

	void MoveConstant(float DeltaTime);
	void MoveAcceleration(float DeltaTime);
	void MoveImpulse(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true"))
	EMovementType MyMovementType = EMovementType::E_Constant;
	//Constant Speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", EditCondition = "MyMovementType == EMovementType::E_Constant || MyMovementType == EMovementType::E_Acceleration", EditConditionHides))
		float Speed = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", EditCondition = "MyMovementType == EMovementType::E_Constant || MyMovementType == EMovementType::E_Acceleration", EditConditionHides))
		float PatrolSpeed = 50;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", EditCondition = "MyMovementType == EMovementType::E_Constant || MyMovementType == EMovementType::E_Acceleration", EditConditionHides))
		float TurnSpeed = 5;
	//Accelerate Speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", EditCondition = "MyMovementType == EMovementType::E_Acceleration || MyMovementType == EMovementType::E_Impulse", EditConditionHides))
		float MaxSpeed = 20;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", EditCondition = "MyMovementType == EMovementType::E_Acceleration", EditConditionHides))
		float SpeedAccelerationRate = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", EditCondition = "MyMovementType == EMovementType::E_Acceleration", EditConditionHides))
		float SpeedDecelerationRate = 800;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", EditCondition = "MyMovementType == EMovementType::E_Acceleration", EditConditionHides))
		bool bRotationIsTiedToSpeed = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", EditCondition = "MyMovementType == EMovementType::E_Acceleration && bRotationIsTiedToSpeed == true", EditConditionHides))
		float MinRotationSpeedFactor = 0.03;
		bool bIsDecelerating = false;
	//Impulsed Speed
	FTimerHandle ImpulseHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", EditCondition = "MyMovementType == EMovementType::E_Impulse", EditConditionHides))
		float ImpulseCoolDownTime = 0.85f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", EditCondition = "MyMovementType == EMovementType::E_Impulse", EditConditionHides))
		float ImpulseAccelerationRate = 10;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DesignerVariables", meta = (AllowPrivateAccess = "true", EditCondition = "MyMovementType == EMovementType::E_Impulse", EditConditionHides))
		float ImpulseDecelerationRate = 2;
	bool bIsImpulsing = false;



};
