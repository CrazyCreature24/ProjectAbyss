// Copyright Astralwater Interactive. All Rights Reserved.


#include "Gamemodes/MainMenuGamemode.h"
#include "Kismet/GameplayStatics.h"
#include "DiveCage.h"
#include "Blueprint/UserWidget.h"
#include "BigMenuEyeball.h"


AMainMenuGamemode::AMainMenuGamemode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainMenuGamemode::BeginPlay()
{
	Super::BeginPlay();
	CageRef = Cast<ADiveCage>(UGameplayStatics::GetActorOfClass(GetWorld(), ADiveCage::StaticClass()));
	FishEyeRef = Cast<ABigMenuEyeball>(UGameplayStatics::GetActorOfClass(GetWorld(), ABigMenuEyeball::StaticClass()));
}

void AMainMenuGamemode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Uses gameplay statics to find the cage actor and move it up or down
	if (bIsCageMoving)
	{
		
		if (CageRef)
		{
			//Slightly move cage down if its not at the bottom
			if (CageRef->GetActorLocation().Z >= CageLowestPoint)
			{
				CageRef->AddActorLocalOffset(FVector(0, 0, -CageMoveSpeed * DeltaTime));
			}
			else
			{
				if (MainMenuWidgetClass)
				{
					if (test < 1)
					{
						test++;
						//Create the main menu widget and add it to the viewport (only once)
						UUserWidget* MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
						if (MainMenuWidget)
						{
							MainMenuWidget->AddToViewport();
						}
					}
				}

			}
		}
		
	}

	if (bIsFishMoving)
	{
		if (FishEyeRef)
		{
			//Slightly move cage down if its not at the bottom
			if (FishEyeRef->GetActorLocation().Y >= FishFarthestPoint)
			{
				FishEyeRef->AddActorWorldOffset(FVector(0, -FishMoveSpeed * DeltaTime, 0));
			}
		}
	}


}

void AMainMenuGamemode::MoveCageDown()
{
	bIsCageMoving = true;
}

void AMainMenuGamemode::StopCageDown()
{
	bIsCageMoving = false;
}

void AMainMenuGamemode::MoveFishRight()
{
	bIsFishMoving = true;
}

void AMainMenuGamemode::StopMoveFishRight()
{
	bIsFishMoving = false;
}

