// Copyright Astralwater Interactive. All Rights Reserved.


#include "Gamemodes/MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PCTerminal.h"

AMainGameMode::AMainGameMode()
{
	// Set default pawn class to our Blueprinted character
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	//Gets PC actor
	//Gets all creatures in the world
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APCTerminal::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		Cast<APCTerminal>(FoundActors[0])->ToggleNotification(true);
	}
}

void AMainGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Add player to list
	PlayerControllerList.Add(NewPlayer);
}
