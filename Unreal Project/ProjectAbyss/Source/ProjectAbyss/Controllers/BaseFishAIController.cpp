// Copyright Astralwater Interactive. All Rights Reserved.


#include "Controllers/BaseFishAIController.h"

#include "FishPointActor.h"
#include "FishMovementComponent.h"
#include "Fish.h"
#include "Actors/Pawns/DiverCharacter.h"
#include "HealthStateComponent.h"
#include "WaterTransitionVolume.h"

#include "Components/AudioComponent.h"
#include "SoundDecoy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "MainGameState.h"
#include <Kismet/GameplayStatics.h>

ABaseFishAIController::ABaseFishAIController()
{
	bDidStateChangeLastFrame = false;

	bCanSee = true;
	bCanHear = true;

	IdleTime = 3.0f;
	bIsIdleTimerActivated = false;

	bDoesAttack = true;
	bAttackCooldownComplete = true;
	AttackCooldown = 3.0f;
	AttackTime = 1.0f;
	AttackRange = 200.0f;

	NumDamageStatesInflicted = 1.0f;
	DamageStateLength = 5.0f;
	IsAttackPermanent = false;


	FleeDistance = 300.0f;

	bDoesPatrol = true; // Add to Begin Play
	PatrolState = EFishPatrolStates::Random;
	bUseRandomGeneratePatrol = true;
	DistanceToPatrolPointForgivness = 200.0f;
	MaxNumberOfWaypointsToCheckPerFrame = 20;
	MinDistance = 250;
	MaxDistanceLower = 700;
	MaxDistanceHigher = 800;
	bGoesIdleOnReachingRandomGenPoint = false;
	CurrentRandomPointsReached = 0;
	MaxRandomPointsReachToSwitchToWaypoints = 5;

	bUseWaypointPatrol = false;

	State = EFishStates::Patrol;
}


FVector ABaseFishAIController::NewPointLocation()
{
	// This generates a random location for the fish to travel too.
	FVector FishyLocation = ReferenceFish->GetActorLocation();

	FVector RandPoint = UKismetMathLibrary::RandomUnitVector();

	RandPoint *= UKismetMathLibrary::RandomFloatInRange(MinDistance, UKismetMathLibrary::RandomFloatInRange(MaxDistanceLower, MaxDistanceHigher));
	RandPoint.Z = FMath::Clamp(RandPoint.Z, -100, 100);
	RandPoint += FishyLocation;

	return RandPoint;
}

void ABaseFishAIController::CreatePoint()
{
	// Creates the initial waypoint object for the fish. Location will need to be set somewhere else
	if (RandomWaypoint == nullptr)
	{
		FVector Spawnpoint = { 0,0,0 };
		FRotator BaseRot = { 0,0,0 };

		if(fishPointTemplate)
			RandomWaypoint = Cast<AFishPointActor>(GetWorld()->SpawnActor(fishPointTemplate, &Spawnpoint, &BaseRot));
		else
			RandomWaypoint = GetWorld()->SpawnActor<AFishPointActor>(AFishPointActor::StaticClass(), Spawnpoint, BaseRot);
	}
}

bool ABaseFishAIController::bRaycastToPoint(const FVector& waypoint)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(ReferenceFish);
	CollisionParams.AddIgnoredActor(RandomWaypoint);

	FVector FishLocation = ReferenceFish->GetActorLocation();
	FVector WaypointLocation = waypoint;

	//Draw debug line
	//DrawDebugLine(GetWorld(), FishLocation, WaypointLocation, FColor::Red, false, 1.f, 0, 1.f);

	bool IsThereObstacle = GetWorld()->SweepSingleByChannel(HitResult, FishLocation, WaypointLocation, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(25.0f), CollisionParams);

	if (!CheckIfPointIsInWaterVolume())
	{
		return false;
	}

	if (IsThereObstacle == false)
	{
		return true;
	}

	return false;
}

void ABaseFishAIController::GenerateNewRandomPoint()
{
	if (RandomWaypoint == nullptr)
	{
		CreatePoint();
	}

	FVector InitialWaypointLocation = NewPointLocation();
	RandomWaypoint->SetActorLocation(InitialWaypointLocation);

	int newWaypointCheckCounter = 0;

	while (bRaycastToPoint(RandomWaypoint->GetActorLocation()) == false)
	{
		FVector NewWaypointLocation = NewPointLocation();
		RandomWaypoint->SetActorLocation(NewWaypointLocation);

		newWaypointCheckCounter++;

		if (newWaypointCheckCounter > MaxNumberOfWaypointsToCheckPerFrame) // For checking for a new point that is not in a wall
		{
			RandomWaypoint->SetActorLocation(ReferenceFish->GetActorForwardVector() * -100);
			break; //Will change to spawnpoint 
		}
	}
}

void ABaseFishAIController::Idle(float DeltaTime)
{
	// Log the start of the idle state
	//UE_LOG(LogTemp, Warning, TEXT("Fish is now in Idle state"));

	if (CheckIfAttackIsActivated() == true)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		bSoundDecoyIsPercieved = false;
		
		TimeElapsed += DeltaTime;

		if (TimeElapsed > IdleTime)
		{
			SetCurrentState(EFishStates::Patrol);
			
			TimeElapsed = 0.0f;
		}
	}
	else
	{
		// Log an error if the world is not valid
		UE_LOG(LogTemp, Error, TEXT("Idle function: Invalid World"));
	}


}

void ABaseFishAIController::Patrol(float DeltaTime)
{
	// This function manages how the fish is patroling

	if (ReferenceFish->bIsSwimming != true)
		return;

	if (CheckIfAttackIsActivated() == true)
	{
		return;
	}


	if (bUseWaypointPatrol == true && PatrolWaypoints.IsEmpty() == false && PatrolState == EFishPatrolStates::Waypoint)
	{
		WaypointPatrol(DeltaTime);
	}
	else if (PatrolWaypoints.IsEmpty() == true) // If there are no waypoints, set to random gen by default
	{
		PatrolState = EFishPatrolStates::Random;
	}

	if (bUseRandomGeneratePatrol == true && PatrolState == EFishPatrolStates::Random)
	{
		RandomGenerationPatrol(DeltaTime);
	}
	else if (PatrolState != EFishPatrolStates::Waypoint)
	{
		// Defaults to idle state if patrols are not active
		SetCurrentState(EFishStates::Idle);
	}
}

void ABaseFishAIController::WaypointPatrol(float DeltaTime)
{
	if (PatrolWaypoints.IsEmpty() == true)
	{
		// If not waypoints are available, return to random generation. Should not be hit, but it is a backup.
		PatrolState = EFishPatrolStates::Random;
		bDidStateChangeLastFrame = true;
		return;
	}

	// Setting target on Fish movement component
	FVector TargetWaypointLocation = PatrolWaypoints[WaypointIndex]->GetActorLocation();
	ReferenceFish->FishMovementComponent->SetTaregtLocation(TargetWaypointLocation);

	// If fish is close to the target waypoint, choose next point.
	float distanceToWaypoint = (ReferenceFish->GetActorLocation() - TargetWaypointLocation).Length();

	if (distanceToWaypoint < DistanceToPatrolPointForgivness)
	{
		WaypointIndex++;

		if (WaypointIndex >= PatrolWaypoints.Num())
		{
			WaypointIndex = 0;
		}

		// If also using the random generate patrol, switch to it now
		if (bUseRandomGeneratePatrol == true)
		{
			PatrolState = EFishPatrolStates::Random;
			bDidStateChangeLastFrame = true;
		}
	}

}

void ABaseFishAIController::RandomGenerationPatrol(float DeltaTime)
{
	// This allows us to create initial point or generate a new point from changing states
	if (RandomWaypoint == nullptr || bDidStateChangeLastFrame == true)
	{
		GenerateNewRandomPoint();
		bDidStateChangeLastFrame = false;
	}

	// This stops the fish from driving into walls. Does need to be optimized.
	if (bRaycastToPoint(RandomWaypoint->GetActorLocation()) == false)
	{
		GenerateNewRandomPoint();
	}

	ReferenceFish->FishMovementComponent->SetTaregtLocation(RandomWaypoint->GetActorLocation());

	float distanceToWaypoint = (ReferenceFish->GetActorLocation() - RandomWaypoint->GetActorLocation()).Length();

	if (distanceToWaypoint < DistanceToPatrolPointForgivness)
	{
		CurrentRandomPointsReached++;

		// If you want the fish to go into the idle state when reaching a random generated point
		if (bGoesIdleOnReachingRandomGenPoint == true)
		{
			SetCurrentState(EFishStates::Idle);
			bDidStateChangeLastFrame = true;
			return; // TODO: Currently cannot be use with Waypoint Patrol
		}


		// If also using waypoint patrol, switch to it. If not, generate a new point
		if (PatrolWaypoints.IsEmpty() == false && bUseWaypointPatrol == true && CurrentRandomPointsReached >= MaxRandomPointsReachToSwitchToWaypoints)
		{
			PatrolState = EFishPatrolStates::Waypoint;
			CurrentRandomPointsReached = 0;
		}
		else
		{
			GenerateNewRandomPoint();
		}
	}
}

bool ABaseFishAIController::CheckIfAttackIsActivated()
{
	TArray<AActor*> PerceivedActors;

	if (bDoesAttack == true)
	{
		ReferenceFish->AIPerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	}

	if (PerceivedActors.Num() > 0 && bAttackCooldownComplete == true)
	{
		SetCurrentState(EFishStates::Attack);
		bDidStateChangeLastFrame = true;
		return true;
	}

	return false;
}

AActor* ABaseFishAIController::CheckIfSoundHeard()
{
	TArray<AActor*> PerceivedActors;
	ReferenceFish->AIPerceptionComp->GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(), PerceivedActors);

	// If not percieved actors, returns null
	if (PerceivedActors.IsEmpty() == true)
	{
		return nullptr;
	}

	float maxDistance = MAX_FLT;
	AActor* savedActor = nullptr;

	// Cycles through all detected actors and saves the closest one to the fish. If one is the sound decoy, it takes priority
	for (auto actor : PerceivedActors)
	{
		float distanceFromFish = ReferenceFish->GetDistanceTo(actor);

		if (Cast<ASoundDecoy>(actor))
		{
			return actor;
		}
		else if (distanceFromFish < maxDistance)
		{
			maxDistance = distanceFromFish;

			if(Cast<ADiverCharacter>(actor))
				if(Cast<ADiverCharacter>(actor)->bIsSwiming)
					savedActor = actor;
		}
	}

	return savedActor;
}

void ABaseFishAIController::Attack(float DeltaTime)
{
	if (bAttackCooldownComplete == false)
		return;

	TArray<AActor*> PerceivedActors;
	TArray<AActor*> HeardActors;
	ReferenceFish->AIPerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	ReferenceFish->AIPerceptionComp->GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(), HeardActors);
	if (PerceivedActors.Num() <= 0)
	{
		State = EFishStates::Idle;
		return;
	}

	for (AActor* PerceivedActor : PerceivedActors)
	{
		ADiverCharacter* PerceivedPlayer = Cast<ADiverCharacter>(PerceivedActor);
		if (PerceivedPlayer == nullptr)
		{
			continue;
		}
		if (HeardActors.FindItemByClass<ASoundDecoy>())
		{
			for (AActor* HeardActor : HeardActors)
			{
				if (Cast<ASoundDecoy>(HeardActor))
				{
					ReferenceFish->FishMovementComponent->SetTaregtLocation((HeardActor->GetActorLocation()));

				}
			}

		}
		else if (PerceivedPlayer->bIsSwiming)
		{
			ReferenceFish->FishMovementComponent->SetTaregtLocation(PerceivedPlayer->GetActorLocation());
			if ((ReferenceFish->GetActorLocation() - PerceivedPlayer->GetActorLocation()).Length() < AttackRange)
			{
				PerceivedPlayer->HealthComponent->TakeDamage(IsAttackPermanent, DamageStateLength, NumDamageStatesInflicted, ReferenceFish->GetActorLocation(), ReferenceFish->KnockBack);
				State = EFishStates::Patrol;
				bAttackCooldownComplete = false;

				FTimerHandle AttackTimerHandle;

				// Use a lambda function to set the state directly
				GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, [this]() {
					bAttackCooldownComplete = true;
					}, AttackCooldown, false);
			}
		}
		else
		{
  			State = EFishStates::Patrol;
			WaypointPatrol(DeltaTime);
		}
	}

}

void ABaseFishAIController::Flee(float DeltaTime)
{
	AActor* heardSound = CheckIfSoundHeard();

	if (heardSound == nullptr)
	{
		State = EFishStates::Patrol;
		return;
	}

	FVector deltaVector = ReferenceFish->GetActorLocation() - heardSound->GetActorLocation();

	FVector direction = (deltaVector).GetSafeNormal();

	FVector targetLocation = ReferenceFish->GetActorLocation() + direction * 100;

	bool bCanReachPoint = bRaycastToPoint(targetLocation);

	if (bCanReachPoint == true)
	{
		ReferenceFish->FishMovementComponent->SetTaregtLocation(targetLocation);
	}
	else
	{
		ReferenceFish->FishMovementComponent->SetTaregtLocation(heardSound->GetActorLocation());
	}

	//Create a point away from player.
	if ((deltaVector).Length() < FleeDistance)
	{
		State = EFishStates::Patrol;
	}


}

void ABaseFishAIController::Alert(float DeltaTime)
{
	// Log the start of the idle state
	//UE_LOG(LogTemp, Warning, TEXT("Fish is now in Alert state"));

	AActor* percievedActor = CheckIfSoundHeard();


	if (percievedActor == nullptr)
	{
		SetCurrentState(EFishStates::Patrol);
		bSoundDecoyIsPercieved = false;
		ReferenceFish->FishMovementComponent->Speed = SavedFishSpeed;
		return;
	}

	if (CheckIfAttackIsActivated() == true)
	{
		ReferenceFish->FishMovementComponent->Speed = SavedFishSpeed;
		return;
	}

	ReferenceFish->FishMovementComponent->Speed = 0;

	FVector soundLocation = percievedActor->GetActorLocation();

	// I have to go through the movement component to deal with rotation to avoid rotation conflicts
	ReferenceFish->FishMovementComponent->SetTaregtLocation(soundLocation);

}

bool ABaseFishAIController::CheckIfPointIsInWaterVolume(const FVector& waypoint)
{

	AMainGameState* MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AMainGameState>() : nullptr;

	if (MyGameState)
	{
		AWaterTransitionVolume* WaterVol = MyGameState->GetWaterVolume();

		if (WaterVol)
		{
			if (PatrolState == EFishPatrolStates::Waypoint) 
			{
				// Way point patrol does not use the FishPoint class at all and neither does the flee, so I had to hack in this fix.
				// Had to change so the waypoint patrol flee behaviour did not allow the fish to surface.
				AFishPointActor* temp = GetWorld()->SpawnActor<AFishPointActor>(fishPointTemplate, waypoint, FRotator(0, 0, 0));

				TArray<AActor*> Result;
				WaterVol->GetOverlappingActors(Result, fishPointTemplate);

				if (Result.Num())
				{
					for (AActor* a : Result)
					{
						if (a == temp)
						{
							temp->Destroy();
							return true;
						}
					}
				}

				temp->Destroy();
			}
			else
			{
				TArray<AActor*> Result;
				WaterVol->GetOverlappingActors(Result, fishPointTemplate);

				if (Result.Num())
				{
					for (AActor* a : Result)
					{
						if (a == RandomWaypoint)
						{
							int bp = 0;
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

void ABaseFishAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ReferenceFish == nullptr)
	{
		ReferenceFish = Cast<AFish>(GetInstigator());
	}

	SavedFishSpeed = ReferenceFish->FishMovementComponent->Speed;

	// Need to be set with variables from the possessed fish, this allows for each fish to be customized
	// Perception
	bCanHear = ReferenceFish->bCanHear;
	bCanSee = ReferenceFish->bCanSee;


	// Idle
	IdleTime = ReferenceFish->IdleTime;

	// Flee
	bDoesFlee = ReferenceFish->bDoesFlee;
	FleeDistance = ReferenceFish->FleeDistance;
	FleeSpeed = ReferenceFish->FleeSpeed;

	// Attack
	AttackTime = ReferenceFish->AttackTime;
	AttackRange = ReferenceFish->AttackRange;
	DistanceToPatrolPointForgivness = ReferenceFish->DistanceToPatrolPointForgivness;
	bDoesAttack = ReferenceFish->bDoesAttack;
	AttackCooldown = ReferenceFish->AttackCooldown;

	NumDamageStatesInflicted = ReferenceFish->NumDamageStatesInflicted;
	DamageStateLength = ReferenceFish->DamageStateLength;
	IsAttackPermanent = ReferenceFish->IsAttackPermanent;

	// Patrol
	bDoesPatrol = ReferenceFish->bDoesPatrol;

	// Patrol (Random)
	bUseRandomGeneratePatrol = ReferenceFish->bUseRandomGeneratePatrol;
	MaxNumberOfWaypointsToCheckPerFrame = ReferenceFish->MaxNumberOfWaypointsToCheckPerFrame;
	MinDistance = ReferenceFish->MinDistance;
	MaxDistanceLower = ReferenceFish->MaxDistanceLower;
	MaxDistanceHigher = ReferenceFish->MaxDistanceHigher;
	bGoesIdleOnReachingRandomGenPoint = ReferenceFish->bGoesIdleOnReachingRandomGenPoint;
	MaxRandomPointsReachToSwitchToWaypoints = ReferenceFish->MaxRandomPointsReachToSwitchToWaypoints;

	// Patrol (Waypoint)
	bUseWaypointPatrol = ReferenceFish->bUseWaypointPatrol;

	if (ReferenceFish->PatrolWaypoints.IsEmpty() == false)
	{
		PatrolWaypoints = ReferenceFish->PatrolWaypoints;
	}


	if (bDoesPatrol == true)
	{
		if (bUseRandomGeneratePatrol == true)
		{
			PatrolState = EFishPatrolStates::Random;
		}

		if (bUseWaypointPatrol == true)
		{
			PatrolState = EFishPatrolStates::Waypoint;
		}
		// If both patrols are available, we want it to start with waypoint, and then it will switch back and fourth
	}
	else
	{
		bUseWaypointPatrol = false;
		bUseRandomGeneratePatrol = false;
	}

}

void ABaseFishAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetControlRotation(ReferenceFish->GetActorRotation());

	// If not attacking, check if the fish hears a sound
	if (bCanHear && State != EFishStates::Attack)
	{
		AActor* percievedActor = CheckIfSoundHeard();

		if (percievedActor != nullptr)
		{
			if (bDoesFlee == false)
			{
				SetCurrentState(EFishStates::Alert);
			}
			else
			{
				SetCurrentState(EFishStates::Flee);
			}
		}
	}

	switch (State)
	{
	case EFishStates::Idle:
		Idle(DeltaTime);
		break;
	case EFishStates::Attack:
		Attack(DeltaTime);
		break;
	case EFishStates::Patrol:
		Patrol(DeltaTime);
		break;
	case EFishStates::Alert:
		Alert(DeltaTime);
		break;
	case EFishStates::Flee:
		Flee(DeltaTime);
		break;
	}
}

void ABaseFishAIController::OnPossess(APawn* aPawn)
{
	ReferenceFish = Cast<AFish>(aPawn);

	ReferenceFish->Controller = this;

}

void ABaseFishAIController::SetCurrentState(EFishStates NewState)
{
	if (ReferenceFish != nullptr)
	{
		State = NewState;
	}
}

EFishStates ABaseFishAIController::GetCurrentState() const
{
	return State;
}
