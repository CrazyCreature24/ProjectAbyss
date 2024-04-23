// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractableActor.h"
#include "LightSwitch.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API ALightSwitch : public AInteractableActor
{
	GENERATED_BODY()

	public:

	//Off static mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* OffMesh;

	public: 
	ALightSwitch();

	/// <summary>
	/// Virtual function that is called when player interacts with it
	/// </summary>
	/// <param name="Player"> The player that interacted with it.</param>
	void Interacted(ADiverCharacter* Player) override;


	//Attached array of lights to turn on and off
	UPROPERTY(EditAnywhere, Category = "Light Switch")
	TArray<class AActor*> Lights;

	protected:

	virtual void BeginPlay() override;


	
};
