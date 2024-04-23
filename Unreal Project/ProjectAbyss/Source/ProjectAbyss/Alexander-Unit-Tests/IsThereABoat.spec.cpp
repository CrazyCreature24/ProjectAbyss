// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
// Alexander McTernans Unit Test - completed on April 5th, 2024

#include "MyTestUtils.h"
#include "Actors/Pawns/Boat.h"

BEGIN_DEFINE_SPEC(FIsThereABoat, "ProjectAbyss.Alexander-Unit-Tests.IsThereABoat", FMyTestUtils::TestsFlags)

UWorld* World;

END_DEFINE_SPEC(FIsThereABoat)

void FIsThereABoat::Define()
{
	BeforeEach([this]()
	{
			// Uncomment line below to make it test properly
		AutomationOpenMap("/Game/ProjectAbyss/Maps/TutorialLevel");
		World = FMyTestUtils::GetWorld();
		TestNotNull("Check if World is properly created", World);
	});

	LatentIt("Is there a Boat", EAsyncExecution::ThreadPool, [this](const FDoneDelegate TestDone)
	{

		AsyncTask(ENamedThreads::GameThread, [this]() {
			int32 BoatCount = 0;
			for (TActorIterator<AActor> It(World, ABoat::StaticClass()); It; ++It)
			{
				BoatCount++;
			}
			TestTrue("Check if there is a boat in the level", BoatCount == 1);
		});
		

		float StartTime = FPlatformTime::Seconds();
		while (FPlatformTime::Seconds() - StartTime < 5.f)
		{
			FPlatformProcess::Sleep(1.0f);
		}

		

		TestDone.Execute();
	});

	AfterEach([this]()
	{
		FMyTestUtils::Exit();
	});
}


// Alexander McTernans Unit Test - completed on April 5th, 2024
// The test checks if the Tutorial level has a boat object in it.
// This is important because this is essential for gameplay progress. this is because it contains the PC Terminal.