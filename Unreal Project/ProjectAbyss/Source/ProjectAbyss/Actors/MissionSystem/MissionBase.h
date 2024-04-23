// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "MissionBase.generated.h"

// Mission type like main quest or sub quest
UENUM()
enum class EMissionType : uint8
{
	Main		= 1 UMETA(DisplayName = "Main"),
	Side		= 2 UMETA(DisplayName = "Side"),
};

//Doesn't actually do much yet
UENUM()
enum class EObjectiveType : uint8
{
	Location	= 0 UMETA(DisplayName = "Location"),
	Picture		= 1 UMETA(DisplayName = "Picture"),
};

//info on where the mission takes place
UENUM()
enum class EBiome : uint8
{
	Shallows	= 0 UMETA(DisplayName = "Shallows"),
	Crystal		= 1 UMETA(DisplayName = "Crystal"),
	Volcanic	= 2 UMETA(DisplayName = "Volcanic Waste"),
	Abyss		= 3 UMETA(DisplayName = "Abyss"),
};

//Depth upgrade prerequisite. does not do much yet as there is no upgrades yet.
UENUM()
enum class EDepthUpgradeRequired : uint8
{
	Upgrade_0	= 0 UMETA(DisplayName = "Upgrade_0"),
	Upgrade_1	= 1 UMETA(DisplayName = "Upgrade_1"),
	Upgrade_2	= 2 UMETA(DisplayName = "Upgrade_2"),
	Upgrade_3	= 3 UMETA(DisplayName = "Upgrade_3"),
};

//Details of an objective
USTRUCT(BlueprintType)
struct FObjectiveDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ObjectiveName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description; // not really being used atm

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectiveType Type = EObjectiveType::Picture;	//does not really do much rn

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsOptional = false;
};

//Details of the stage
USTRUCT(BlueprintType)
struct FStageDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StageName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description; //not really being used atm

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FObjectiveDetails> Objectives;

};

//Details of the mission
USTRUCT(BlueprintType)
struct FMissionDetails : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText MissionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText LogDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TrackingDescription; //Not really in use. might remove later

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMissionType Type = EMissionType::Side;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FStageDetails> Stage;

	//Either not going to be used or might be moved to MissionDetails
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MoneyReward = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBiome Biome = EBiome::Shallows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDepthUpgradeRequired DepthUpgradeRequired = EDepthUpgradeRequired::Upgrade_0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> PrequisiteMissions;

};

//The progress of the objective
USTRUCT(BlueprintType)
struct FMissionObjectiveProgress
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ObjectiveName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ObjectiveCount = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveUpdated);


UCLASS()
class PROJECTABYSS_API AMissionBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMissionBase();

	/// <summary>
	/// Called by ObjectiveIDCalled delegate in MissionLogSubsystem to check if this mission has a matching objective, then increments the ObjectiveCount.
	/// Moves to next stage if all objectives are complete.
	/// </summary>
	/// <param name="ObjectiveID"></param>
	UFUNCTION(BlueprintCallable)
	void OnObjectiveIDHeared(FString ObjectiveID);

	/// <summary>
	/// When a Mission object is created in the MissionLogSubsystem, it calls this function right after where it uses the 
	/// MissionID to get the information from the data table to fill the empty variables. The stage gets it variables from the current stage count.
	/// Gets called again when the stage gets updated.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void SetMissionDetails();

	/// <summary>
	/// Grabs the objective details from the ObjectiveID
	/// </summary>
	/// <param name="ObjectiveID"></param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
	FObjectiveDetails ObjectiveDataByID(FString ObjectiveID);

	/// <summary>
	/// If ALL objectives are complete in current stage, return true. (Should add optional skip)
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
	bool CheckObjectivesAreComplete();

	/// <summary>
	/// Forcfully moves the current stage to the next stage. If there is no next stage, the mission becomes complete.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void MoveToNextStage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable", Replicated)
	FName MissionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable", Replicated)
	FMissionDetails MissionDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable", Replicated)
	int CurrentStage = 0;
	//Current stage details at current stage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable", Replicated)
	FStageDetails CurrentStageDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable", Replicated)
	TArray<FMissionObjectiveProgress> CurrentObjectiveProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission Variable", Replicated)
	bool bIsComplete;

	UPROPERTY(BlueprintAssignable, Category = "Objective")
	FObjectiveUpdated ObjectiveUpdated;

public:


};

/*
ChangeLog:
-----------------------------
Date:		Oct 4 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	Quest Base for all quests
-----------------------------
Date:		Oct 19 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Added three functions
				+ OnObjectiveIDHeared(FString ObjectiveID) [Made for event call]
				+ SetQuestDetails()
				+ ObjectiveDataByID(FString ObjectiveID)
			- Added Prequisite array to FQuestDetails
-----------------------------
Date:		Oct 20 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Commenting code
-----------------------------
Date:		Oct 31 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Started Networking
-----------------------------
Date:		Nov 02 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Created deligate for when an objective gets updated
-----------------------------
Date:		Nov 13 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Renaming "Quest" to "Mission"
-----------------------------
Date:		Nov 15 2023
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Added more mission specitifity with with which biome its in and which depth upgrade needed.
-----------------------------
Date:		Jan 19 2024
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Commenting and cleaning up code.
-----------------------------
*/