// Copyright Astralwater Interactive. All Rights Reserved.


#include "Components/DivebotMovementComponent.h"
#include "Actors/Pawns/Divebot.h"
#include "Actors/Pawns/DiverCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainGameState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PhysicsVolume.h" 

// Sets default values for this component's properties
UDivebotMovementComponent::UDivebotMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsMovingForward = false;
	bIsMovingBackward = false;
	AIMoveSpeed = 3.0f;
	HoverDistance = 200.0f;
}


// Called when the game starts
void UDivebotMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (!Owner)
	{
		Owner = CastChecked<ADivebot>(GetOwner());
	}
}

void UDivebotMovementComponent::HandleInput(const FInputActionValue& Value)
{
	if (Cast<ADivebot>(GetOwner())->bIsPossessed)
	{
		FVector InputVector;

		InputVector.X = Value.Get<FVector2D>().X;
		InputVector.Y = Value.Get<FVector2D>().Y;

		Move(InputVector);
	}
}

void UDivebotMovementComponent::HandleVerticalInput(int Value)
{
	return;
}

void UDivebotMovementComponent::Move(FVector InputVector)
{
	if (bIsSwiming)
	{
		float speed = InputVector.Y > 0 ? PossessedForwardMoveSpeed : PossessedBackwardMoveSpeed;

		MaxSwimSpeed = speed;
		
		Cast<ADivebot>(GetOwner())->AddMovementInput(Cast<ADivebot>(GetOwner())->FirstPersonCameraComponent->GetForwardVector(), InputVector.Y);
	}
}

void UDivebotMovementComponent::FollowDiver(float DeltaTime)
{
	if (Owner)
	{
		FVector diverLocation = Owner->player->GetActorLocation();

		AController* diversController = Owner->player->Controller;

		if (diversController)
		{
			// Rotates towards divers forward
			FRotator newRot = diversController->GetControlRotation();
			Owner->SetActorRotation(newRot);
		}

		// Hover over the shoulder effect
		FVector direction = (Owner->player->GetActorForwardVector() + Owner->player->GetActorRightVector() + Owner->player->GetActorUpVector()).GetSafeNormal();
		FVector hoverOffset = (direction * HoverDistance);
		hoverOffset *= FVector(-1, -1, 1); // Hover over left shoulder
		FVector desiredLocation = diverLocation + hoverOffset;

		// Move towards diver and hover nearby
		FVector newPos = FVector(0, 0, 0);
		FVector botLocation = Owner->GetActorLocation();
		newPos += FMath::Lerp(botLocation, desiredLocation, AIMoveSpeed * DeltaTime);
		Owner->SetActorLocation(newPos);

		// TODO: Divebot does not hover over the shoulder properly when looking up and down.
		//FVector direction = newRot.RotateVector();   //FVector(newRot.Roll, newRot.Pitch, newRot.Yaw).GetSafeNormal();
	}
}

void UDivebotMovementComponent::GoToShip(float DeltaTime)
{
 //  	FVector Location = boatPoint->GetComponentLocation();

	//Owner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), Location));

	//// Move towards diver and hover nearby
	//FVector newPos;
	//FVector botLocation = Owner->GetActorLocation();
	//newPos += FMath::Lerp(botLocation, Location, AIMoveSpeed * DeltaTime);
	//Owner->SetActorLocation(newPos);

	//if (FVector::Distance(Location, Owner->GetActorLocation()) < 10)
	//{
		AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;
		
		
		if (MyGameState)
		{
			MyGameState->DeSpawnDivebot();
			
		}
	//}
}

void UDivebotMovementComponent::TransitionMovementStates()
{
	if (Cast<ADivebot>(GetOwner())->bIsPossessed)
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
			SetMovementMode(EMovementMode::MOVE_Falling);
			GetPhysicsVolume()->bWaterVolume = false;
			GravityScale = 1;
		}
	}
}

void UDivebotMovementComponent::ChangeEviroment(bool bIsSwimming)
{
	GravityScale = 0;
	SetMovementMode(EMovementMode::MOVE_Swimming);
	GetPhysicsVolume()->bWaterVolume = true;
}
