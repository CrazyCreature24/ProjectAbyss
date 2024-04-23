// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseFishAIController.generated.h"

/**
 *
 */
UENUM()
enum class EFishStates
{
	Idle,
	Patrol,
	Flee,
	Attack,
	Alert,
	NumStates
};


UENUM()
enum class EFishPatrolStates
{
	Random,
	Waypoint,
	NumPatrolStates
};


UCLASS()
class PROJECTABYSS_API ABaseFishAIController : public AAIController
{
	GENERATED_BODY()


public:

	ABaseFishAIController();

	// This is used for the idle timer
	float TimeElapsed = 0.0f;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer")
	class AFish* ReferenceFish;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Designer")
	EFishStates State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Perception")
	class UAIPerceptionComponent* AIPerceptionComp;

	// Ways to Percieve
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Perception")
	bool bCanSee; // Needed in fish

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Perception")
	bool bCanHear; // Needed in fish

	float SavedFishSpeed;


	// This makes it so the idle timer is only called once
	bool bDidStateChangeLastFrame;

	bool bSoundDecoyIsPercieved = false;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Designer|Attack", meta = (EditCondition = "bDoesAttack", EditConditionHides))
	bool bAttackCooldownComplete; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Attack", meta = (EditCondition = "bDoesAttack", EditConditionHides))
	float AttackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Attack", meta = (EditCondition = "bDoesAttack", EditConditionHides))
	float AttackRange;

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
	bool bDoesPatrol;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Designer|Patrol", meta = (EditCondition = "bDoesPatrol", EditConditionHides))
	EFishPatrolStates PatrolState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol", meta = (EditCondition = "bDoesPatrol", EditConditionHides))
	float DistanceToPatrolPointForgivness;


	// Random patrol point generation variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol", meta = (EditCondition = "bDoesPatrol", EditConditionHides))
	bool bUseRandomGeneratePatrol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol", EditConditionHides))
	class AFishPointActor* RandomWaypoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol", EditConditionHides))
	int MaxNumberOfWaypointsToCheckPerFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol", EditConditionHides))
	float MinDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol", EditConditionHides))
	float MaxDistanceLower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol", EditConditionHides))
	float MaxDistanceHigher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol", EditConditionHides))
	bool bGoesIdleOnReachingRandomGenPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol", EditConditionHides))
	int CurrentRandomPointsReached;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol", EditConditionHides))
	int MaxRandomPointsReachToSwitchToWaypoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Random", meta = (EditCondition = "bUseRandomGeneratePatrol", EditConditionHides))
	TSubclassOf<AActor> fishPointTemplate;

	// Used for spline patrol over random point generation. Will have the option to do either.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol", meta = (EditCondition = "bDoesPatrol", EditConditionHides))
	bool bUseWaypointPatrol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Waypoint", meta = (EditCondition = "bUseWaypointPatrol", EditConditionHides))
	TArray<AActor*> PatrolWaypoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Designer|Patrol|Waypoint", meta = (EditCondition = "bUseWaypointPatrol", EditConditionHides))
	int WaypointIndex;


	/// <summary>
	/// This Method generates a new Random Point to be used to target in the RandomGeneration Patrol state.
	/// </summary>
	FVector NewPointLocation();

	/// <summary>
	/// Spawns the initial RandomWaypoint.
	/// </summary>
	void CreatePoint();

	/// <summary>
	/// Raycast to make sure the fish can reach the point. Returns a bool.
	/// </summary>
	bool bRaycastToPoint(const FVector& waypoint);

	/// <summary>
	/// Generates a valid Random Point, using the CreatePoint(), bRaycastToPoint(), and NewPointLocation() methods.
	/// </summary>
	void GenerateNewRandomPoint();

	/// <summary>
	/// Executes what is required in the idle state. Also checks to see if it can attack.
	/// </summary>
	void Idle(float DeltaTime);

	/// <summary>
	/// Checks to see if it can attack. Then it sets the Patrol state of the fish, either calling WapointPatrol or RandomGenerationPatrol.
	/// </summary>
	void Patrol(float DeltaTime);

	/// <summary>
	/// If there are no vaild waypoints to travel too, Switch teh patrol state to Random. Then it handles which waypoint the fish is traveling towards via the WaypointIndex. 
	/// If bUseRandomGeneratePatrol is true, set the patrol state to be Random when it reaches a point.
	/// </summary>
	void WaypointPatrol(float DeltaTime);

	/// <summary>
	/// If this is the first time this function is called at all or since a state change, Generate a new point.
	/// Then handle its target location. Generate a new point if the old one is reached. If it has reached the required number of points, switch the state to Waypoint.
	/// </summary>
	void RandomGenerationPatrol(float DeltaTime);

	/// <summary>
	/// This method checks to see if it percieves players by Sight nearby. If so, change the state to Attack.
	/// </summary>
	bool CheckIfAttackIsActivated();

	/// <summary>
	/// This method checks to see if it percieves sound nearby. If so, change the state to Alert.
	/// </summary>
	/// <returns>Closest actor or the player takes priority</returns>
	AActor* CheckIfSoundHeard();

	/// <summary>
	/// This method handles the attack state. If there are percieved actors, the fish will target it. An a successful hit, deals damage and then sets a timer on the attack cooldown
	/// </summary>
	void Attack(float DeltaTime);

	/// <summary>
	/// Not fully implemented yet
	/// </summary>
	void Flee(float DeltaTime);

	/// <summary>
	/// When the fish hearws a sound, it turns towards the sound and stops moving for the duration
	/// </summary>
	void Alert(float DeltaTime);

	virtual bool CheckIfPointIsInWaterVolume(const FVector& waypoint = FVector(0,0,0));

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* aPawn) override;

	/// <summary>
	/// Set the current state of the fish
	/// </summary>
	void SetCurrentState(EFishStates NewState);

	/// <summary>
	/// Get the current state of the fish 
	/// </summary>
	EFishStates GetCurrentState() const;

	bool bIsIdleTimerActivated;
	FTimerHandle IdleTimerHandle;
	FTimerDelegate IdleTimerDel;
	FTimerDelegate AttackTimerDel;

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
Summary :	Perception works. Attack mode is now implmented at base, meaning it moves towards player, then when it reaches them, goes idle, then patrols.
			Next steps, Clean up variables. Need to make them accessable in the blueprint. All Methods need to be commented with the summary code (See Deployable Light.h)
---------------------------- -
Date : January 13th
Name : Alex McTernan
Summary :	Removed dead variables. Added some new behaviour variables and reogranized some so we know more what is going on.
			Also made it so variables are copied from the Fish to the controller, this makes it so each fish can have its own behaviours. Not complete, but started.
---------------------------- -
Date : January 13th
Name : Jarod Beach
Summary : Commenting, category changes, Starting fleeing State.
---------------------------- -
Date :		January 15th
Name :		Alexander McTernan
Summary :	Moved the State switch statement into Tick.
			bRaycastToPoint has been optimized to remove a raycast.
			Player also now takes damage in the Attack method.
---------------------------- -
Date :		January 16th
Name :		Alexander McTernan
Summary :	Added some attack variables and made it so you can turn off the attack function.
			Optimized Patrol and Attack methods and renamed some variables to make it more readable.
---------------------------- -
Date :		January 18th
Name :		Alexander McTernan
Summary :	Random generated patrol points are now generated on the first frame the fish has switched to the patrol state.
			Made different patrol states. Waypoint and Random. If both are activated via the booleans. The fish will move to a waypoint, random generate for a set time, then return to waypoint patrol
			Separated some code into methods to make the code more acessable and used for specific purposes. Ex: GenerateNewRandomPoint() and CheckIfAttackIsActivated().
			Gave the ability to turn off the different behaviours via bools. Idle, attack, Patrol, etc.
			The idle timer is now only set once, and the fish will now attack from the idle state.
			A bunch more Flee, Attack and Patrol variables have been added and have cooresponding variables in the Fish.h class.
			UPDATE: Fixed the fish getting stuck in the wall by making a raycast for each fish every frame. Thsi will need to be optimized.
---------------------------- -
Date :		January 25th
Name :		Alexander McTernan
Summary :	Added the IsSoundHeard() method and the Alert state. In the alert state, the fish will stop moving and rotate towards the sound it heard.
---------------------------- -
Date :		January 27th
Name :		Alexander McTernan
Summary :	Fish hearing now takes priority to a Sound Decoy instead of the player.
			Controller actually rotates with the fish actor now. So now the sight cone rotates with the fish.
			Flee functionality works!! I will be making it adjust the speed, but base functionality works.
---------------------------- -
Date :		Feb 6th
Name :		Alexander McTernan
Summary :	Made UPROPERTY variables for increased health state manipulation
---------------------------- -
Date :		Feb 13th
Name :		Alexander McTernan
Summary :	Had to change the idle timer to be a manuel one because the timers started lasting past world end, causinga the game to crash
---------------------------- -
Date :		Feb 21st
Name :		Alexander McTernan
Summary :	Turned off UE_Logs
---------------------------- -
Date :		March 12th
Name :		Alexander McTernan
Summary :	Fixed fish flee waypoint state issue where the the fish would flee toward the player.
---------------------------- -
Date :		March 13th
Name :		Alexander McTernan
Summary :	Fixed a created bug from the above fix. Now it spawns a fishpoint actor to do the check instead of a blanket statement.
---------------------------- -
*/

