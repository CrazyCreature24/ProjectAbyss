// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentBase.h"
#include "InventoryComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTABYSS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Array to hold the inventory items
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<AEquipmentBase*> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	AEquipmentBase* CamInv;

	// Maximum number of inventory slots
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Designer")
	int32 MaxInventorySlots;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsInventoryFull();

	// Function to add an item to the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddToInventory(AEquipmentBase* Equipment);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		bool AddToHotbarInventory(AEquipmentBase* Equipment);

		UFUNCTION(BlueprintCallable, Category = "Inventory")
		bool AddCamera(AEquipmentBase* Equipment);

	// Function to remove an item from the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveFromInventory(int32 Index);

	// Function to swap two items in the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool SwapEquipment(int32 IndexA, int32 IndexB);

	// Swaps equipment between two inventories (used for storage chests)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		bool SwapEquipmentBetweenInventories(UInventoryComponent* OtherInventory, int32 IndexA, int32 IndexB);

	// Function to get an item at a specific index in the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AEquipmentBase* GetEquipmentAtIndex(int32 Index);

	// Function to get equipment by type
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AEquipmentBase* GetEquipmentByType(TSubclassOf<AEquipmentBase> EquipmentType);

	// Set equipment at index
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SetEquipmentAtIndex(int32 Index, AEquipmentBase* Equipment);

	// Function to drop the currently equipped item
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropEquipment(int32 Index);

	// Change image of hotbar for current slot
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ChangeHotbarImage(int32 Index, AEquipmentBase* Equipment);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EmptyInventory();

	/// <summary>
	/// Shows the active equipments mesh and makes all the others not visible.
	/// </summary>
	/// <param name="Index"> Brings in the index of the equipment you want to make visible </param>
	void SetEquipmentVisiblility(uint32 Index);
};

/*
ChangeLog:
-----------------------------
Date		Nov 14 2023
Name:		Austin Morris
Summary:	Created base inventory component class with simple functions to add, remove, and swap items in the inventory.
-----------------------------
Date		Nov 14 2023
Name:		Alexander McTernan
Summary:	Fixed bug that only allowed you to add one item to you inventory. Was in the AddToInventory().
			Fixed bug that doesn't actually remove element from Inventory properly. Inventory.RemoveAt(Index) doesn't function this way, it actually shrinks the array. Instead I just set the element to nullptr.
			Also made it remove the image from the hotbar when it is removed from the array.
-----------------------------
Date		Nov 20 2023
Name:		Alexander McTernan
Summary:	Added SetEquipmentVisibility() so now it only shows the active equipments mesh.
-----------------------------
*/

