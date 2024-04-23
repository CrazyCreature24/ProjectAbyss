// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pawns/Creature.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISightTargetInterface.h"
#include "Fish.generated.h"

/**
 *
 */

class UFishMovementComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;


UCLASS()
class PROJECTABYSS_API AFish : public ACreature, public IAISightTargetInterface
{
	GENERATED_BODY()


public:

	AFish();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components")
	UFishMovementComponent* FishMovementComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components")
	UAIPerceptionComponent* AIPerceptionComp;

	// Ways to Percieve
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Perception")
	bool bCanSee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Perception")
	bool bCanHear; 

	UAISenseConfig_Sight* SightConfig;

	UAISenseConfig_Hearing* HearingConfig;


	// Focus Widget methods
	virtual void SetIsInFocus(bool bInFocus);
	virtual void ShowFocusWidget();
	virtual void HideFocusWidget();

	int FishCollectionIndex = 0;
	bool bIsSwimming = true;
	float JumpTime = 0.33f;
	FTimerHandle SwimTransitionHandle;

	// Adjustable behaviour variables the are sent to the controller
	// Idle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Idle")
	float IdleTime;

	//Fleeing variables 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
	bool bDoesFlee; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Flee", meta = (EditCondition = "bDoesFlee", EditConditionHides))
	float FleeDistance; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Flee", meta = (EditCondition = "bDoesFlee", EditConditionHides)) //If needed
	float FleeSpeed;

	// Attack variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
	bool bDoesAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Attack", meta = (EditCondition = "bDoesAttack", EditConditionHides))
	float AttackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Attack", meta = (EditCondition = "bDoesAttack", EditConditionHides))
	float AttackRange; // May just be handled by the AI Perception comp.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Attack", meta = (EditCondition = "bDoesAttack", EditConditionHides))
	float AttackCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Attack", meta = (EditCondition = "bDoesAttack", EditConditionHides))
	int NumDamageStatesInflicted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Attack", meta = (EditCondition = "bDoesAttack", EditConditionHides))
	float DamageStateLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Attack", meta = (EditCondition = "bDoesAttack", EditConditionHides))
	bool IsAttackPermanent;


	// Patrol variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
	bool bDoesPatrol; // Needs to be added to fish

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol", meta = (EditCondition = "bDoesPatrol", EditConditionHides))
	float DistanceToPatrolPointForgivness;

	// Random Patrol
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol", meta = (EditCondition = "bDoesPatrol", EditConditionHides))
	bool bUseRandomGeneratePatrol; // Needs to be added to fish

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol && bDoesPatrol", EditConditionHides))
	int MaxNumberOfWaypointsToCheckPerFrame; // Needs to be added to fish

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol && bDoesPatrol", EditConditionHides))
	float MinDistance; // Needs to be added to fish

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol && bDoesPatrol", EditConditionHides))
	float MaxDistanceLower; // Needs to be added to fish

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol && bDoesPatrol", EditConditionHides))
	float MaxDistanceHigher; // Needs to be added to fish

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol && bDoesPatrol", EditConditionHides))
	bool bGoesIdleOnReachingRandomGenPoint; // Needs to be added to fish

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol && bDoesPatrol", EditConditionHides))
	int MaxRandomPointsReachToSwitchToWaypoints; // Needs to be added to fish


	// Waypoint Patrol
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol", meta = (EditCondition = "bDoesPatrol", EditConditionHides))
	bool bUseWaypointPatrol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Waypoint", meta = (EditCondition = "bUseWaypointPatrol && bDoesPatrol", EditConditionHides))
	TArray<AActor*> PatrolWaypoints; // Needs to be added to fish

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Called when the game starts or when spawned
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Debug tool to help with perception component
	/// </summary>
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};


/*
ChangeLog:
---------------------------- -
Date : November 11
Name : Jarod Beach
Summary : Class Created
---------------------------- -
Date : November and December
Name : Jarod Beach
Summary : We don't know what happened
---------------------------- -
Date : November 11
Name : Jarod Beach and Alex McTernan
Summary : Perception works. Attack mode is now implmented at base. OnTargetPerceptionUpdated is created.
---------------------------- -
Date : January 13th
Name : Alexander McTernan
Summary : Added variables that the AI controller now uses to adjust behaviour.
---------------------------- -
Date : January 13th
Name : Jarod Beach
Summary : Commenting
---------------------------- -
Date : January 15th
Name : Alexander McTernan
Summary : Moved the State switch statement to the ABaseFishAIController
---------------------------- -
Date : January 16th
Name : Alexander McTernan
Summary : Added more behaviour variables
---------------------------- -
Date : January 18th
Name : Alexander McTernan
Summary :	A bunch more Flee, Attack and Patrol variables have been added and have cooresponding variables in the BaseFishController class.
			All the variables have been organized in the blueprints to bee visible when they are needed, based on behaviours available.
			Add an OnHit function to deal with the fish getting stuck in walls.
---------------------------- -
Date :		January 23rd
Name :		Alexander McTernan
Summary :	Set up the Hearing component
---------------------------- -
Date :		Feb 6th
Name :		Alexander McTernan
Summary :	Made UPROPERTY variables for increased health state manipulation
---------------------------- -
*/