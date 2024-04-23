// Copyright Astralwater Interactive. All Rights Reserved.


#include "Components/DiverMovementComponent.h"
#include "Actors/Pawns/DiverCharacter.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "HealthStateComponent.h"
#include "GameFramework/PhysicsVolume.h" 

UDiverMovementComponent::UDiverMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bRunPhysicsWithNoController = true;


	BrakingDecelerationSwimming = SlowDownRate;

	MaxSwimSpeed = SwimmingSpeed;

}

void UDiverMovementComponent::HandleInput(const FInputActionValue& Value)
{
	//converts to 3d vector
	FVector InputVector;

	InputVector.X = Value.Get<FVector2D>().X;
	InputVector.Y = Value.Get<FVector2D>().Y;

	Move(InputVector);

	//HandleAnimations(InputVector);
}

void UDiverMovementComponent::HandleVerticalInput(int Value)
{
	//calls  jump or movevertical depending on the p[layers movement state
	ADiverCharacter* father = Cast<ADiverCharacter>(GetOwner());

	MyInputVector.Z = Value;
	if (Value == 1 && !bIsSwiming)
		father->Jump();
}

void UDiverMovementComponent::Move(FVector InputVector)
{
	//stores important vars
	ADiverCharacter* father = Cast<ADiverCharacter>(GetOwner());


	MyInputVector.X = InputVector.X;
	MyInputVector.Y = InputVector.Y;
	if (father)
	{
		if (!bIsSwiming) // Non swiming movement
		{
			father->AddMovementInput(father->GetActorForwardVector(), MyInputVector.Y);
			father->AddMovementInput(father->GetActorRightVector(), MyInputVector.X);
		}
	}

	// Fish only hear player when it moves
	if (MyInputVector.Length() > 0.2f)
	{
		father->MakeNoise(1.0f, father, GetActorLocation(), 2000.0f);
	}

	

}

void UDiverMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OGSpeed = SwimmingSpeed;
	MaxSwimSpeed = SwimmingSpeed;
}



void UDiverMovementComponent::TransitionMovementStates()
{
	bIsSwiming = !bIsSwiming;

	if (bIsSwiming)
	{
		GravityScale = 0;
		SetMovementMode(EMovementMode::MOVE_Swimming);
		GetPhysicsVolume()->bWaterVolume = true;
	}
	else
	{
		SetMovementMode(EMovementMode::MOVE_Walking);
		NewTargetVelocity = FVector(0, 0, 0);
		GetPhysicsVolume()->bWaterVolume = false;
		GravityScale = 1;
	}
}

void UDiverMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsSwiming)
	{
		ADiverCharacter* father = Cast<ADiverCharacter>(GetOwner());

		float forwardMultiplier = (MyInputVector.Y == 1 ? 1 : BackwardsSpeedMultiplier);

		father->AddMovementInput(father->FirstPersonCameraComponent->GetForwardVector(), MyInputVector.Y * SwimmingSpeed * forwardMultiplier);
		father->AddMovementInput(father->GetActorRightVector(), MyInputVector.X * SwimmingSpeed * SidewaysSpeedMultiplier);
		father->AddMovementInput(father->GetActorUpVector(), MyInputVector.Z * VerticalSpeed);

		//debug print speed
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, "Speed: " + FString::SanitizeFloat(SwimmingSpeed));
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Yellow, "FPS: " + FString::SanitizeFloat(1.0f / DeltaTime));

	}
}

void UDiverMovementComponent::HandleAnimations(FVector InputVector)
{
	ADiverCharacter* father = Cast<ADiverCharacter>(GetOwner());

	//Movement animations
	if (father)
	{

		if (bIsSwiming == true)
		{
			//In water
			if (bIsMoving == false)
			{
				// Not moving Tread water animation
				if (father->CurrentAnim != father->TreadWaterAnim)
				{
					father->GetMesh()->SetAnimation(father->TreadWaterAnim);
					father->CurrentAnim = father->TreadWaterAnim;
					father->GetMesh()->Play(true);
				}
			}
			else
			{
				//Moving
				if (InputVector.Y > 0.2f && FMath::Abs(InputVector.Y) > FMath::Abs(InputVector.X))
				{
					// Moving forward
					if (father->CurrentAnim != father->SwimForwardAnim)
					{
						father->GetMesh()->SetAnimation(father->SwimForwardAnim);
						father->CurrentAnim = father->SwimForwardAnim;
						father->GetMesh()->Play(true);
						
					}
				}
				else if (InputVector.Y < -0.2f && FMath::Abs(InputVector.Y) > FMath::Abs(InputVector.X))
				{
					// Moving backward
					if (father->CurrentAnim != father->SwimBackwardAnim)
					{
						father->GetMesh()->SetAnimation(father->SwimBackwardAnim);
						father->CurrentAnim = father->SwimBackwardAnim;
						father->GetMesh()->Play(true);
					}
				}
			}
		}
		else if (bIsSwiming == false)
		{
			// Not in water
			if (bIsMoving == false)
			{
				if (father->CurrentAnim != father->IdleLandAnim)
				{
					// Idle animation
					father->GetMesh()->SetAnimation(father->IdleLandAnim);
					father->CurrentAnim = father->IdleLandAnim;
					father->GetMesh()->Play(true);
				}
			}
			else
			{
				if (father->CurrentAnim != father->RunAnim)
				{
					//Run animation
					father->GetMesh()->SetAnimation(father->RunAnim);
					father->CurrentAnim = father->RunAnim;
					father->GetMesh()->Play(true);
				}
			}

		}

		if (father->CurrentAnim == father->SwimForwardAnim)
		{
			father->GetMesh()->SetRelativeLocation(FVector(-80.0f, 0.0f, -40.0f));
		}
		else
		{
			father->GetMesh()->SetRelativeLocation(FVector(-10.0f, 0.0f, -100.0f));
		}

	}
}

void UDiverMovementComponent::HitSpeed(float NewSpeed)
{
	if (OGSpeed * NewSpeed > SwimmingSpeed)
	{
		SwimmingSpeed = OGSpeed;
		MultiplySpeed(NewSpeed);
		UWorld* world = GetWorld();
		if (world)
		{
			world->GetTimerManager().SetTimer(HitSpeedIncrease, this, &UDiverMovementComponent::ResetSpeed, HitSpeedBoostActiveTime);
		}
	}
}

void UDiverMovementComponent::SetToIsSwimming()
{
	bIsSwiming = true;

	GravityScale = 0;
	SetMovementMode(EMovementMode::MOVE_Swimming);
	GetPhysicsVolume()->bWaterVolume = true;
}

/*
ChangeLog:
-----------------------------
Date:	 	Oct 12 2023
Name:		Maxime Paradis
Summary:	Created movement component and got to where the demo left off
-----------------------------
Date:	 	Oct 15 2023
Name:		Maxime Paradis
Summary:	Updated movement to have vertical inputs and fixed a couple bugs
-----------------------------
Date:	 	Oct 16 2023
Name:		Maxime Paradis
Summary:	Quick bug fix
-----------------------------
Date:	 	Oct 19 2023
Name:		Maxime Paradis
Summary:	Little clean up and commentation and fixed up some vars/inputs
-----------------------------
Date:	 	Oct 25 2023
Name:		Maxime Paradis
Summary:	Implemented feedback from designers
-----------------------------
Date:	 	Nov 1 2023
Name:		Maxime Paradis
Summary:	Fixed Networked
-----------------------------
Date:	 	Jan 30 2024
Name:		Alexander Mcternan
Summary:	Now emits a noise when they move.
-----------------------------
Date:	 	Jan 31 2024
Name:		Maxime Paradis
Summary:	Removed networked, added speed changer
-----------------------------
Date:	 	Jan 30 2024
Name:		Maxime Paradis
Summary:	Updated a couple things
-----------------------------
Date:	 	March 16 2024
Name:		Alexander Mcternan
Summary:	Added in player movement animations
-----------------------------
*/
