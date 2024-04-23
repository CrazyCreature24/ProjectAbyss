#include "StressComponent.h"
// Copyright Astralwater Interactive. All Rights Reserved.


#include "Components/StressComponent.h"

// Sets default values for this component's properties
UStressComponent::UStressComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UStressComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	for (int i = 0; i < MyStates.Num() - 1; i++)
	{
		if (MyStates[i].ThershHoldValue > MyStates[i + 1].ThershHoldValue)
		{
			for (int j = 0; j < 40; j++)
			{
				UE_LOG(LogTemp, Display, TEXT("Please Order the states in thershold order. (Smallest to Bigest)"));
			}
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "ERROR: Please Order the states in thershold order. (Smallest to Bigest)");
		}
	}

	if (MyStates.Num() > 4)
	{
		for (int j = 0; j < 40; j++)
		{
			UE_LOG(LogTemp, Display, TEXT("To many stress states! Limit to 4 PLEASE"));
		}
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "ERROR: To many stress states! Limit to 4 PLEASE");
	}
}


// Called every frame
void UStressComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, ("Current Stress Percentage: " + FString::SanitizeFloat(CurrentStressPercentage)));


	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, ("Current Stress State: " + MyStates[CurrentStressStateIndex].MyName));

}

void UStressComponent::ChangeEviroment(bool IsInWater)
{
	if (IsInWater)
	{
		GetWorld()->GetTimerManager().SetTimer(StressTimerHandle, this, &UStressComponent::TickStress, StressGracePeriod, false);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StressTimerHandle);
	}
}

void UStressComponent::TickStress()
{
	if (bIsStressIncreasing)
	{
		float distFromSerface = GetOwner()->GetActorLocation().Z * -1.0f;
		CurrentDebthMultiplier = floor(distFromSerface / DebthMultiplierThreashHold);
		CurrentDebthMultiplier = FMath::Clamp(CurrentDebthMultiplier, 1, 1000);

		CurrentStressPercentage += StressIncreaseRate * CurrentDebthMultiplier;
		CurrentStressPercentage = FMath::Clamp(CurrentStressPercentage, 0, 100);
		if (CurrentStressPercentage > MyStates[CurrentStressStateIndex].ThershHoldValue && MyStates[CurrentStressStateIndex].MyStressState != EStressThreshold::EPanicked)
		{
			CurrentStressStateIndex += 1;
		}

	}

	GetWorld()->GetTimerManager().SetTimer(StressTimerHandle, this, &UStressComponent::TickStress, TickRate, false);
}

bool UStressComponent::SetStressDecrease(float ValuePerSecond)
{
	if (ValuePerSecond > 0)
	{
		bIsStressIncreasing = false;
		if (CurrentStressPercentage - ValuePerSecond > 0)
		{
			CurrentStressPercentage -= ValuePerSecond;
			if (MyStates[CurrentStressStateIndex].MyStressState != EStressThreshold::ECalm && CurrentStressPercentage < MyStates[CurrentStressStateIndex - 1].ThershHoldValue)
				CurrentStressStateIndex--;
			return true;
		}
	}
	else
	{
		bIsStressIncreasing = true;
	}
	return false;
}

/*
ChangeLog:
-----------------------------
Date:	 	Oct 19 2023
Name:		Maxime Paradis
Summary:	Created component with very basic implementation
-----------------------------
Date:	 	Nov 12 2023
Name:		Maxime Paradis
Summary:	Implemented changes to the struct
-----------------------------
Date:	 	nov 13 2023
Name:		Maxime Paradis
Summary:	Worked on the implementation of the decrease of stress via light
-----------------------------
Date:	 	nov 14 2023
Name:		Maxime Paradis
Summary:	Finished stress implementation, debth needs to be updated
-----------------------------
*/