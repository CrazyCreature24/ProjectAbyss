// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
// Alexander McTernans Unit Test - completed on April 11th, 2024

#include "MyTestUtils.h"
#include "Controllers/BaseFishAIController.h"
#include "Fish.h"
#include <Kismet/GameplayStatics.h>

BEGIN_DEFINE_SPEC(FParrotFishFollowWaypointPatrol, "ProjectAbyss.Alexander-Unit-Tests.ParrotFishFollowWaypointPatrol", FMyTestUtils::TestsFlags)

UWorld* World;
TArray<AActor*> AllFish;
TArray<AFish*> ParrotFish;

END_DEFINE_SPEC(FParrotFishFollowWaypointPatrol)

void FParrotFishFollowWaypointPatrol::Define()
{
	BeforeEach([this]()
	{
		
		AutomationOpenMap("/Game/ProjectAbyss/Maps/Overworld");
		World = FMyTestUtils::GetWorld();
		TestNotNull("Check if World is properly created", World);
	});

	LatentIt("Do the Parrot Fish Waypoint Patrol", EAsyncExecution::ThreadPool, [this](const FDoneDelegate TestDone)
	{
		float StartTime = FPlatformTime::Seconds();
		while (FPlatformTime::Seconds() - StartTime < 5.f)
		{
			FPlatformProcess::Sleep(1.0f);
		}

		// Collect all fish in the level
		AsyncTask(ENamedThreads::GameThread, [this]() {
			
			UGameplayStatics::GetAllActorsOfClass(World, AFish::StaticClass(), AllFish);
			TestTrue("Check if there is at least 1 Fish in the level", AllFish.Num() > 0);
		});
		
		// Isolate the Parrot Fish
		AsyncTask(ENamedThreads::GameThread, [this]() {

			for (AActor* fish : AllFish)
			{
				AFish* temp = Cast<AFish>(fish);
				if (temp)
				{
					if (temp->Name == "Parrotfish")
					{
						ParrotFish.Add(temp);
					}
				}
			}
			TestTrue("Check if there is at least 1 Parrot Fish in the level", ParrotFish.Num() > 0);
			});


		// Check each Parrot fish patrol state
		for (AFish* fish : ParrotFish)
		{
			AsyncTask(ENamedThreads::GameThread, [&]() {

				ABaseFishAIController* control = Cast<ABaseFishAIController>(fish->Controller);

				TestTrue("Check if they are using the Waypoint Patrol state", control->PatrolState == EFishPatrolStates::Waypoint);
				});
		}

		TestDone.Execute();
	});

	AfterEach([this]()
	{
		FMyTestUtils::Exit();
	});
}


// Alexander McTernans Unit Test - completed on April 11th, 2024
// The test checks if the Parrotfish in the Overworld are using WaypointPatrol.
// 