// Copyright Astralwater Interactive. All Rights Reserved.


#include "GameInstance_GameSave/ProjectAbyssGameInstance.h"

#include "Subsystems/MissionSystem/MissionLogSubsystem.h"
#include "Subsystems/SaveLoadSubsystem.h"
#include "Actors/MissionSystem/MissionBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance_GameSave/ProjectAbyssSaveGame.h"


void UProjectAbyssGameInstance::Init()
{
	Super::Init();

	bInvertedControls = false;
	MouseSensitivityX = 1.0f;
	MouseSensitivityY = 1.0f;
	BrightnessLevel = 1.0f;
	VolumeLevel = 1.0f;

	UProjectAbyssSaveGame* DataToSave = Cast<UProjectAbyssSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	if (DataToSave == nullptr)
	{
		return;
	}

	for (auto i : DataToSave->MissionActorSaveDetails)
	{
		AMissionBase* LoadedMission = GetWorld()->SpawnActor<AMissionBase>();

		LoadedMission->MissionID = i.MissionID;
		LoadedMission->MissionDetails = i.MissionDetails;
		LoadedMission->CurrentStage = i.CurrentStage;
		LoadedMission->CurrentStageDetails = i.CurrentStageDetails;
		LoadedMission->CurrentObjectiveProgress = i.CurrentObjectiveProgress;
		LoadedMission->bIsComplete = i.bIsComplete;

	}
}

void UProjectAbyssGameInstance::LoadComplete(const float LoadTime, const FString& MapName) //Fuck this function called after all Actor's begin plays
{
	Super::LoadComplete(LoadTime, MapName);

	UMissionLogSubsystem* MissionLog = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>();
	USaveLoadSubsystem* SaveLoad = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USaveLoadSubsystem>();


	//SaveLoad->LoadMissionActors();
	//MissionLog->MissionTrackingChange.Broadcast(MissionLog->CurrentTrackedMission);

	if (MapName == L"/Game/ProjectAbyss/Maps/TutorialLevel" && bGameStarted == false) //This is to load tutorial specific missions
	{
		SaveLoad->SaveAllData();
		MissionLog->SetTutorial();
		MissionLog->MissionTrackingChange.Broadcast(MissionLog->CurrentTrackedMission);

		//SaveLoad->SaveBoatState();
		//SaveLoad->SavePlayerState();
		bGameStarted = true;
	}
	else if (MapName == L"/Game/ProjectAbyss/Maps/ShallowsCave" && bGameStarted == false)
	{
		SaveLoad->SaveBoatState();
		SaveLoad->SavePlayerState();
		bGameStarted = true;

		MissionLog->LoadOverworldDT();
		MissionLog->AddNewMission("MM00");
		MissionLog->TrackMission("MM00");
	}
	//Just realised i made something redundant... Fix later....
	else if (MapName == L"/Game/ProjectAbyss/Maps/OverWorld" && bGameStarted == false) 
	{
	/*	SaveLoad->SaveBoatState();
		SaveLoad->SavePlayerState();*/

		//bGameStarted gets set to false when moveing from tutorial level in blueprint
		SaveLoad->SaveAllData();
		SaveLoad->LoadAllData();
		bGameStarted = true;
	}
	else if (MapName == L"/Game/ProjectAbyss/Maps/MainMenuMap" || MapName == L"/Game/ProjectAbyss/Maps/UEDPIE_0_MainMenuMap")
	{
		bGameStarted = false;
		TotalPhotosTaken = 0;
	}
	else
	SaveLoad->LoadAllData();
	//MissionLog->MissionTrackingChange.Broadcast(MissionLog->CurrentTrackedMission);
	PreviousMap = MapName;

	/*SaveLoad->LoadPlayerState();
	SaveLoad->LoadBoatState();*/
	MapLoadComplete.Broadcast();
}
