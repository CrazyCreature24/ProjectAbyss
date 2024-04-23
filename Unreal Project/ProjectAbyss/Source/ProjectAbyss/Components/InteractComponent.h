// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTABYSS_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// Performs a raytrace of interaction. If it hits an interactable object, it will call the interact function on that object.
	/// </summary>
	/// <param name="Player"> Who is currently interacting</param>
	UFUNCTION()
		void Interact(ADiverCharacter* Player);


private:
	// Distance for the raycast
	UPROPERTY(EditAnywhere, Category = "3_Variables")
		float RaycastDistance = 300.f;
		
	UPROPERTY(EditAnywhere, Category = "3_Variables")
		UMaterialInstance* OutlineMat;
	
	class AInteractableActor* LastHitActor = nullptr;

};
/*
ChangeLog:
-----------------------------
Date:	 	Oct 18 2023
Name:		Austin Morris
Summary:	Simple interact functionality created.
-----------------------------
*/
