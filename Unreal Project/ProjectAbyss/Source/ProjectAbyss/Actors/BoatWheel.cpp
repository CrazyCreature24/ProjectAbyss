// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/BoatWheel.h"
#include "DiverCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "Boat.h"
#include "Subsystems/MissionSystem/MissionLogSubsystem.h"

ABoatWheel::ABoatWheel()
{
	SetReplicates(true);
}

void ABoatWheel::BeginPlay()
{
Super::BeginPlay();
}

void ABoatWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoatWheel::Interacted(ADiverCharacter* Player)
{
	AInteractableActor::Interacted(Player);


	UMissionLogSubsystem* QuestLog = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>();


	QuestLog->CallObjectiveID("DriveBoatID");

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoat::StaticClass(), FoundActors);
	ABoat* Boat = FoundActors[0] ? Cast<ABoat>(FoundActors[0]) : nullptr;

	if (bIsLocked == false)
	{
		Boat->PossessBoat(Player);
	}
	//

}
