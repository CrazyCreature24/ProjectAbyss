// Copyright Astralwater Interactive. All Rights Reserved.



#include "Subsystems/CollectionSystemBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance_GameSave/ProjectAbyssSaveGame.h"
#include "GameInstance_GameSave/ProjectAbyssGameInstance.h"
#include "SaveLoadSubsystem.h"
#include "InFocusWidget.h"
#include "Components/WidgetComponent.h"
// Sets default values
ACollectionSystemBase::ACollectionSystemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

 
}

// Called when the game starts or when spawned
void ACollectionSystemBase::BeginPlay()
{
	Super::BeginPlay();

    UProjectAbyssGameInstance* PAGameInstance =  Cast<UProjectAbyssGameInstance>(GetGameInstance());
                                                                        //Hush
    USaveLoadSubsystem* SaveLoad = UGameplayStatics::GetGameInstance(PAGameInstance)->GetSubsystem<USaveLoadSubsystem>();

    if (PAGameInstance->bGameStarted == false && PAGameInstance->PreviousMap != L"/Game/ProjectAbyss/Maps/TutorialLevel")
    {                                                                     
        for (int i = 0; i < CreatureCollectionArray.Num(); i++)
        {
            CreatureCollectionArray[i].Creature.GetDefaultObject()->bFirstTimeBonusAvailable = true;

            CreatureCollectionArray[i].CreatureName = CreatureCollectionArray[i].Creature.GetDefaultObject()->Name;
        }

        SaveLoad->SaveCollectionSystemlData(this);
    }

    SaveLoad->LoadCollectionSystemData(this);

        for (int i = 0; i < CreatureCollectionArray.Num(); i++)
        {
            CreatureCollectionArray[i].Creature.GetDefaultObject()->bFirstTimeBonusAvailable = CreatureCollectionArray[i].bFirstTimeBonusAvailable;
            CreatureCollectionArray[i].Creature.GetDefaultObject()->Name = CreatureCollectionArray[i].CreatureName;
        }
        
        //Gets all creatures in the world
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACreature::StaticClass(), FoundActors);
        for (int i = 0; i < FoundActors.Num(); i++)
        {
		    ACreature* Creature = Cast<ACreature>(FoundActors[i]);
            if (Creature)
            {
                Creature->bFirstTimeBonusAvailable = CreatureCollectionArray[Creature->CollectionIndex-1].bFirstTimeBonusAvailable;
                Creature->Name = CreatureCollectionArray[Creature->CollectionIndex-1].CreatureName;
		    }
	    }

}

// Called every frame
void ACollectionSystemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectionSystemBase::SaveCollectionData()
{
    //UProjectAbyssSaveGame* SaveGameInstance = Cast<UProjectAbyssSaveGame>(UGameplayStatics::CreateSaveGameObject(UProjectAbyssSaveGame::StaticClass()));
    //if (SaveGameInstance)
    //{
    //    // Copy data from the actor to the save game object
    //    SaveGameInstance->CreatureCollectionArray = CreatureCollectionArray;

    //    // Save the data to disk
    //    UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
    //}
}

void ACollectionSystemBase::LoadCollectionData()
{
    //UProjectAbyssSaveGame* SaveGameInstance = Cast<UProjectAbyssSaveGame>(UGameplayStatics::CreateSaveGameObject(UProjectAbyssSaveGame::StaticClass()));
    //if (SaveGameInstance)
    //{
    //    // Make sure CreatureCollectionArray is valid before copying data
    //    if (SaveGameInstance->CreatureCollectionArray.Num() > 0)
    //    {
    //    CreatureCollectionArray = SaveGameInstance->CreatureCollectionArray;
    //    }
    //}
}

void ACollectionSystemBase::UpdateInfoOfFishInWorld(int FishIndex, FString NewName)
{
    CreatureCollectionArray[FishIndex - 1].CreatureName = NewName;
    CreatureCollectionArray[FishIndex - 1].Creature.GetDefaultObject()->Name = NewName;
    USaveLoadSubsystem* SaveLoad = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USaveLoadSubsystem>();
    SaveLoad->SaveCollectionSystemlData(this);

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), CreatureCollectionArray[FishIndex - 1].Creature.GetDefaultObject()->StaticClass(), FoundActors);
    for (int i = 0; i < FoundActors.Num(); i++)
    {
        ACreature* Creature = Cast<ACreature>(FoundActors[i]);
        if (Creature)
        {
            Creature->bFirstTimeBonusAvailable = CreatureCollectionArray[Creature->CollectionIndex - 1].bFirstTimeBonusAvailable;
            Creature->Name = CreatureCollectionArray[Creature->CollectionIndex - 1].CreatureName;
            Cast<UInFocusWidget>(Creature->FocusWidgetComponent->GetWidget())->SetCreatureName(Creature);
        }
    }
}

void ACollectionSystemBase::OnEndPlay()
{
    //if (GetWorld())
    //{
    //    USaveLoadSubsystem* SaveLoad = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USaveLoadSubsystem>();
    //    SaveLoad->SaveCollectionSystemlData(this);
    //}

    Super::OnEndPlay;
}

