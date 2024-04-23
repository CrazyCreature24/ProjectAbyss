// Copyright Astralwater Interactive. All Rights Reserved.


#include "Subsystems/SaveLoadSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance_GameSave/ProjectAbyssSaveGame.h"
#include "Gamemodes/MainGameMode.h"

#include "MissionSystem/MissionLogSubsystem.h"
#include "Components/StressComponent.h"
#include "Components/HealthStateComponent.h"
#include "Actors/Pawns/DiverCharacter.h"
#include "InventoryComponent.h"
#include "Boat.h"
#include "Divebot.h"

#include "DeployableLight.h"
#include "PhotographyCamera.h"
#include "PhotographyComponent.h"
#include "FishFinder.h"
#include "SoundDecoy.h"
#include "Seascooter.h"

#include "ProfileCardWidget.h"
#include "Components/EditableText.h"
#include "Components/MultiLineEditableText.h"

#include "CollectionSystemBase.h"
#include "Actors/Pawns/Creature.h"

#include "CollectionJournalWidget.h"

#include "StorageChest.h"

//Was gettubg annoyed doing UGameplayStatics so made some defines lol. for getting and saving the data.
#define GetSave Cast<UProjectAbyssSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));
#define SaveData(DataToUse) (UGameplayStatics::SaveGameToSlot(DataToUse, "Slot1", 0));

USaveLoadSubsystem::USaveLoadSubsystem()
{
	CreateSave();

	//TSoftClassPtr<AActor>LightBPClass = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/ProjectAbyss/Blueprints/Actors/Equipment/BP_DeployableLight.BP_DeployableLight'")));
	//LoadedBpAsset = LightBPClass.LoadSynchronous();
	////LoadedBpAsset = LoadObject<ADeployableLight>(nullptr, TEXT("/Game/ProjectAbyss/Blueprints/Actors/Equipment/BP_DeployableLight.BP_DeployableLight"));

	TSoftClassPtr<ADeployableLight>LightBPClass = TSoftClassPtr<ADeployableLight>(FSoftObjectPath(TEXT("Blueprint'/Game/ProjectAbyss/Blueprints/Actors/Equipment/BP_DeployableLight.BP_DeployableLight_C'")));
	TSoftClassPtr<APhotographyCamera>CameraBPClass = TSoftClassPtr<APhotographyCamera>(FSoftObjectPath(TEXT("Blueprint'/Game/ProjectAbyss/Blueprints/Actors/Equipment/BP_PhotographyCamera.BP_PhotographyCamera_C'")));
	TSoftClassPtr<AFishFinder>FishFinderBpClass = TSoftClassPtr<AFishFinder>(FSoftObjectPath(TEXT("Blueprint'/Game/ProjectAbyss/Blueprints/Actors/Equipment/BP_FishFinder.BP_FishFinder_C'")));
	TSoftClassPtr<ASoundDecoy>SoundDecoyBPClass = TSoftClassPtr<ASoundDecoy>(FSoftObjectPath(TEXT("Blueprint'/Game/ProjectAbyss/Blueprints/Actors/Equipment/BP_SoundDecoy.BP_SoundDecoy_C'")));
	TSoftClassPtr<ASeascooter>SeaScooterBPClass = TSoftClassPtr<ASeascooter>(FSoftObjectPath(TEXT("Blueprint'/Game/ProjectAbyss/Blueprints/Actors/Equipment/BP_Seascooter.BP_Seascooter_C'")));
	//TSoftClassPtr<AActor> ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/PluginModuleName/Some/Path/BP_MyAwesomeActor.BP_MyAwesomeActor_C'")));

	PhotoCameraBpAsset = CameraBPClass.LoadSynchronous();
	PhotoCameraBpAsset->AddToRoot();

	LightBpAsset = LightBPClass.LoadSynchronous();
	LightBpAsset->AddToRoot();

	FishFinderBpAsset = FishFinderBpClass.LoadSynchronous();
	FishFinderBpAsset->AddToRoot();

	SoundDecoyBpAsset = SoundDecoyBPClass.LoadSynchronous();
	if (SoundDecoyBpAsset != nullptr)
	SoundDecoyBpAsset->AddToRoot();

	SeaScooterBpAsset = SeaScooterBPClass.LoadSynchronous();
	SeaScooterBpAsset->AddToRoot();
}

void USaveLoadSubsystem::CreateSave()
{
	//Creating the save slot
	DataToUse = Cast<UProjectAbyssSaveGame>(UGameplayStatics::CreateSaveGameObject(UProjectAbyssSaveGame::StaticClass()));
	SaveData(DataToUse);
}

void USaveLoadSubsystem::SaveMissionActors()
{

	UMissionLogSubsystem* MissionLog = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>();
	MissionLog->SaveMissionActors();
}

void USaveLoadSubsystem::SavePlayerState()
{
	//TODO - Save Player State
	DataToUse = GetSave;

	//Get Player
	ADiverCharacter* Player = Cast<ADiverCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (Player == nullptr)
		return;

		//Get health states here
	DataToUse->SavedHealthStates = Player->HealthComponent->GetHPSaveData();

	UInventoryComponent* InventoryComponent = Player->InventoryComponent;
	//TArray<AEquipmentBase*> Inventory = InventoryComponent->Inventory;

	DataToUse->PlayerEquipmentNames.Empty();
	DataToUse->PlayerEquipmentNames = GetEquipmentInInventory(InventoryComponent);

	SaveData(DataToUse);
}

void USaveLoadSubsystem::SavePhotoGallery(TArray<class UTextureRenderTarget2D*> photos) //UTextureRenderTarget2D
{
	DataToUse = GetSave;
	DataToUse->SavedPhotoGallery = photos;
	SaveData(DataToUse);
}

void USaveLoadSubsystem::SaveJournalEntry(UProfileCardWidget* ProfileCard)
{
	DataToUse = GetSave;

	TArray<FProfileCardSavedInfo>& EditedEntries = DataToUse->EditedEntries;

	int EntryIndex = ProfileCard->StoredCollectionIndex;
	FString EditableNname = ProfileCard->EditableName->GetText().ToString();
	FString OtherNotes = ProfileCard->OtherNotes->GetText().ToString();

	for (int i = 0; i < EditedEntries.Num(); i++)
	{
		if (EditedEntries[i].JournalEntry == EntryIndex)
		{
			EditedEntries.RemoveAt(i);
			continue;
		}
	}

	//FProfileCardSavedInfo tempInfo = { EntryIndex, EditableNname, OtherNotes };
	//EditedEntries.Add(tempInfo);

	EditedEntries.Add(FProfileCardSavedInfo{ EntryIndex, EditableNname, OtherNotes });
	//FString Notes = ProfileCard->Notes->GetText().ToString();

	SaveData(DataToUse);
}

void USaveLoadSubsystem::SaveDivebot()
{
	UProjectAbyssSaveGame* save = Cast<UProjectAbyssSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

	if (MyGameState)
	{
		save->divebotInfo.BatteryTime = MyGameState->GetBot()->BatteryLifeInSeconds;
		if (GetWorld()->GetTimerManager().IsTimerActive(MyGameState->GetBot()->Grace))
			save->divebotInfo.GraceTimeRemaninig = GetWorld()->GetTimerManager().GetTimerRemaining(MyGameState->GetBot()->Grace);
		else
			save->divebotInfo.GraceTimeRemaninig = -1;
	}

	UGameplayStatics::SaveGameToSlot(save, "Slot1", 0);

}

void USaveLoadSubsystem::SaveCollectionSystemlData(ACollectionSystemBase* CollectionSystem)
{
	DataToUse = GetSave;

	DataToUse->CreatureCollectionArray = CollectionSystem->CreatureCollectionArray;

	//CollectionSystem->CreatureCollectionArray[0].Creature.GetDefaultObject()->bFirstTimeBonusAvailable = false; //How the fuck is this different??????

	for (FCreatureEntryWrapper& CreeatureWrapper : DataToUse->CreatureCollectionArray)
	{
		CreeatureWrapper.bFirstTimeBonusAvailable = Cast<ACreature>(CreeatureWrapper.Creature->GetDefaultObject())->bFirstTimeBonusAvailable;
		CreeatureWrapper.CreatureName = Cast<ACreature>(CreeatureWrapper.Creature->GetDefaultObject())->Name;
	}


	SaveData(DataToUse);
}

void USaveLoadSubsystem::SaveBoatState()
{
	DataToUse = GetSave;

	// grab boat
	TArray<AActor*> Boats;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoat::StaticClass(), Boats);

	// If a boat is in the level
	if (Boats.IsEmpty())
		return; 

	ABoat* boatRef = Cast<ABoat>(Boats[0]);

	FTransform boatTransform = boatRef->GetTransform();

	DataToUse->BoatTransform = boatTransform;

	//Storage save code TODO:
	TArray<AActor*> StorageChests;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStorageChest::StaticClass(), StorageChests);

	if (StorageChests.IsEmpty())
		return;

	AStorageChest* BoatStorage = Cast<AStorageChest>(StorageChests[0]);

	UInventoryComponent* InventoryComponent = BoatStorage->Inventory;

	DataToUse->StorageEquipmentNames.Empty();
	DataToUse->StorageEquipmentNames = GetEquipmentInInventory(InventoryComponent);

	SaveData(DataToUse);
}

void USaveLoadSubsystem::SaveJournalEnteries()
{
}

void USaveLoadSubsystem::SaveCameraPictures()
{
	DataToUse = GetSave;

	TArray<AActor*> PhotoCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhotographyCamera::StaticClass(), PhotoCameras);

	if (PhotoCameras.IsEmpty())
		return;

	APhotographyCamera* PhotoCamera = Cast<APhotographyCamera>(PhotoCameras[0]);

	DataToUse->CameraFilm = PhotoCamera->PhotographyCameraComponent->CurrentFilm;

	//DataToUse->TotalPhotosTaken = PhotoCamera->PhotographyCameraComponent->TotalPhotosTaken;

	DataToUse->FishCapturedOnCam = PhotoCamera->PhotographyCameraComponent->FishCapturedNico;

	SaveData(DataToUse);

}

void USaveLoadSubsystem::SaveMoney()
{
	AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode == nullptr)
		return;
	
	DataToUse = GetSave;

	DataToUse->SavedMoney = GameMode->Money;

	SaveData(DataToUse);
}

void USaveLoadSubsystem::SaveCaveEntrence(USceneComponent* comp)
{
	AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

	if (MyGameState && comp)
	{
		DataToUse = GetSave;

		if (!DataToUse->bHasEnteredCave)
		{
			DataToUse->CavePointLoc = comp->GetComponentLocation();
			DataToUse->CavePointRot = comp->GetComponentRotation();
			DataToUse->bHasEnteredCave = true;
		}
		SaveData(DataToUse);
	}
}

void USaveLoadSubsystem::LoadMissionActors()
{
	DataToUse = GetSave;
	UMissionLogSubsystem* MissionLog = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>();

	MissionLog->CurrentMissions.Empty();

	for (auto i : DataToUse->MissionActorSaveDetails)
	{
		AMissionBase* LoadedMission = GetWorld()->SpawnActor<AMissionBase>();

		LoadedMission->MissionID = i.MissionID;
		LoadedMission->MissionDetails = i.MissionDetails;
		LoadedMission->CurrentStage = i.CurrentStage;
		LoadedMission->CurrentStageDetails = i.CurrentStageDetails;
		LoadedMission->CurrentObjectiveProgress = i.CurrentObjectiveProgress;
		LoadedMission->bIsComplete = i.bIsComplete;

		MissionLog->CurrentMissions.Push(LoadedMission);

	}

	MissionLog->MissionTrackingChange.Broadcast(MissionLog->CurrentTrackedMission);
}

void USaveLoadSubsystem::LoadPlayerState()
{
	DataToUse = GetSave;

	Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->Money; //idk if this is right

	//Get Player
	ADiverCharacter* Player = Cast<ADiverCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (Player == nullptr)
		return;

	//Get health states here
	Player->HealthComponent->SetHPSaveData(DataToUse->SavedHealthStates);

	//Get Player Inventory
	UInventoryComponent* InventoryComponent = Player->InventoryComponent; 

	SetEquipmentInInventory(InventoryComponent, DataToUse->PlayerEquipmentNames);
	

}

void USaveLoadSubsystem::LoadBoatState()
{
	// grab boat
	TArray<AActor*> Boats;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoat::StaticClass(), Boats);


	// If a boat is in the level
	if (Boats.IsEmpty())
		return;

	ABoat* boatRef = Cast<ABoat>(Boats[0]);

	DataToUse = GetSave;

	boatRef->SetActorTransform(DataToUse->BoatTransform);

	//Boat Storage
	TArray<AActor*> StorageChests;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStorageChest::StaticClass(), StorageChests);

	if (StorageChests.IsEmpty())
		return;

	AStorageChest* BoatStorage = Cast<AStorageChest>(StorageChests[0]);

	UInventoryComponent* InventoryComponent = BoatStorage->Inventory;

	SetEquipmentInInventory(InventoryComponent, DataToUse->StorageEquipmentNames);

}

void USaveLoadSubsystem::LoadMoney()
{
	AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode == nullptr)
		return;

	DataToUse = GetSave;

	GameMode->Money = DataToUse->SavedMoney;
}

void USaveLoadSubsystem::LoadCamera()
{
	DataToUse = GetSave;

	TArray<AActor*> PhotoCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhotographyCamera::StaticClass(), PhotoCameras);

	if (PhotoCameras.IsEmpty())
		return;

	APhotographyCamera* PhotoCamera = Cast<APhotographyCamera>(PhotoCameras[0]);

	PhotoCamera->PhotographyCameraComponent->CurrentFilm = DataToUse->CameraFilm;

//	PhotoCamera->PhotographyCameraComponent->TotalPhotosTaken = DataToUse->TotalPhotosTaken;

	PhotoCamera->PhotographyCameraComponent->FishCapturedNico = DataToUse->FishCapturedOnCam;

}

float USaveLoadSubsystem::LoadDivebotTimeRemaining()
{
	DataToUse = GetSave;
	return DataToUse->divebotInfo.BatteryTime;
}

float USaveLoadSubsystem::LoadDivebotGraceRemaining()
{
	DataToUse = GetSave;
	return DataToUse->divebotInfo.GraceTimeRemaninig;
}

FVector USaveLoadSubsystem::GetCaveExitLocationFromSave()
{
	DataToUse = GetSave;
	return 	DataToUse->CavePointLoc;
}

FRotator USaveLoadSubsystem::GetCaveExitRotationFromSave()
{
	DataToUse = GetSave;
	return DataToUse->CavePointRot;
}

void USaveLoadSubsystem::LoadCollectionSystemData(ACollectionSystemBase* CollectionSystem)
{
	DataToUse = GetSave;

	CollectionSystem->CreatureCollectionArray = DataToUse->CreatureCollectionArray;
	
}

bool USaveLoadSubsystem::GetProfileCardInfo(int JournalIndex, FProfileCardSavedInfo& OutProfileCardEditableInfo)
{
	DataToUse = GetSave;

	TArray<FProfileCardSavedInfo> EditedEntries = DataToUse->EditedEntries;

	FProfileCardSavedInfo EntryAtIndex;

	for (auto Entry : EditedEntries)
	{
		if (Entry.JournalEntry == JournalIndex)
		{
			OutProfileCardEditableInfo = Entry;
			return true;
		}
	}

	return false; 
}

void USaveLoadSubsystem::InitialLoadSetup()
{
	DataToUse = GetSave;

	DataToUse->BoatTransform.SetLocation(FVector(69971, -7083, 6930.293312f)); //quick test this was overworld pos

	SaveData(DataToUse);
}

void USaveLoadSubsystem::SaveAllData()
{
	SaveMissionActors();
	SavePlayerState();
	SaveBoatState();
	//SaveJournalEnteries(); //todo
	SaveCameraPictures(); //WIP
	SaveMoney(); 
	//SavePhotoGallery(); //Todo
}

void USaveLoadSubsystem::LoadAllData()
{
	LoadMissionActors();
	LoadPlayerState();
	LoadBoatState();
	LoadMoney();
	//LoadCamera();
}

void USaveLoadSubsystem::ResetPhotos()
{
	FString JournalPhotosFilePath = Cast<UCollectionJournalWidget>(UCollectionJournalWidget::StaticClass()->GetDefaultObject())->JournalPhotosFilePath;
	FString CameraPhotosFilePath = Cast<UPhotographyComponent>(UPhotographyComponent::StaticClass()->GetDefaultObject())->CameraPhotosFilePath;

	TArray<FString> CameraPhotoFiles;
	TArray<FString> JournalPhotoFiles;

	IFileManager& FileManager = IFileManager::Get();
	FileManager.FindFiles(JournalPhotoFiles, JournalPhotosFilePath.GetCharArray().GetData(), NULL);
	FileManager.FindFiles(CameraPhotoFiles, CameraPhotosFilePath.GetCharArray().GetData(), NULL);

	bool test = FileManager.DeleteDirectory(JournalPhotosFilePath.GetCharArray().GetData(),true,true);
	FileManager.DeleteDirectory(CameraPhotosFilePath.GetCharArray().GetData(),true,true);


	//for (const auto& PhotoFile : CameraPhotoFiles)
	//{
	//	FString PhotoFilePath = CameraPhotosFilePath + "/" + PhotoFile;

	//	FileManager.DeleteDirectory
	//}
}

TArray<FString> USaveLoadSubsystem::GetEquipmentInInventory(UInventoryComponent* ActorInventory)
{
	TArray<AEquipmentBase*> Inventory = ActorInventory->Inventory;

	TArray<FString> EquipmentNames;

	for (auto i : Inventory)
	{
		if (i == nullptr)
		{
			EquipmentNames.Add("NONE");
		}
		else
		{
			EquipmentNames.Add(i->GetItemName());
		}
	}

	return EquipmentNames;
}

void USaveLoadSubsystem::SetEquipmentInInventory(UInventoryComponent* ActorInventory, TArray<FString> EquipmentNames)
{
	if (EquipmentNames.Num())
	{
		TArray<AEquipmentBase*> Inventory = ActorInventory->Inventory;

		for (int i = 0; i < Inventory.Num(); i++)
		{
			FString equipName = EquipmentNames[i];
			if (equipName == "Deployable Light")
			{
				ADeployableLight* LightActor = Cast<ADeployableLight>(GetWorld()->SpawnActor(LightBpAsset));
				LightActor->Equip(ActorInventory->GetOwner());
				ActorInventory->SetEquipmentAtIndex(i, LightActor); //how the fuck
				ActorInventory->ChangeHotbarImage(i, LightActor);
			}
			else if (equipName == "Photography Camera")
			{
				APhotographyCamera* CameraActor = Cast<APhotographyCamera>(GetWorld()->SpawnActor(PhotoCameraBpAsset));
				CameraActor->Equip(ActorInventory->GetOwner());
				ActorInventory->SetEquipmentAtIndex(i, CameraActor); //how the fuck
				ActorInventory->ChangeHotbarImage(i, CameraActor);
			}
			else if (equipName == "Fish Finder")
			{
				AFishFinder* FishFinderActor = Cast<AFishFinder>(GetWorld()->SpawnActor(FishFinderBpAsset));
				FishFinderActor->Equip(ActorInventory->GetOwner());
				ActorInventory->SetEquipmentAtIndex(i, FishFinderActor); //how the fuck
				ActorInventory->ChangeHotbarImage(i, FishFinderActor);
			}
			else if (equipName == "Sound Decoy")
			{
				ASoundDecoy* SoundDecoyActor = Cast<ASoundDecoy>(GetWorld()->SpawnActor(SoundDecoyBpAsset));
				SoundDecoyActor->Equip(ActorInventory->GetOwner());
				ActorInventory->SetEquipmentAtIndex(i, SoundDecoyActor); //how the fuck
				ActorInventory->ChangeHotbarImage(i, SoundDecoyActor);
			}
			else if (equipName == "Sea Scooter")
			{
				ASeascooter* SeaScooterActor = Cast<ASeascooter>(GetWorld()->SpawnActor(SeaScooterBpAsset));
				SeaScooterActor->Equip(ActorInventory->GetOwner());
				ActorInventory->SetEquipmentAtIndex(i, SeaScooterActor); //how the fuck
				ActorInventory->ChangeHotbarImage(i, SeaScooterActor);
			}
		}
	}
}


