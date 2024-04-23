// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractableActor.h"
#include "DiveCageLockButton.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API ADiveCageLockButton : public AInteractableActor
{
	GENERATED_BODY()

public:
	ADiveCageLockButton();

	/// <summary>
		/// Virtual function that is called when player interacts with it
		/// </summary>
		/// <param name="Player"> The player that interacted with it.</param>
	virtual void Interacted(ADiverCharacter* Player) override;

	// Dive cage parent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dive Cage")
        class ADiveCage* DiveCage;
	
};
