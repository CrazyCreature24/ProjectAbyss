// Copyright Astralwater Interactive. All Rights Reserved.


#include "Components/MissionObjectiveIDComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/MissionSystem/MissionLogSubsystem.h"
#include "Actors/Pawns/Creature.h"

// Sets default values for this component's properties
UMissionObjectiveIDComponent::UMissionObjectiveIDComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UMissionObjectiveIDComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMissionObjectiveIDComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMissionObjectiveIDComponent::BroadcastObjectiveIDs(UObject* WorldContextObject)
{

	//UMissionLogSubsystem* MissionLog = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UMissionLogSubsystem>();
	UMissionLogSubsystem* MissionLog = UGameplayStatics::GetGameInstance(WorldContextObject->GetWorld())->GetSubsystem<UMissionLogSubsystem>();

	if (ObjectiveIDs.IsEmpty()) //Error if there are no quest IDs set.
	{
		GEngine->AddOnScreenDebugMessage( - 1, 5.f, FColor::Magenta, FString("No ID in MissionObjectiveIDComponent"));
		return;
	}
	for (FString ObjectiveID : ObjectiveIDs)
	{
		MissionLog->CallObjectiveID(ObjectiveID);
	}
}

