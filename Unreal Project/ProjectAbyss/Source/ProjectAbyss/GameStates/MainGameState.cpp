// Copyright Astralwater Interactive. All Rights Reserved.


#include "MainGameState.h"
#include "Divebot.h"
#include "Boat.h"
#include "DiverCharacter.h"
#include "HealthStateComponent.h"
#include "ProjectAbyssSaveGame.h"
#include "InventoryComponent.h"
#include "Subsystems/SaveLoadSubsystem.h"
#include "PhotographyCamera.h"
#include "DiverController.h"

#include "Kismet/GameplayStatics.h"

AMainGameState::AMainGameState()
{

}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();
}

void AMainGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainGameState::SpawnDivebot()
{

	if (GameBoat && GameDiveBot == nullptr)
	{
		FActorSpawnParameters spawnParameters = FActorSpawnParameters();
		spawnParameters.Instigator = GetInstigator();
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FRotator rot = FRotator(0, 0, 0);
		FVector vec = GameBoat->GetDivebotPointLocation()->GetComponentLocation();

		UWorld* world = GetWorld();
 		GameDiveBot = Cast<ADivebot>(world->SpawnActor(DiveBotClass, &vec, &rot, spawnParameters));
	}
}

void AMainGameState::SpawnDivebotNearDiver()
{

	if (GameDiveBot)
	{
		GameDiveBot->Destroy();
		GameDiveBot = nullptr;
	}

	if (!GameDiveBot)
	{
		FRotator rot = FRotator(0, 0, 0);
		FVector vec = GameCharacter->GetActorLocation() + GameCharacter->GetActorRightVector().Normalize() * 1;
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FActorSpawnParameters& params = FActorSpawnParameters(param);

		GameDiveBot = Cast<ADivebot>(GetWorld()->SpawnActor(DiveBotClass, &vec, &rot, params));
	}

}

void AMainGameState::OnLevelTransferSpawnAsDivebot()
{

	UProjectAbyssSaveGame* save = Cast<UProjectAbyssSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	if (!GameDiveBot)
	{
		SpawnDivebot();
	}

	GameDiveBot->PossessDivebot();

	if (save)
	{
		FHitResult hit;
		save->bHasEnteredCave = false;
		GameDiveBot->SetActorLocationAndRotation(save->CavePointLoc, save->CavePointRot, false, &hit, ETeleportType::TeleportPhysics);
	}
}

void AMainGameState::DeSpawnDivebot()
{
	if (GameDiveBot)
	{
		GameDiveBot->Destroy();
		GameDiveBot = nullptr;
	}
}

void AMainGameState::PlayerDied()
{
	bPlayerHasDied = true;

	USaveLoadSubsystem* SaveLoad = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USaveLoadSubsystem>();
	SaveLoad->SavePlayerState();

	if (!GameDiveBot)
	{
		SpawnDivebot();
	}

	// Only the icons were being wiped, not the actual equipment
	//GetPlayer()->InventoryComponent->EmptyInventory();
}

void AMainGameState::ResetPlayerState()
{
	bPlayerHasDied = false;
}


void AMainGameState::LoadOverWorld_Implementation()
{
}

void AMainGameState::RespawnPlayer_Implementation()
{
	GetPlayer()->OnRespawn();

	//else call function to change level
	GetPlayer()->HealthComponent->Reset();

	GetPlayer()->bBlockInput = false;

	UProjectAbyssSaveGame* save = Cast<UProjectAbyssSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	UGameplayStatics::SaveGameToSlot(save, "Slot1", 0);

	if (GetBoat())
	{
		GetPlayer()->SetActorLocation(GetBoat()->GetRespawnPointLocation());
		DeSpawnDivebot();
		save->HasPlayerDied = false;
		save->HasPlayerRespawn = true;
		GetPlayer()->SwapPlayerToDeathMesh(false);
		UGameplayStatics::SaveGameToSlot(save, "Slot1", 0);
	}
	else
	{
		UGameplayStatics::SaveGameToSlot(save, "Slot1", 0);
		LoadOverWorld();
	}

	USaveLoadSubsystem* SaveLoad = UGameplayStatics::GetGameInstance(this)->GetSubsystem<USaveLoadSubsystem>();
	SaveLoad->LoadPlayerState();
	GetPlayer()->CamInventoryComponent->ChangeHotbarImage(0, GetPlayer()->CameraActor);

	if (Cast<ADiverController>(GetPlayer()->GetController())->HeadToBoatWidget)
		Cast<ADiverController>(GetPlayer()->GetController())->HeadToBoatWidget->SetVisibility(ESlateVisibility::Hidden);
}
