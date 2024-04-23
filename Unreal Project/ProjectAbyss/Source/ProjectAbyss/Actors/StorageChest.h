// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractableActor.h"
#include "Components/InventoryComponent.h"
#include "StorageChestWidget.h"
#include "StorageChest.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API AStorageChest : public AInteractableActor
{
	GENERATED_BODY()

public: 
	// Sets default values for this actor's properties
	AStorageChest();

	// Inventory component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		class UInventoryComponent* Inventory;


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

	//Storage chest widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> StorageChestWidgetClass;

	//Storage chest widget instance
	class UStorageChestWidget* StorageChestWidget;


	//TSUBCLASS for TESTING
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		TSubclassOf<class APhotographyCamera> EquipmentClass;

	//TSUBCLASS for TESTING
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		TSubclassOf<class ADeployableLight> EquipmentClass1;

	//TSUBCLASS for TESTING
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<class AFishFinder> EquipmentClass2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<class ASeascooter> EquipmentClass3;
};
/*
ChangeLog:
-----------------------------
Date:	 	Nov 18 2023
Name:		Austin Morris
Summary:	Added storage chest class with interacted function
-----------------------------
*/
