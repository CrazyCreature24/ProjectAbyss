// Copyright Astralwater Interactive. All Rights Reserved.


#include "Components/InventoryComponent.h"
#include "DiverCharacter.h"
#include "DiverController.h"
#include "PhotographyCamera.h"
#include <StorageChest.h>

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...


	//Set the default inventory to be empty with the max number of slots
	Inventory.SetNum(MaxInventorySlots);
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UInventoryComponent::IsInventoryFull()
{
	//If any slot is nullptr, return true
	for (int32 Index = 0; Index < Inventory.Num(); ++Index)
	{
		if (Inventory[Index] == nullptr)
		{
			return false;
		}
	}
	return true;


}

bool UInventoryComponent::AddToInventory(AEquipmentBase* Equipment)
{
	if (Equipment)
	{
		// Iterate through the inventory array to find an empty slot
		for (int32 Index = 0; Index < Inventory.Num(); ++Index)
		{
			if (Inventory[Index] == nullptr)
			{
				// Found an empty slot, replace it with the new equipment
				Inventory[Index] = Equipment;

				return true;
			}
		}

	}

	// No empty slot found, return false
	return false;
}

bool UInventoryComponent::AddToHotbarInventory(AEquipmentBase* Equipment)
{
	if (Equipment)
	{
		// Iterate through the inventory array to find an empty slot
		for (int32 Index = 0; Index < Inventory.Num(); ++Index)
		{
			if (Inventory[Index] == nullptr)
			{
				// Found an empty slot, replace it with the new equipment
				Inventory[Index] = Equipment;
				ChangeHotbarImage(Index, Equipment);
				return true;
			}
		}

	}

	// No empty slot found, return false
	return false;
}

bool UInventoryComponent::AddCamera(AEquipmentBase* Equipment)
{
	if (Equipment)
	{
		
		if (CamInv == nullptr)
		{
			CamInv = Equipment;
			ChangeHotbarImage(0, Equipment);
			return true;
		}
		
	}

	return false;
}

bool UInventoryComponent::RemoveFromInventory(int32 Index)
{
	if (Index >= 0 && Index < Inventory.Num())
	{
		// Change the hotbar image to the default image
		ChangeHotbarImage(Index, nullptr);
		Inventory[Index] = nullptr;
		return true;
	}

	return false;
}

bool UInventoryComponent::SwapEquipment(int32 IndexA, int32 IndexB)
{
	if (IndexA >= 0 && IndexA < Inventory.Num() && IndexB >= 0 && IndexB < Inventory.Num())
	{
		Inventory.Swap(IndexA, IndexB);
		ChangeHotbarImage(IndexA, Inventory[IndexA]);
		ChangeHotbarImage(IndexB, Inventory[IndexB]);
	}

	return false;
}

bool UInventoryComponent::SwapEquipmentBetweenInventories(UInventoryComponent* OtherInventory, int32 IndexA, int32 IndexB)
{
	// Swaps equipment between two inventories (used for storage chests)
	if (IndexA >= 0 && IndexA < Inventory.Num() && IndexB >= 0 && IndexB < OtherInventory->Inventory.Num())
	{
		AEquipmentBase* Temp = Inventory[IndexA];
		Inventory[IndexA] = OtherInventory->Inventory[IndexB];
		OtherInventory->Inventory[IndexB] = Temp;
		ChangeHotbarImage(IndexA, Inventory[IndexA]);

		//Attach the equipment to the new inventories owner
		if (Inventory[IndexA])
		{
			Inventory[IndexA]->Equip(GetOwner());
			ADiverCharacter* PlayerPawn = Cast<ADiverCharacter>(GetOwner());
			if (PlayerPawn)
			{
				ADiverController* PlayerController = Cast<ADiverController>(PlayerPawn->GetController());
				if (PlayerController)
				{
					if (IndexA == PlayerController->HeldEquipment)
					{
						SetEquipmentVisiblility(IndexA);
					}
				}
			}

		}
		if (OtherInventory->Inventory[IndexB])
		{
			OtherInventory->Inventory[IndexB]->Equip(OtherInventory->GetOwner());
			ADiverCharacter* PlayerPawn = Cast<ADiverCharacter>(GetOwner());
			if (PlayerPawn)
			{
				ADiverController* PlayerController = Cast<ADiverController>(PlayerPawn->GetController());
				if (PlayerController)
				{
					if (IndexB == PlayerController->HeldEquipment)
					{
						SetEquipmentVisiblility(IndexB);
					}
				}
			}
		}

		return true;
	}
	return false;
}

AEquipmentBase* UInventoryComponent::GetEquipmentAtIndex(int32 Index)
{
	// Check if the index is valid
	if (Index >= 0 && Index < Inventory.Num())
	{
		return Inventory[Index];
	}
	else if (Index == 10)
	{
		return CamInv;
	}

	return nullptr;
}

AEquipmentBase* UInventoryComponent::GetEquipmentByType(TSubclassOf<AEquipmentBase> EquipmentType)
{
	// Iterate through the inventory array to find the equipment
	for (AEquipmentBase* Equipment : Inventory)
	{
		if (Equipment && Equipment->IsA(EquipmentType))
		{
			return Equipment;
		}
	}
	return nullptr;
}

void UInventoryComponent::SetEquipmentAtIndex(int32 Index, AEquipmentBase* Equipment)
{
	// Check if the index is valid
	if (Index >= 0 && Index < Inventory.Num())
	{
		Inventory[Index] = Equipment;
	}
}

void UInventoryComponent::DropEquipment(int32 Index)
{
	// Check if the index is valid
	if (Index >= 0 && Index < Inventory.Num())
	{
		// Get the equipment at the index
		AEquipmentBase* Equipment = Inventory[Index];
		if (Equipment)
		{
			Inventory.Remove(Equipment);
			//Implement dropping the equipment here
		}
	}

}

void UInventoryComponent::ChangeHotbarImage(int32 Index, AEquipmentBase* Equipment)
{

	// Cast to player pawn
	ADiverCharacter* PlayerPawn = Cast<ADiverCharacter>(GetOwner());
	if (PlayerPawn)
	{
		ADiverController* PlayerController = Cast<ADiverController>(PlayerPawn->GetController());
		if (PlayerController)
		{
			// If the equipment is not null, set the image to the equipment's hotbar image
			if (Equipment)
			{
				if (Cast<APhotographyCamera>(Equipment))
				{
					PlayerController->HotbarWidget->CamSlot->SetBrushFromTexture(Equipment->HotbarImage);
				}
				else
				{
					PlayerController->HotbarWidget->SetImage(Index, Equipment->HotbarImage);
				}
			}
			// If the equipment is null, set the image to null
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Setting image to null"));
				PlayerController->HotbarWidget->SetImage(Index, PlayerController->HotbarWidget->DefaultTexture);
			}
		}
	}
	
}

void UInventoryComponent::EmptyInventory()
{
	int Index = 0;
	for (AEquipmentBase* e : Inventory)
	{
		// Change the hotbar image to the default image
		ChangeHotbarImage(Index, nullptr);
		e = nullptr;
		Index++;
	}
}

void UInventoryComponent::SetEquipmentVisiblility(uint32 Index)
{
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		if (Inventory[i] == nullptr)
		{
			continue;
		}


		if (Inventory[i] == Inventory[Index])
		{
			Inventory[i]->SetActorHiddenInGame(false);
			Inventory[i]->SetIsHeld(true);
		}
		else
		{
			Inventory[i]->SetActorHiddenInGame(true);
			Inventory[i]->SetIsHeld(false);
		}
	}
}
