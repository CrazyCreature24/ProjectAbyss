// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "BoatWheel.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API ABoatWheel : public AInteractableActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABoatWheel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	 /// <summary>
	 /// Virtual function that is called when player interacts with it
	 /// </summary>
	 /// <param name="Player"> The player that interacted with it.</param>
	 void Interacted(ADiverCharacter* Player) override;

};
/*
ChangeLog:
-----------------------------
Date:	 	Oct 10 2023
Name:		Austin Morris
Summary:	Added boat wheel base class with basic input functionality.
-----------------------------
-----------------------------
Date:	 	Nov 11 2023
Name:		Austin Morris
Summary:	Added check to see if locked & other networking changes.
-----------------------------
*/
