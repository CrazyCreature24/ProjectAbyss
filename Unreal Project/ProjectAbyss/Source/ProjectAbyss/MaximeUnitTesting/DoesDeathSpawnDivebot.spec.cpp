// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
// Maxime Paradis Unit Test - completed on April 11th, 2024

#include "MaxTestUtils.h"
#include "DiverCharacter.h"
#include "HealthStateComponent.h"
#include "Boat.h"
#include "Divebot.h"

BEGIN_DEFINE_SPEC(FDoesDeathSpawnDivebot, "ProjectAbyss.Maxime-Unit-Tests.DoesDeathSpawnDivebot", FMaxTestUtils::TestsFlags)

UWorld* World;

END_DEFINE_SPEC(FDoesDeathSpawnDivebot)

void FDoesDeathSpawnDivebot::Define()
{
	BeforeEach([this]()
	{
		// Uncomment line below to make it test properly
		AutomationOpenMap("/Game/ProjectAbyss/Maps/OverWorld");
		World = FMaxTestUtils::GetWorld();
		TestNotNull("Check if World is properly created", World);
	});

	LatentIt("Does Death Spawn Divebot", EAsyncExecution::ThreadPool, [this](const FDoneDelegate TestDone)
	{

		AsyncTask(ENamedThreads::GameThread, [this]() 
			{
				bool IsTherePlayer = false;
				for (TActorIterator<AActor> It(World, ADiverCharacter::StaticClass()); It; ++It)
				{
					IsTherePlayer = true;
				}
				TestTrue("Check if there is a Player in the level", IsTherePlayer);
			}
		);

		AsyncTask(ENamedThreads::GameThread, [this]() 
			{
				bool IsThereABoat = false;
				for (TActorIterator<AActor> It(World, ABoat::StaticClass()); It; ++It)
				{
					IsThereABoat = true;
				}
				TestTrue("Check if there is a Boat in the level", IsThereABoat);
			}
		);


		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				ABoat* boat = FMaxTestUtils::GetBoat();
				boat->SetActorLocation(FVector(0, 0, 0));
			}
		);

		float StartTime = FPlatformTime::Seconds();
		while (FPlatformTime::Seconds() - StartTime < 4.f)
		{
			FPlatformProcess::Sleep(1.9f);
		}

		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				ADiverCharacter* player = FMaxTestUtils::GetPlayer();

				if (player)
				{
					TestTrue("Check if there is a DiverCharacter is Possessed", player->bIsPossessed);
					player->HealthComponent->DEBUG_KILLPLAYER();
				}
			}
		);


		StartTime = FPlatformTime::Seconds();
		while (FPlatformTime::Seconds() - StartTime < 4.f)
		{
			FPlatformProcess::Sleep(.80f);
		}

		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				bool IsThereABot = false;
				for (TActorIterator<AActor> It(World, ADivebot::StaticClass()); It; ++It)
				{
					IsThereABot = true;
				}
				TestTrue("Check if there is a DiveBot in the level", IsThereABot);
			}
		);


		StartTime = FPlatformTime::Seconds();
		while (FPlatformTime::Seconds() - StartTime < 4.f)
		{
			FPlatformProcess::Sleep(.80f);
		}


		ADivebot* Bot = FMaxTestUtils::GetDivebot();

		if (Bot)
		{
			TestTrue("Check if there is a DiveBot is Possessed", Bot->bIsPossessed);
		}


		TestDone.Execute();
	});

	AfterEach([this]()
	{
		FMaxTestUtils::Exit();
	});
}


// Maxime Paradis Unit Test - completed on April 11th, 2024
// The test checks if the player and the boat are in the overworld, if the player can die, if the divebot spawn and is possessed.
// This is important because this is essential for gameplay. If the player can not die and the events that fallow dont there are no stakes or the game loop is broken.