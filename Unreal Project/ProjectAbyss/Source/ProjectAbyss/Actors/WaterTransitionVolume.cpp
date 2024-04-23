// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/WaterTransitionVolume.h"
#include "Components/BoxComponent.h"
#include "Actors/Pawns/DiverCharacter.h"
#include "StateTransitionInterface.h"
#include "MainGameState.h"
#include "Divebot.h"
#include "DiverMovementComponent.h"
#include "Boat.h"


// Sets default values
AWaterTransitionVolume::AWaterTransitionVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject <UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetCollisionProfileName("OverlapAllDynamic");
	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWaterTransitionVolume::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AWaterTransitionVolume::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AWaterTransitionVolume::BeginPlay()
{
	Super::BeginPlay();
	
	if (bIsMainOceanBody == true)
	{

		AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

		if (MyGameState)
		{
			MyGameState->SetWaterVolume(this);
		}

	}

}

// Called every frame
void AWaterTransitionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> Result;
	GetOverlappingActors(Result);

	AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

	//if (MyGameState)
	//{
	//	if(MyGameState->GetPlayer()->MyMovementComponent->MovementMode == MOVE_Swimming)
	//		MyGameState->GetPlayer()->MyMovementComponent->SetMovementMode(MOVE_Walking);
	//}

	bool isPlayerInWater = false;

	for (AActor* a : Result)
	{
		IStateTransitionInterface* myInterface = Cast<IStateTransitionInterface>(a);
		if (myInterface)
		{
			myInterface->SetToIsSwimming();
			if (myInterface == MyGameState->GetPlayer())
				isPlayerInWater = true;
		}

		if (bIsFirstTick)
		{

			if ( Cast<ADiverCharacter>(a))
			{
				MyGameState->SpawnDivebotNearDiver();
			}
		}

	}

	if(!isPlayerInWater)
		if(MyGameState->GetPlayer()->MyMovementComponent->MovementMode == MOVE_Swimming)
			MyGameState->GetPlayer()->MyMovementComponent->SetMovementMode(MOVE_Walking);

	if (bIsFirstTick)
		bIsFirstTick = false;

}

void AWaterTransitionVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IStateTransitionInterface* A = Cast<IStateTransitionInterface>(OtherActor);

	if (A)
	{
		A->TransitionMovementStates();
	}

	if (Cast<ADiverCharacter>(OtherActor) && bIsMainOceanBody)
	{
		AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

		if (MyGameState && !MyGameState->GetBot())
		{
			MyGameState->SpawnDivebot();
		}
		else if (MyGameState)
		{
			//if(GetWorld()->GetMapName() != L"UEDPIE_0_OverWorld") // Was used to stop crash when walking in a cave
			//	MyGameState->GetBot()->CancelGetToShip();
		}
	}
}

void AWaterTransitionVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IStateTransitionInterface* A = Cast<IStateTransitionInterface>(OtherActor);

 	if (A)
	{
		A->TransitionMovementStates();
	}

	if (Cast<ADiverCharacter>(OtherActor) && bIsMainOceanBody)
	{
		AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

		if (MyGameState)
		{
			if (MyGameState->GetBot() != nullptr)
			{
				FString mapname = GetWorld()->GetMapName();
				if(MyGameState->GetBoat())
 					MyGameState->GetBot()->GetToShip(MyGameState->GetBoat()->GetDivebotPointLocation());
			}
		}
	}
}

