// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/Ladder.h"
#include "Actors/Pawns/DiverCharacter.h"
#include "DiverController.h"
#include "Components/BoxComponent.h"
// Sets default values
ALadder::ALadder()
{
	PrimaryActorTick.bCanEverTick = false;

	//Create the top scene component
	Top = CreateDefaultSubobject<USceneComponent>(TEXT("Top"));
	//Create the bottom scene component
	Bottom = CreateDefaultSubobject<USceneComponent>(TEXT("Bottom"));

	//Create the ladder speed box
	LadderSpeedBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LadderSpeedBox"));
	//Attach the ladder speed box to the root component
	LadderSpeedBox->SetupAttachment(RootComponent);
	//Set the size of the ladder speed box
	LadderSpeedBox->SetBoxExtent(FVector(50, 50, 100));
	LadderSpeedBox->SetCollisionProfileName("OverlapAll");

	//Bind the overlap function to the ladder speed box
	LadderSpeedBox->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnLadderSpeedBoxOverlap);
	LadderSpeedBox->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnLadderSpeedBoxEndOverlap);
}

void ALadder::OnLadderSpeedBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADiverCharacter* Diver = Cast<ADiverCharacter>(OtherActor);
	if (Diver)
	{
		//Get controller of player by casting to diver controller
		ADiverController* PlayerController = Cast<ADiverController>(Diver->GetController());

		if (PlayerController)
		{
			//Set the player's ladder speed to the ladder speed
			PlayerController->StartClimb();
		}
	}
}

void ALadder::OnLadderSpeedBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ADiverCharacter* Diver = Cast<ADiverCharacter>(OtherActor);
	if (Diver)
	{
		//Get controller of player by casting to diver controller
		ADiverController* PlayerController = Cast<ADiverController>(Diver->GetController());

		if (PlayerController)
		{
			//Set the player's ladder speed to the ladder speed
			PlayerController->StopClimb();
		}
	}
}

void ALadder::Interacted(ADiverCharacter* Interactor)
{
	//Get controller of player by casting to diver controller
	ADiverController* PlayerController = Cast<ADiverController>(Interactor->GetController());

	if (PlayerController)
	{
		
	}

}

void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
