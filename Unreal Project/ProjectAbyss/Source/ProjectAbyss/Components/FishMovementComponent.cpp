// Copyright Astralwater Interactive. All Rights Reserved.


#include "Components/FishMovementComponent.h"
#include "FishMovementComponent.h"
#include "Kismet/KismetMathLibrary.h" 
#include "Components/CapsuleComponent.h" 
#include "Fish.h"
#include "BaseFishAIController.h"
#include "Math/Quat.h" // Include the Quat header for spherical interpolation

// Sets default values for this component's properties
UFishMovementComponent::UFishMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFishMovementComponent::BeginPlay()
{
	// save the player
	Super::BeginPlay();
	MyPawn = Cast<APawn>(GetOwner());

	// start randompoint generation used for testign (To Delete)
	//PickRandomLocation();


	//Sets proper movement type based on the type selected in blueprint (Sets function pointer top desired function/behavior)
	switch (MyMovementType)
	{
	case EMovementType::E_Constant:
		MyMovementFunction = &UFishMovementComponent::MoveConstant;
		break;
	case EMovementType::E_Impulse:
		MyMovementFunction = &UFishMovementComponent::MoveImpulse;
		break;
	case EMovementType::E_Acceleration:
		MyMovementFunction = &UFishMovementComponent::MoveAcceleration;
		break;
	default:
		break;
	}
}


// Called every frame
void UFishMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!bIsSwimming)
		return;

	//Call proper movement function (Constant, accel or impulse)
	(this->*(MyMovementFunction))(DeltaTime);

	//TODO:Delete : Used to visualize target location
	//DrawDebugSphere(GetWorld(), TargetLocation, 10, 12, FColor::Purple, false, -1);

	//TODO: Delete: was used for testing purpuses
	if (!GetWorld()->GetTimerManager().IsTimerActive(Spoopee))
	{
		//PickRandomLocation();
	}
}

void UFishMovementComponent::SetTaregtLocation(FVector Target)
{
	//Sets the target location
	TargetLocation = Target;
}

void UFishMovementComponent::PickRandomLocation()
{
	FVector Unit = UKismetMathLibrary::RandomUnitVector();

	Unit *= 900;

	bIsDecelerating = false;

	Unit += MyPawn->GetActorLocation();

	TargetLocation = Unit;

	GetWorld()->GetTimerManager().SetTimer(Spoopee, this, &UFishMovementComponent::PickRandomLocation, 5, false);
}

void UFishMovementComponent::TransitionStates( bool b )
{
	bIsSwimming = b;
}

void UFishMovementComponent::MoveConstant(float DeltaTime)
{
	// Initilization of important vars
	FVector myLoc = MyPawn->GetActorLocation(); // current loc
	FVector For = MyPawn->GetActorForwardVector(); // forward of fish
	FVector Direction = TargetLocation - MyPawn->GetActorLocation();
	if (TargetLocation.Length() && Direction.Length() > 0.1) //if there is a target location that isnt 0 and we are further then 0.1 units from the target location 
	{
		//Get look at rotation (aka the target rotation)
		Direction.Normalize();

		FRotator LookAtRotation = Direction.Rotation();

		// Get the alpha used in slerp aka slerp propertie, it is a value between 0 and 1, 1 being 100% of end 0 being 100& of start
		float InterpolationFactor = FMath::Clamp(TurnSpeed * DeltaTime, 0.0f, 1.0f);

		FQuat Start = For.ToOrientationQuat();
		FQuat End = LookAtRotation.Quaternion();

		FQuat Result = FQuat::Slerp(Start, End, InterpolationFactor);

		//Sets the rotation to the interpelated value results
		FRotator CurrentRotation = Result.Rotator();
		CurrentRotation.Roll = 0; // Make sure the fish doesnt flip
		MyPawn->SetActorRotation(CurrentRotation);

		// Move fish forward at constant speed
		if(Cast<ABaseFishAIController>(Cast<AFish>(MyPawn)->GetController())->State == EFishStates::Attack)
			MyPawn->SetActorLocation(myLoc + For * Speed * DeltaTime, true);
		else if (Cast<ABaseFishAIController>(Cast<AFish>(MyPawn)->GetController())->State == EFishStates::Patrol)
			MyPawn->SetActorLocation(myLoc + For * PatrolSpeed * DeltaTime, true);
		else if (Cast<ABaseFishAIController>(Cast<AFish>(MyPawn)->GetController())->State == EFishStates::Alert)
			MyPawn->SetActorLocation(myLoc + For * 0 * DeltaTime, true);
		else if (Cast<ABaseFishAIController>(Cast<AFish>(MyPawn)->GetController())->State == EFishStates::Flee)
			MyPawn->SetActorLocation(myLoc + For * (PatrolSpeed * 1.5) * DeltaTime, true);
	}
}

void UFishMovementComponent::MoveAcceleration(float DeltaTime)
{
	// Initilization of important vars
	FVector myLoc = MyPawn->GetActorLocation();
	FVector For = MyPawn->GetActorForwardVector();
	FVector Direction = TargetLocation - MyPawn->GetActorLocation();
	if (TargetLocation.Length()) // if the target point isnt 0,0,0
	{
		float newSpeed = 0;
		float DotProduct = FVector::DotProduct(MyPawn->GetActorForwardVector(), Direction); // used to check if fish is rotating
		float AngleInDegrees = FMath::Acos(DotProduct) * 180.0f / PI; // translate it to usable angles
		if (AngleInDegrees < 0.01) // if not rotating increase speed
		{
			newSpeed = (CurrentSpeed + SpeedAccelerationRate * DeltaTime);
		}
		else //if rotating slowdown fish 
		{
			newSpeed = (CurrentSpeed - SpeedDecelerationRate * DeltaTime);
		}

		//Make sure speed doesnt go over max speed
		CurrentSpeed = FMath::Clamp(newSpeed, 0.0f, MaxSpeed);

		//Same rotation logic as constant
		Direction.Normalize();

		FRotator LookAtRotation = Direction.Rotation();

		float InterpolationFactor = FMath::Clamp(TurnSpeed * DeltaTime, 0.0f, 1.0f);

		FQuat Start = MyPawn->GetActorForwardVector().ToOrientationQuat();
		FQuat End = LookAtRotation.Quaternion();
		FQuat Result;

		if (!bRotationIsTiedToSpeed)
			Result = FQuat::Slerp(Start, End, InterpolationFactor);
		else
		{
			//scale rot speed to 
			InterpolationFactor = FMath::Clamp(InterpolationFactor * (CurrentSpeed / MaxSpeed), MinRotationSpeedFactor, 1);

			Result = FQuat::Slerp(Start, End, InterpolationFactor);
		}

		FRotator CurrentRotation = Result.Rotator();
		CurrentRotation.Roll = 0;
		MyPawn->SetActorRotation(CurrentRotation);

		MyPawn->SetActorLocation(MyPawn->GetActorLocation() + MyPawn->GetActorForwardVector() * CurrentSpeed * DeltaTime, true);
	}
	//TODO: remove (used for debug perposes)
	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Blue, ("Fish Current Speed: " + FString::SanitizeFloat(CurrentSpeed)));

}

void UFishMovementComponent::MoveImpulse(float DeltaTime)
{
	//Rotate (Same logic as other two)
	FVector myLoc = MyPawn->GetActorLocation();
	FVector For = MyPawn->GetActorForwardVector();
	FVector Direction = TargetLocation - MyPawn->GetActorLocation();
	if (TargetLocation.Length() && Direction.Length() > 0.1)
	{
		Direction.Normalize();

		FRotator LookAtRotation = Direction.Rotation();

		float InterpolationFactor = FMath::Clamp(TurnSpeed * DeltaTime, 0.0f, 1.0f);

		FQuat Start = MyPawn->GetActorForwardVector().ToOrientationQuat();
		FQuat End = LookAtRotation.Quaternion();

		FQuat Result = FQuat::Slerp(Start, End, DeltaTime * TurnSpeed);

		FRotator CurrentRotation = Result.Rotator();
		CurrentRotation.Roll = 0;
		MyPawn->SetActorRotation(CurrentRotation);
	}
	//Move

	//Get the world timer here to avoid function calls 
	FTimerManager& tm = GetWorld()->GetTimerManager();

	if (!tm.IsTimerActive(ImpulseHandle)) // If we are not on impulse cool down start an impulse 
	{
		bIsImpulsing = true;
	}
	//impulse logic
	if (bIsImpulsing)
	{
		//lerp the speed to the max speed
		CurrentSpeed = FMath::Lerp(CurrentSpeed, MaxSpeed, DeltaTime * ImpulseAccelerationRate);
		//make sure it never goes over max
		CurrentSpeed = FMath::Clamp(CurrentSpeed, 0, MaxSpeed);
		if (CurrentSpeed >= MaxSpeed - 6) // if the speed is close to the max speed stop accelerating
		{
			GetWorld()->GetTimerManager().SetTimer(ImpulseHandle, ImpulseCoolDownTime, false);
			bIsImpulsing = false; // is no longer speeding up so start slowing down 
		}
	}
	else
	{
		//lerp speed down
		CurrentSpeed = FMath::Lerp(CurrentSpeed, 0, DeltaTime * ImpulseDecelerationRate);
		CurrentSpeed = FMath::Clamp(CurrentSpeed, 0, MaxSpeed);
	}

	//check again to make sure speed isnt going over max
	CurrentSpeed = FMath::Clamp(CurrentSpeed, 0, MaxSpeed);
	//move fish
	MyPawn->SetActorLocation(MyPawn->GetActorLocation() + MyPawn->GetActorForwardVector() * CurrentSpeed * DeltaTime, true);

}

/*
ChangeLog:
-----------------------------
Date:	 	Oct 16 2023
Name:		Maxime Paradis
Summary:	Started Development of fish movement component, got basic constant speed working
-----------------------------
Date:	 	Oct 17 2023
Name:		Maxime Paradis
Summary:	Finished Acceleration movement mode allong with changes to the designer variables
-----------------------------
Date:	 	Oct 18 2023
Name:		Maxime Paradis
Summary:	Finished impulse based movement and clean up of code
-----------------------------
Date:	 	Oct 19 2023
Name:		Maxime Paradis
Summary:	Commented and further orginized code
-----------------------------
Date:	 	Feb 11 2024
Name:		Alexander Mcternan
Summary:	Made the MoveConstant feature use deltaTime
-----------------------------
*/
