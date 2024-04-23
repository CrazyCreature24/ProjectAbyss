// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MissionSystem/MissionBase.h"
#include "Subsystems/MissionSystem/MissionLogSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AMissionBase::AMissionBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true; //idk what this does exactly
}

void AMissionBase::OnObjectiveIDHeared(FString ObjectiveID)
{
	
	for (auto &i : CurrentObjectiveProgress)
	{
		if (i.ObjectiveID == ObjectiveID)
		{
			//Checks if objective will go over and will deny the incriment if reaching the quest quantity
			FObjectiveDetails ObjectiveDetails = ObjectiveDataByID(ObjectiveID);
			if(i.ObjectiveCount < ObjectiveDetails.Quantity)
			{
				i.ObjectiveCount++;
			}

			if (CheckObjectivesAreComplete())
			{
				MoveToNextStage();
			}

			//Called for ui to update their objectives (called in the MissionTracker blueprint)
			ObjectiveUpdated.Broadcast();
			return;
		}
	}
}

void AMissionBase::SetMissionDetails()
{
	//Get MissionLog
	UMissionLogSubsystem* MissionLog = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>();
	//Mission is set when it is created in the QuestLogSubSystem
	
	//Sets the MissionDetails
	MissionDetails = *MissionLog->MissionTable->FindRow<FMissionDetails>(MissionID, "");

	//Sets the Stage Details at current stage
	CurrentStageDetails = MissionDetails.Stage[CurrentStage];

	//Clears incase we are going after the first stage
	CurrentObjectiveProgress.Empty();

	//sets the objectives in the array structs
	TArray<FObjectiveDetails> Objectives = CurrentStageDetails.Objectives;
	for (auto i : Objectives)
	{
		FMissionObjectiveProgress a = { i.ObjectiveName, i.ObjectiveID, 0 };
		CurrentObjectiveProgress.Add(a);
	}

}

FObjectiveDetails AMissionBase::ObjectiveDataByID(FString ObjectiveID)
{
	TArray<FObjectiveDetails> Objectives = CurrentStageDetails.Objectives;

	for (auto i : Objectives)
	{
		if (i.ObjectiveID == ObjectiveID)
		{
			return i; //Returns the FObjectiveDetails if found. IT SHOULD ALWAYS BE unless I F***ed up.
		}
	}
	
	return Objectives[0]; //this is an error. fix this later/ THIS SHOULD NEVER HAPPEN
}

bool AMissionBase::CheckObjectivesAreComplete()
{

	for (auto i : CurrentObjectiveProgress)
	{
		FObjectiveDetails Objective = ObjectiveDataByID(i.ObjectiveID);

		if (i.ObjectiveCount < Objective.Quantity)
			return false;
	}
	return true;
}

void AMissionBase::MoveToNextStage()
{
	if (CurrentStage < MissionDetails.Stage.Max() - 1)
	{
		//moves to new stage and sets that stage's objectives
		CurrentStage++;
		SetMissionDetails();
	}
	else
	{
		bIsComplete = true;
		//Gets called in list user widget. unsure what it does.
		UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>()->MissionWasCompleted.Broadcast(MissionID);
	}
}


// Called when the game starts or when spawned
void AMissionBase::BeginPlay()
{
	Super::BeginPlay();


	UMissionLogSubsystem* MissionLog = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>();

	//When a objective Id is called, check if this mission has that objective and increment
	MissionLog->ObjectiveIDCalled.AddDynamic(this, &AMissionBase::OnObjectiveIDHeared);

}

// Called every frame
void AMissionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

 void AMissionBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMissionBase, MissionID);
	DOREPLIFETIME(AMissionBase, MissionDetails);
	DOREPLIFETIME(AMissionBase, CurrentStage);
	DOREPLIFETIME(AMissionBase, CurrentStageDetails);
	DOREPLIFETIME(AMissionBase, CurrentObjectiveProgress);
	DOREPLIFETIME(AMissionBase, bIsComplete);
}

