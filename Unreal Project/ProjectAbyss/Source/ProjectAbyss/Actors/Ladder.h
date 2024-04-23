// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractableActor.h"
#include "Ladder.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API ALadder : public AInteractableActor
{
	GENERATED_BODY()

	public:
	ALadder();

	//Scene component for top of ladder
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder")
	USceneComponent* Top;

	//Scene component for bottom of ladder
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder")
	USceneComponent* Bottom;

	//Hitbox for ladder speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder")
	UBoxComponent* LadderSpeedBox;

	//Speed box overlap function override
	UFUNCTION()
	void OnLadderSpeedBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//End overlap
	UFUNCTION()
	void OnLadderSpeedBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Interacted(ADiverCharacter* Interactor) override;

	// Tick
	virtual void Tick(float DeltaTime) override;
	
};
