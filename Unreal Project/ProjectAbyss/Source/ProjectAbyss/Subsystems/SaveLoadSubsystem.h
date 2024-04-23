// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ProjectAbyssSaveGame.h"
#include "SaveLoadSubsystem.generated.h"

/**
 * 
 */
class ABoat;

UCLASS()
class PROJECTABYSS_API USaveLoadSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	USaveLoadSubsystem();


	/// <summary>
	/// Creates the save slot for the game to use.
	/// Gets called:
	/// - this Subsystem's constructor
	/// - when play or tutorial is pressed in the main menu. (This is to reset the save)
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Saving")
	void CreateSave();

public:
	//Saving

	/// <summary>
	/// Calls SaveMissionActors in MissionLogSubsystem which basically saves the mission Actor's variables
	/// Should get called:
	/// - When Moving to new instance in overworld (Cave Entrance BP)
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Saving")
	void SaveMissionActors(); //done

	/// <summary>
	/// (WIP) Should save player stats. Player stress and health. Probably equipment too?
	/// Should get called:
	/// - When Moving to new instance in overworld (Cave Entrance BP)
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Saving")
	void SavePlayerState(); //Might just be hp now and equipment //done

	//TODO:
	/// <summary> 
	/// Saves boat position and storage
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Saving")
	void SaveBoatState(); //done needs some tweaking


	/// <summary>
	/// Saves the journal enteries that are unlocked
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Saving")
	void SaveJournalEnteries(); //TODO: //Not in use
	
	/// <summary>
	/// Saves the pictues in the camera
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Saving")
	void SaveCameraPictures(); //TODO:
	
	/// <summary>
	/// Save player's money
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Saving")
	void SaveMoney(); //TODO

	/// <summary>
	/// Saves the entrance of a cave if entered
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Saving")
	void SaveCaveEntrence(USceneComponent* comp);

	/// <summary>
	/// (WIP) Should save the photo gallery.
	/// Should get called:
	/// - When uploading photos in the photo gallery
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Saving")
	void SavePhotoGallery(TArray<class UTextureRenderTarget2D*> photos); //UTextureRenderTarget2D

	UFUNCTION(BlueprintCallable, Category = "Saving")
	void SaveJournalEntry(class UProfileCardWidget* ProfieCard);

	UFUNCTION(BlueprintCallable, Category = "Saving")
	void SaveDivebot();

	UFUNCTION(BlueprintCallable, Category = "Saving")
	void SaveCollectionSystemlData(class ACollectionSystemBase* CollectionSystem);

public:
	//Loading

	/// <summary>
	/// Loads Mission actor details from the save file.
	/// Should get called:
	/// - ProjectAbyssGameInstance's LoadComplete
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Loading")
	void LoadMissionActors(); //done

	/// <summary>
	/// (WIP) Loads player's stats. player stress and health. Equipmenmt too
	/// Should get called:
	/// - Probably on DiverCharacter's BeginPlay?
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Loading")
	void LoadPlayerState(); //Done //need to setup in 

	/// <summary>
	/// Load Boat position and rotation. Storage stuff too.
	/// Should get called:
	/// - Probably on Boat's Begin Play
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Loading")
	void LoadBoatState();  
	//Done Boat Transform

	UFUNCTION(BlueprintCallable, Category = "Loading")
	void LoadMoney();  

	UFUNCTION(BlueprintCallable, Category = "Loading")
	void LoadCamera();  //Called in PhotographyComponent

	UFUNCTION(BlueprintCallable, Category = "Loading")
	float LoadDivebotTimeRemaining();

	UFUNCTION(BlueprintCallable, Category = "Loading")
	float LoadDivebotGraceRemaining();

	/// <summary>
	/// Loads spawn loacation for cave exit 
	/// </summary>
	/// <returns>The location vector</returns>
	UFUNCTION(BlueprintCallable, Category = "Loading")
	FVector GetCaveExitLocationFromSave();

	/// <summary>
	/// Loads spawn rotation for cave exit 
	/// </summary>
	/// <returns>The rotation rotator</returns>
	UFUNCTION(BlueprintCallable, Category = "Loading")
	FRotator GetCaveExitRotationFromSave();

	UFUNCTION(BlueprintCallable, Category = "Loading")
	void LoadCollectionSystemData(class ACollectionSystemBase* CollectionSystem);

	UFUNCTION( Category = "Loading")
	bool GetProfileCardInfo(int JournalIndex, FProfileCardSavedInfo& ProfileCardEditableInfo);

public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void InitialLoadSetup();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SaveAllData();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void LoadAllData();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void ResetPhotos();

	// Get data to use
	class UProjectAbyssSaveGame* GetDataToUse() { return DataToUse; }

private:
	
	UFUNCTION()
	TArray<FString>GetEquipmentInInventory(UInventoryComponent* ActorInventory); //helper method

	UFUNCTION()
	void SetEquipmentInInventory(UInventoryComponent* ActorInventory, TArray<FString> EquipmentNames); //helper method

private:

	// A pointer to easily plop the data into
	class UProjectAbyssSaveGame* DataToUse;

	UPROPERTY()
	UClass* LightBpAsset;// = LightBPClass.LoadSynchronous();
	UPROPERTY()
	UClass* PhotoCameraBpAsset;
	UPROPERTY()
	UClass* FishFinderBpAsset;
	UPROPERTY()
	UClass* SoundDecoyBpAsset;
	UPROPERTY()
	UClass* SeaScooterBpAsset;

};
