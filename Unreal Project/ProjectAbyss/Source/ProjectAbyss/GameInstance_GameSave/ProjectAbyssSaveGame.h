// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PhotographyCollectionSubsystem.h"
#include "../Subsystems/CollectionSystemBase.h"

#include "Actors/MissionSystem/MissionBase.h"
#include "Components/HealthStateComponent.h"

#include "ProjectAbyssSaveGame.generated.h"

/**
 * 
 */
USTRUCT()
struct FMissionBaseActorDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FName MissionID;

	UPROPERTY()
	FMissionDetails MissionDetails;

	UPROPERTY()
	int CurrentStage = 0;

	//Current stage details at current stage
	UPROPERTY()
	FStageDetails CurrentStageDetails;

	UPROPERTY()
	TArray<FMissionObjectiveProgress> CurrentObjectiveProgress;

	UPROPERTY()
	bool bIsComplete;
};

USTRUCT()
struct FProfileCardSavedInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int JournalEntry;

	UPROPERTY()
	FString FishName;

	UPROPERTY()
	FString Notes;

};

USTRUCT()
struct FDivebotInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	float BatteryTime = 0;
	UPROPERTY()
	float GraceTimeRemaninig = 0;

};

UCLASS()
class PROJECTABYSS_API UProjectAbyssSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UProjectAbyssSaveGame();	

	//Getters

	// Photo gallery getter
	UFUNCTION(BlueprintCallable, Category = "Photography Collection")
		TArray<class UTextureRenderTarget2D*> GetPhotoGallery() const { return SavedPhotoGallery; } //TODO //UTextureRenderTarget2D

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Photography Collection")
		TArray<FCreatureEntryWrapper> CreatureCollectionArray; //TODO

	/*UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;*/

	//Saved Mission Actor Data
	UPROPERTY(SaveGame)
	TArray<FMissionBaseActorDetails> MissionActorSaveDetails;


	//Saved Player Stats Data
	UPROPERTY()
	int SavedCurrentStressStateIndex = 0; //NOt in use

	UPROPERTY(SaveGame)
	TArray<FDamageState> SavedHealthStates; //Done

	//Money
	UPROPERTY()
	int SavedMoney = 500;

	UPROPERTY()
	// Current gallery of textures, set by the photography component
	TArray<class UTextureRenderTarget2D*> SavedPhotoGallery; //UTextureRenderTarget2D

	//Boat Position
	UPROPERTY()
	FTransform BoatTransform; //done 

	//Divebot
	UPROPERTY()
	FDivebotInfo divebotInfo;

	UPROPERTY()
	TArray<FString> PlayerEquipmentNames; //DONE

	UPROPERTY()
	TArray<FString> StorageEquipmentNames; //Done

	UPROPERTY()
	TArray<FProfileCardSavedInfo> EditedEntries;

	//UPROPERTY()
	//TArray<FCreatureEntryWrapper> CreatureEntryData; //This is for the collection syatem to load stuff in between maps

	//Cave ExitInfo
	UPROPERTY()
	FRotator CavePointRot = {0,0,0};
	UPROPERTY()
	FVector CavePointLoc = {0,0,0};
	UPROPERTY()
	bool bHasEnteredCave = false;

	UPROPERTY()
	bool HasPlayerDied = false;

	UPROPERTY()
	bool HasPlayerRespawn = true;

	//Camera Save info
	UPROPERTY()
	int CameraFilm = 20;

	UPROPERTY()
	int TotalPhotosTaken = 0;

	UPROPERTY()
	TArray<TSubclassOf<ACreature>> FishCapturedOnCam; //This Could break since im using static classes

	//Need: 
	//	- Journal Enteries //Need to check in with austin
	//	- Camera Saved Pictures
	//	- Player Inventory (Position and which item) //done
	//	- Storage Inventory (Position and which item) //done practically
	// 
	// 
	//	- Dropped equipment on death in the world to be picked up later? //scrapped. uneeded


};
