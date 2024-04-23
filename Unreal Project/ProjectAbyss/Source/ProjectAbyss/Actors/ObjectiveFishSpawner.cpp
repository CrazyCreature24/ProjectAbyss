// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/ObjectiveFishSpawner.h"
#include "Actors/Pawns/Creature.h"
#include "Subsystems/MissionSystem/MissionLogSubsystem.h"
#include "Actors/MissionSystem/MissionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MissionObjectiveIDComponent.h"
#include "GameInstance_GameSave/ProjectAbyssGameInstance.h"


// Sets default values
AObjectiveFishSpawner::AObjectiveFishSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectiveFishSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	UProjectAbyssGameInstance* GameInstance = Cast<UProjectAbyssGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance)
	{
		GameInstance->MapLoadComplete.AddDynamic(this, &AObjectiveFishSpawner::GetObjectiveIDsFromMissions);
	}
	//GetObjectiveIDsFromMissions(); //testing
	//SpawnFish(); //Testing

}

// Called every frame
void AObjectiveFishSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObjectiveFishSpawner::SpawnFish()
{
	TArray<TSubclassOf<ACreature>> CreaturesToSpawn;

	for (auto& Creature : CreaturesInMap)
	{
		//TArray<FString> CreatureObjectiveIDs = Cast<ACreature>(Creature)->ObjectiveIDsComponent->ObjectiveIDs;
		TArray<FString> CreatureObjectiveIDs = Creature.GetDefaultObject()->ObjectiveIDsComponent->ObjectiveIDs;
		bool CreatureWasSet = false;
			for (auto CreatureObjectiveID : CreatureObjectiveIDs)
			{
				for (auto MissionObjectiveID : ActiveMissionObjectiveIDs)
				{
					if (CreatureObjectiveID == MissionObjectiveID)
					{
						CreaturesToSpawn.AddUnique(Creature);
						CreatureWasSet = true;
						continue;
					}
				}
				if (CreatureWasSet)
					continue;
			}
	}

	for (auto& Creature : CreaturesToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (!SpawnLocations.IsEmpty())
		{
			FVector SpawnLocation = SpawnLocations[FMath::RandRange(0, SpawnLocations.Num()-1)]->GetActorLocation();
			GetWorld()->SpawnActor<ACreature>(Creature, SpawnLocation, FRotator(0,0,0),SpawnParams);
		}
		else
			GetWorld()->SpawnActor<ACreature>(Creature, GetActorLocation(), FRotator(0,0,0));

		//GetWorld()->SpawnActor<ACreature>(GetActorLocation(),);
	}

}

void AObjectiveFishSpawner::GetObjectiveIDsFromMissions()
{
	UMissionLogSubsystem* MissionLog = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>();

	TArray<AMissionBase*> ActiveMissions = MissionLog->CurrentMissions;


	for (auto &mission : ActiveMissions)
	{
		for (auto& ObjectiveDetails : mission->CurrentObjectiveProgress)
		{
			ActiveMissionObjectiveIDs.AddUnique(ObjectiveDetails.ObjectiveID);
		}
	}
	SpawnFish();
}

