// Copyright Astralwater Interactive. All Rights Reserved.


#include "Components/InteractComponent.h"
#include <InteractableActor.h>
#include <DiverController.h>
#include <DiverCharacter.h>
#include "Actors/Pawns/Divebot.h"
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PCTerminal.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get players location
	FVector PlayerLocation = GetOwner()->GetActorLocation();
	// Head offset
	PlayerLocation.Z += 50.0f;

	GetOwner()->MakeNoise(1.0f, Cast<ADiverCharacter>(GetOwner()), Cast<ADiverCharacter>(GetOwner())->GetActorLocation(), 2000.0f);

	//Cast to player controller
	ADiverController* PlayerController = Cast<ADiverController>(GetOwner()->GetInstigatorController());

	// End location based off player control rotation
	if (PlayerController)
	{
		FVector RaycastEnd = PlayerLocation + (PlayerController->GetControlRotation().Vector() * RaycastDistance);

		// Perform the raycast
		TArray<FHitResult> HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		//Draw debug line
		//DrawDebugLine(GetWorld(), PlayerLocation, RaycastEnd, FColor::Red, false, 1.f, 0, 1.f);
		// If the raycast hits something
		//if (GetWorld()->LineTraceSingleByChannel(HitResult, PlayerLocation, RaycastEnd, ECC_GameTraceChannel1, CollisionParams))
		if (GetWorld()->LineTraceMultiByChannel(HitResult, PlayerLocation, RaycastEnd, ECC_Vehicle, CollisionParams))
		{
			// If the hit actor is an interactable actor
			// Can alternatively do InteractableActor->IsA(AInteractableActor::StaticClass())
			for (FHitResult h : HitResult)
			{
				if (Cast<AInteractableActor>(h.GetActor()))
				{
					if (LastHitActor)
					{
						LastHitActor->DeActivateOutline();
						LastHitActor->DeActivateText();
						LastHitActor = nullptr;
					}
					LastHitActor = Cast<AInteractableActor>(h.GetActor());
					LastHitActor->ActivateOutline();
					LastHitActor->ActivateText();
					return;
				}
			}
			if (LastHitActor)
			{
				LastHitActor->DeActivateOutline();
				LastHitActor->DeActivateText();
				LastHitActor = nullptr;
			}
		}
		if (LastHitActor)
		{
			LastHitActor->DeActivateText();
			LastHitActor->DeActivateOutline();
			LastHitActor = nullptr;
		}
	}
}

void UInteractComponent::Interact(ADiverCharacter* Player)
{
	// Get players location
	FVector PlayerLocation = GetOwner()->GetActorLocation();
	// Head offset
	PlayerLocation.Z += 50.0f;

	GetOwner()->MakeNoise(1.0f, Cast<ADiverCharacter>(GetOwner()), Cast<ADiverCharacter>(GetOwner())->GetActorLocation(), 2000.0f);

	//Cast to player controller
	ADiverController* PlayerController = Cast<ADiverController>(GetOwner()->GetInstigatorController());

	// End location based off player control rotation
	if (PlayerController)
	{
		FVector RaycastEnd = PlayerLocation + (PlayerController->GetControlRotation().Vector() * RaycastDistance);

		// Perform the raycast
		TArray<FHitResult> HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		//Draw debug line
		//DrawDebugLine(GetWorld(), PlayerLocation, RaycastEnd, FColor::Red, false, 1.f, 0, 1.f);
		// If the raycast hits something
		//if (GetWorld()->LineTraceSingleByChannel(HitResult, PlayerLocation, RaycastEnd, ECC_GameTraceChannel1, CollisionParams))
		if (GetWorld()->LineTraceMultiByChannel(HitResult, PlayerLocation, RaycastEnd, ECC_Vehicle, CollisionParams))
		{
			FHitResult myHit;
			// If the hit actor is an interactable actor
			// Can alternatively do InteractableActor->IsA(AInteractableActor::StaticClass())
			for (FHitResult h : HitResult)
			{
				if (h.GetActor()->ActorHasTag("Interactable"))
				{
					myHit = h;
					break;
				}
			}

			if (myHit.GetActor())
			{
				if (myHit.GetComponent()->ComponentHasTag("InteractHitbox"))
				{
					// Cast the hit actor to an interactable actor
					AInteractableActor* InteractableActor1 = Cast<AInteractableActor>(myHit.GetActor());

					if (InteractableActor1)
					{
						// If the interactable actor is not locked
						if (!InteractableActor1->bIsLocked)
						{
							// Call the interact function
							InteractableActor1->Interacted(Player);
						}
					}
				}
			}
		}
	}
}

