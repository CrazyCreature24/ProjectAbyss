// Copyright Astralwater Interactive. All Rights Reserved.


#include "Subsystems/MissionSystem/MissionLogSubsystem.h"
#include "Actors/MissionSystem/MissionBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance_GameSave/ProjectAbyssSaveGame.h"
#include "MainGameMode.h"




UMissionLogSubsystem::UMissionLogSubsystem()
{
	//Get access to the quest table as soon as possible.
	//MissionTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/QuestSystem/DT_MissionDetails.DT_MissionDetails"));



	//ResetArrays();

}

void UMissionLogSubsystem::AddNewMission(FName MissionID)
{
	//Checks if the mission is not in current or completed arrays. tho prob not needed.
	if (!CurrentActiveMissions.Contains(MissionID) || !CompletedMissions.Contains(MissionID))
	{
		AMissionBase* newMission = GetWorld()->SpawnActor<AMissionBase>();

		//adds to current active quests and makes sure it wont add if its already in there
		CurrentActiveMissions.AddUnique(MissionID);

		//Createing a MissionBase to put into the array
		newMission->MissionID = MissionID;

		//Do this after MissionID has been set. sets the mission details from the table with the matching name and puts it into the actor
		newMission->SetMissionDetails();

		//adds the new missions into the current missions objects array
		CurrentMissions.AddUnique(newMission);

		//removes from available quests so there is no double
		if (GetMissionDetails(MissionID).Type == EMissionType::Main)
		{
			AvailableMainMissions.Remove(MissionID);
		}
		else if (GetMissionDetails(MissionID).Type == EMissionType::Side)
		{
			AvailableSideMissions.Remove(MissionID);
			randomMissionSize--;
		}
	}
}

void UMissionLogSubsystem::CompleteMission(AMissionBase* CompletedMission) //added side quests (self mental note)
{
	FName CompletedMissionID = CompletedMission->MissionID;

	CurrentActiveMissions.Remove(CompletedMissionID);

	//Moves the mission ID either to completed or back to available depending on the mission type
	if (CompletedMission->MissionDetails.Type == EMissionType::Main)
	{
		CompletedMissions.Add(CompletedMissionID);
	}
	else if (CompletedMission->MissionDetails.Type == EMissionType::Side) //recycles the mission back to available side missions
	{
		UnavailableSideMissions.Add(CompletedMissionID);
		randomMissionSize++;
	}

	//Reward the player
	AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->Money += CompletedMission->MissionDetails.MoneyReward;

	//Removes the given mission actor from the current missions array and destroying the actor.
	CurrentMissions.Remove(CompletedMission);
	CompletedMission->Destroy();

	//Broadcast that a mission was completed deligate, mainly to update the mission widget in the PC
	MissionWasCompleted.Broadcast(CompletedMissionID);

	MissionWasSubmitted.Broadcast(CompletedMissionID);
	//updates the mission arrays if things are conditions are met
	CheckPrequisites();
}

bool UMissionLogSubsystem::QueryActiveMissions(FName MissionID)
{
	return CurrentActiveMissions.Contains(MissionID);
}

void UMissionLogSubsystem::TrackMission(FName MissionID)
{
	if (CurrentTrackedMission == MissionID)
		CurrentTrackedMission = TEXT("None");
	else
		CurrentTrackedMission = MissionID;

	//Being used by the MissionTrackerPrototype Blueprint to switch missions in the UI on screen.
	MissionTrackingChange.Broadcast(CurrentTrackedMission);
}

void UMissionLogSubsystem::NewGameMissionSetup() //Sets missions for the start of the game
{
	FMissionDetails* CurrentMissionDetails;

	//If missions dont have prerequisites, they get put into the arrays

	//Switch this to just force the first quest later
	for (int i = 0; i < UnavailableMainMissions.Num(); i++)
	{
		FName CurrentMissionID = UnavailableMainMissions[i];
		CurrentMissionDetails = MissionTable->FindRow<FMissionDetails>(CurrentMissionID, "");
		if (CurrentMissionDetails->PrequisiteMissions.IsEmpty())
		{
			AvailableMainMissions.AddUnique(CurrentMissionID);
			UnavailableMainMissions.Remove(CurrentMissionID);
			i--;
		}
	}

	RandomizeSideMissions();

	//for (int i = 0; i < UnavailableSideMissions.Num(); i++)
	//{
	//	FName CurrentQuestID = UnavailableSideMissions[i];
	//	CurrentMissionDetails = MissionTable->FindRow<FMissionDetails>(CurrentQuestID, "");
	//	if (CurrentMissionDetails->DepthUpgradeRequired == EDepthUpgradeRequired::Upgrade_0)
	//	{
	//		AvailableSideMissions.AddUnique(CurrentQuestID);
	//		UnavailableSideMissions.Remove(CurrentQuestID);
	//		i--;
	//	}
	//}

}

void UMissionLogSubsystem::LoadMissionsFromSave()
{
}

FMissionDetails UMissionLogSubsystem::GetMissionDetails(FName MissionID)
{
	return *MissionTable->FindRow<FMissionDetails>(MissionID, ""); //Mission does not exist if error occures here
}

void UMissionLogSubsystem::CheckPrequisites() //Need to add the side missions
{

	for (int i = 0; i < UnavailableMainMissions.Num(); i++)
	{
		FName CurrentMissionID = UnavailableMainMissions[i];
		FMissionDetails CurrentMissionDetails = *MissionTable->FindRow<FMissionDetails>(CurrentMissionID, "");
		TArray<FName> missionPrerequisites = CurrentMissionDetails.PrequisiteMissions;

		//if the preReq array is empty, go to next itteration. This means something broke
		if (missionPrerequisites.IsEmpty())
			continue;

		//check if the prequisite quests are in the completed quests array.
		bool prerequsitesComplete = true;
		for (FName MPID : missionPrerequisites)
		{
			//If it does not find any prerequisite quest in the completed quest
			if (CompletedMissions.Find(MPID) == INDEX_NONE)
			{
				prerequsitesComplete = false;
				break;
			}
		}
		//adds quest to available quests array while removing it from unavailble array when prequisites are complete
		//Need to add for item completion too
		if (prerequsitesComplete)
		{
			AvailableMainMissions.Add(CurrentMissionID);
			UnavailableMainMissions.RemoveSingle(CurrentMissionID);
			i--;
		}
	}

	//(WIP) Player needs to get depth upgrades to check that for prerequisites 

	RandomizeSideMissions();

	//Not sure this call anything from other classes.
	PrerequisiteMet.Broadcast();
}

void UMissionLogSubsystem::CallObjectiveID(FString ObjectiveID)
{
	//Gets heard by all mission actors
	ObjectiveIDCalled.Broadcast(ObjectiveID);
}

AMissionBase* UMissionLogSubsystem::GetMissionPtr(FName MissionID)
{
	for (auto i : CurrentMissions)
	{
		if (i == nullptr)
		{
			return nullptr; //temp fix for transfer bug.
		}
		if (i->MissionID == MissionID)
		{
			return i;
		}
	}
	return nullptr;
}

void UMissionLogSubsystem::ResetArrays()
{
	//Reseting all arrays
	CurrentActiveMissions.Empty();
	CompletedMissions.Empty();
	UnavailableMainMissions.Empty();
	AvailableMainMissions.Empty();
	UnavailableSideMissions.Empty();
	AvailableSideMissions.Empty();
	CurrentMissions.Empty();

	//Organizing all main and side missions into their unavailable arrays
	TArray<FName> MissionTableNames = MissionTable->GetRowNames();
	for (auto i : MissionTableNames)
	{
		if (GetMissionDetails(i).Type == EMissionType::Main)
		{
			UnavailableMainMissions.Add(i);
		}
		else if (GetMissionDetails(i).Type == EMissionType::Side)
		{
			UnavailableSideMissions.Add(i);
		}
	}

	NewGameMissionSetup(); //more testing purposes (Will likely be moved later) to fix back to menu and then back to game

}

void UMissionLogSubsystem::SaveMissionActors()
{
	UProjectAbyssSaveGame* DataToSave = Cast<UProjectAbyssSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));
	if (DataToSave != nullptr) //Not needed but just in case. IF this breaks somehow. I will cry
	{
		DataToSave->MissionActorSaveDetails.Empty();

		for (auto i : CurrentMissions)
		{
			FMissionBaseActorDetails Data;

			Data.MissionID = i->MissionID;
			Data.MissionDetails = i->MissionDetails;
			Data.CurrentStage = i->CurrentStage;
			Data.CurrentStageDetails = i->CurrentStageDetails;
			Data.CurrentObjectiveProgress = i->CurrentObjectiveProgress;
			Data.bIsComplete = i->bIsComplete;

			DataToSave->MissionActorSaveDetails.Add(Data);

		}


			UGameplayStatics::SaveGameToSlot(DataToSave, "Slot1", 0);
	}
}

void UMissionLogSubsystem::SetTutorial()
{
	AddNewMission("T0001");
	TrackMission("T0001");
}

void UMissionLogSubsystem::LoadTutorialDT()
{
	MissionTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/QuestSystem/DT_MissionDetails.DT_MissionDetails"));
}
void UMissionLogSubsystem::LoadOverworldDT()
{
	MissionTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/QuestSystem/DT_MissionDetails_Overworld.DT_MissionDetails_Overworld"));
}

void UMissionLogSubsystem::RandomizeSideMissions()
{
	/*if (AvailableSideMissions.IsEmpty() && randomMissionSize != 0)
	{
		while (AvailableSideMissions.Num() < 3)
		{
			int randNum = FMath::RandRange(0, UnavailableSideMissions.Num() - 1);

			FName CurrentMissionID = UnavailableSideMissions[randNum];

			UnavailableSideMissions.Remove(CurrentMissionID);
			AvailableSideMissions.AddUnique(CurrentMissionID);

			randomMissionSize = 3;
		}
	}
	else*/
	//{
	if (!UnavailableSideMissions.IsEmpty())
	{
		while (AvailableSideMissions.Num() < randomMissionSize)
		{
			int randNum = FMath::RandRange(0, UnavailableSideMissions.Num() - 1);

			FName CurrentMissionID = UnavailableSideMissions[randNum];

			UnavailableSideMissions.Remove(CurrentMissionID);
			AvailableSideMissions.AddUnique(CurrentMissionID);
		}
	}
	//}
}

void UMissionLogSubsystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const //I have no idea what I'm doing
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(UQuestLogSubsystem, CurrentTrackedQuest);
}