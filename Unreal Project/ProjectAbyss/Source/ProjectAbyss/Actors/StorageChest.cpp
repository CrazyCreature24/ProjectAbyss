// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/StorageChest.h"
#include "DiverCharacter.h"
#include "DiverController.h"
#include "PhotographyCamera.h"
#include <DeployableLight.h>
#include "Components/InventoryComponent.h"
#include "FishFinder.h"

//Nico Mods
#include "Kismet/GameplayStatics.h"
#include "Subsystems/MissionSystem/MissionLogSubsystem.h"
#include "GameInstance_GameSave/ProjectAbyssGameInstance.h"
#include <Seascooter.h>

AStorageChest::AStorageChest()
{
	//Create the inventory component
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));


	Inventory->MaxInventorySlots = 20;
	Inventory->Inventory.SetNum(Inventory->MaxInventorySlots);
}

void AStorageChest::BeginPlay()
{
	Super::BeginPlay();

	//Nico Mod
	if (Cast<UProjectAbyssGameInstance>(GetGameInstance())->bGameStarted == false) //This is for when the game starts the first time. does not repeat
	{



		ADeployableLight* LightActor;
		LightActor = GetWorld()->SpawnActor<ADeployableLight>(EquipmentClass1, GetActorLocation(), GetActorRotation());
		LightActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		LightActor->SetActorRelativeLocation(FVector(0.f, 0.f, 0.f));
		LightActor->Equip(this);
		Inventory->AddToInventory(LightActor);




	}
}

void AStorageChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStorageChest::Interacted(ADiverCharacter* Player)
{
	//Nico Mod
	UMissionLogSubsystem* MissionLog = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>();
	MissionLog->CallObjectiveID("Storage");

	Super::Interacted(Player);
	// Opens the storage chest UI
	if (StorageChestWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("StorageChestWidgetClass is nullptr. Please update it on the blueprint."));
		return;
	}
	StorageChestWidget = CreateWidget<UStorageChestWidget>(GetWorld(), StorageChestWidgetClass);
	StorageChestWidget->AddToViewport();
	StorageChestWidget->SetOwningStorageChest(this);

	
	ADiverController* DiverController = Cast<ADiverController>(Player->Controller);
	DiverController->SetInputMode(FInputModeUIOnly());
	DiverController->FlushPressedKeys();
	DiverController->bShowMouseCursor = true;



}
