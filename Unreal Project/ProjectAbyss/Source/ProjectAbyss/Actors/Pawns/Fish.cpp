// Copyright Astralwater Interactive. All Rights Reserved.


#include "Actors/Pawns/Fish.h"
#include "FishPointActor.h"
#include "FishMovementComponent.h"
#include "BaseFishAIController.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "DiverCharacter.h"

#include "Components/BoxComponent.h"

AFish::AFish()
{
	// Perception behaviours
	bCanSee = true;
	bCanHear = true;

	// Idle
	IdleTime = 3.0f;

	// Attack
	bDoesAttack = true;
	AttackCooldown = 3.0f;
	AttackTime = 1.0f;
	AttackRange = 200.0f;

	NumDamageStatesInflicted = 1.0f;
	DamageStateLength = 5.0f;
	IsAttackPermanent = false;

	// Flee
	FleeDistance = 300.0f;

	// Patrol
	bDoesPatrol = true;

	// Patrol (Random)
	bUseRandomGeneratePatrol = true;
	DistanceToPatrolPointForgivness = 200.0f;
	MaxNumberOfWaypointsToCheckPerFrame = 40;
	MinDistance = 250;
	MaxDistanceLower = 700;
	MaxDistanceHigher = 800;
	bGoesIdleOnReachingRandomGenPoint = false;
	MaxRandomPointsReachToSwitchToWaypoints = 5;

	// Patrol (Waypoint)
	bUseWaypointPatrol = false;

	CollisionBox->OnComponentHit.AddDynamic(this, &AFish::OnHit);

	// Tag Setup
	Tags.Add("Fish");

	FishMovementComponent = CreateDefaultSubobject<UFishMovementComponent>(TEXT("Fish Movement"));

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));


	if (AIPerceptionComp)
	{
		// Configure sight sense
		SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
		SightConfig->SightRadius = 1000.0f;
		SightConfig->LoseSightRadius = 2000.0f;
		SightConfig->PeripheralVisionAngleDegrees = 45.0f;
		SightConfig->SetMaxAge(5.0f);

		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;


		AIPerceptionComp->ConfigureSense(*SightConfig);

		// Configure Hearing perception
		HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing"));
		HearingConfig->HearingRange = 3000.0f;
		HearingConfig->SetMaxAge(5.0f);

		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

		AIPerceptionComp->ConfigureSense(*HearingConfig);

		AIPerceptionComp->SetDominantSense(UAISense_Sight::StaticClass());

		// Activate after configuring
		AIPerceptionComp->Activate();

		UE_LOG(LogTemp, Warning, TEXT("AFish BeginPlay - AI Perception configured"));
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
}

void AFish::SetIsInFocus(bool bInFocus)
{
	ACreature::SetIsInFocus(bInFocus);
}

void AFish::ShowFocusWidget()
{
	ACreature::ShowFocusWidget();
}

void AFish::HideFocusWidget()
{
	ACreature::HideFocusWidget();
}



void AFish::BeginPlay()
{
	Super::BeginPlay();
}

void AFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AFish::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			// The pawn successfully sensed something
			UE_LOG(LogTemp, Warning, TEXT("%s saw %s!"), *GetName(), *Actor->GetName());

			// Access additional information about the perceived actor
			if (Actor->IsA(ADiverCharacter::StaticClass()))
			{
				ADiverCharacter* FoundPlayer = Cast<ADiverCharacter>(Actor);
				if (FoundPlayer)
				{

				}
			}
		}
	}
}

void AFish::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector normal = Hit.Normal;

	SetActorLocation(GetActorLocation() + normal * 20.0f);
}
