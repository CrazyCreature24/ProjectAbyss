// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DiverCharacter.h"
#include "Components/WidgetComponent.h"
#include "InteractableActor.generated.h"

UCLASS()
class PROJECTABYSS_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

	// COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components")
		class UStaticMeshComponent* MeshComponent;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components")
		class UStaticMeshComponent* OutlineMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components")
		class UBoxComponent* HitboxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "1_Components")
		class UBoxComponent* InteractHitboxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UWidgetComponent* InteractWidget;

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
	UFUNCTION()
		virtual void Interacted(ADiverCharacter* Player);

	UFUNCTION()
	FORCEINLINE void ActivateOutline() { OutlineMeshComponent->SetVisibility(true); }

	UFUNCTION()
	FORCEINLINE void DeActivateOutline() { OutlineMeshComponent->SetVisibility(false); }

	UFUNCTION()
	FORCEINLINE void ActivateText() { InteractWidget->SetVisibility(true); }

	UFUNCTION()
	FORCEINLINE void DeActivateText() { InteractWidget->SetVisibility(false); }

	// VARIABLES
	// Is the object locked?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2_Variables")
		bool bIsLocked = false;

};
/*
ChangeLog:
-----------------------------
Date:	 	Oct 10 2023
Name:		Austin Morris
Summary:	Added base interact functionality
-----------------------------
*/
