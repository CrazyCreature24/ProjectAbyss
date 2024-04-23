// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Engine/DataTable.h"

#include "MissionLogSubsystem.generated.h"

/**
 *
 */
class AMissionBase;
struct FMissionDetails;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectiveIDCalled, FString, ObjectiveID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMissionWasCompleted, FName, MissionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMissionWasSubmitted, FName, MissionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMissionTrackingChange, FName, MissionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPrerequisiteMet);

UCLASS()
class PROJECTABYSS_API UMissionLogSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	//The Data Table containing all the missions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable")
	UDataTable* MissionTable;


	//Mission Arrays
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable")
	TArray<FName>CurrentActiveMissions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable")
	TArray<FName>CompletedMissions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable")
	TArray<FName>UnavailableMainMissions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable")
	TArray<FName>AvailableMainMissions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable")
	TArray<FName>UnavailableSideMissions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable")
	TArray<FName>AvailableSideMissions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable")
	TArray<AMissionBase*> CurrentMissions;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable")
	FName CurrentTrackedMission;


	//Delegates
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FObjectiveIDCalled ObjectiveIDCalled;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FMissionWasCompleted MissionWasCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FMissionTrackingChange MissionTrackingChange;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FPrerequisiteMet PrerequisiteMet;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FMissionWasSubmitted MissionWasSubmitted;

public:

	UMissionLogSubsystem();

	/// <summary>
	/// Move a Mission from unavailableMainMission or unavailableSideMission array to currentActiveMissions array. Creates a new MissionBase Object and adds it to CurrentMissions array.
	/// </summary>
	/// <param name="MissionID"></param>
	UFUNCTION(BlueprintCallable)
	void AddNewMission(FName MissionID);

	/// <summary>
	/// Removes the mission from current mission arrays, and sends the mission ID to completed missions if its a main mission or returns it to available side missions if its a side mission. Then destroys the given mission actor.
	/// </summary>
	/// <param name="CompletedMission"></param>
	UFUNCTION(BlueprintCallable)
	void CompleteMission(AMissionBase* CompletedMission);

	/// <summary>
	/// Returns true if CurrentActiveMission contain the MissionID that is given. I'm not too sure what this would be good for. Just kinda copied it from a tutorial.
	/// </summary>
	/// <param name="QuestID"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
	bool QueryActiveMissions(FName MissionID);

	/// <summary>
	/// Sets the current tracked mission name to the mission of the given input
	/// </summary>
	/// <param name="MissionID"></param>
	UFUNCTION(BlueprintCallable)
	void TrackMission(FName MissionID);

	/// <summary>
	/// When a new game is set-up. This should be called to initialize the Missions to their proper arrays.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void NewGameMissionSetup();

	/// <summary>
	/// (WIP) Planned to load from save, all the quests arrays.
	/// Might be used to move missions actors from one instance to another
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void LoadMissionsFromSave();

	/// <summary>
	/// Get the MissionDetails of the given Mission from the DataTable. Meant to help the PC Terminal.
	/// </summary>
	/// <param name="MissionID"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
	FMissionDetails GetMissionDetails(FName MissionID);

	/// <summary>
	/// Called after each quest completion or upgrade. Checks all the unavailable missions if there requirements are met, then moved those quests to available quests.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void CheckPrequisites();

	/// <summary>
	/// Broadcast the ObjectiveID deligate for mission Actors to hear and update their objectives if they have a matching ObjectiveID
	/// </summary>
	/// <param name="ObjectiveID"></param>
	UFUNCTION(BlueprintCallable)
	void CallObjectiveID(FString ObjectiveID);

	/// <summary>
	/// Get an active mission Actor through its MissionID
	/// Returns null if no actor exists
	/// </summary>
	/// <param name="MissionID"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
	AMissionBase* GetMissionPtr(FName MissionID);

	/// <summary>
	/// This Resets all the arrays back to empty and puts all the missions back into the unavailableMissions arrays
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ResetArrays();

	UFUNCTION(BlueprintCallable)
	void SaveMissionActors();

	UFUNCTION(BlueprintCallable)
	void SetTutorial();

	UFUNCTION(BlueprintCallable)
	void LoadTutorialDT();

	UFUNCTION(BlueprintCallable)
	void LoadOverworldDT();

	UFUNCTION(BlueprintCallable)
	void RandomizeSideMissions();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; //IDK what I'm doing

	const int maxRandMissionSize = 1;

	int randomMissionSize = maxRandMissionSize;

};

/*
ChangeLog:
-----------------------------
Date:		Oct 12 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	Logging all active quests
-----------------------------
Date:		Oct 17, 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Added some functions to help select the quest. and to load quests into their repective arrays.
			- Still need to add functions to load from save.
-----------------------------
Date:		Oct 20, 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Commenting code
			- Set-up ObjectiveID call delegate for questBase to hear
-----------------------------
Date:		Nov 01, 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Trying to set-up networking. IT IS NOT GOING WELL
-----------------------------
Date:		Nov 02, 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Fixed ranged based loop errors
-----------------------------
Date:		Nov 13 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Renaming "Quest" to "Mission"
-----------------------------
Date:		Nov 17 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Added side missions and modified functions support
			- Check prerequisites still need work as we dont have upgrade system yet
-----------------------------
Date:		Nov 21 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	-
-----------------------------
Date:		Jan 18 2024
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Updating comments
			- Cleaning up code
-----------------------------
Date:		Feb 20 2024
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Added rewarding the player money when completeing the mission
-----------------------------
*/

