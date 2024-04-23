// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/StorageChestWidget.h"
#include "Widgets/ChestInventorySlotWidget.h"
#include "InventoryComponent.h"
#include "DiverCharacter.h"

#include "DataObject.h"
#include "DeployableLight.h"
#include "StorageChest.h"
#include "InventoryComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "DiverCharacter.h"
#include "Components/Button.h"


// To any developer seeing this in the future, I'm sorry. I'm so sorry.
// I'm sure there's a better way to do this, but I'm not sure what it is.
// I'm not sure if this is the best way to do it, but it works.
// I'm sorry.
// Add 1 to the tried to fix this counter.
// 1
// -AM

void UStorageChestWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	//Cast to owning player controller
	ADiverController* pController = Cast<ADiverController>(GetOwningPlayer());
	HotbarWidget = pController->HotbarWidget;

	//Store player pawn ref
	OwningDiverCharacter = Cast<ADiverCharacter>(GetOwningPlayerPawn());

	SetFocus();
	
}

void UStorageChestWidget::RefreshChestInventory_Implementation()
{
	TileViewWidget->ClearListItems();
	HotbarTileViewWidget->ClearListItems();

	InventorySlots.Empty();
	HotbarInventorySlots.Empty();
	for (int i = 0; i < OwningStorageChest->Inventory->Inventory.Num(); i++)
	{
		// Create a new widget for each item in the inventory
		UDataObject* DataObject = NewObject<UDataObject>(UDataObject::StaticClass());
		DataObject->Init(OwningStorageChest->Inventory->Inventory[i], i, ESlotType::Storage);
		TileViewWidget->AddItem(DataObject);

		UE_LOG(LogTemp, Warning, TEXT("Added item to tileview"));
	}	
	for (int i = 0; i < OwningDiverCharacter->InventoryComponent->Inventory.Num(); i++)
	{
		// Create a new widget for each item in the inventory	
		UDataObject* DataObject = NewObject<UDataObject>(UDataObject::StaticClass());
		DataObject->Init(OwningDiverCharacter->InventoryComponent->Inventory[i], i, ESlotType::Hotbar);
		HotbarTileViewWidget->AddItem(DataObject);

		UE_LOG(LogTemp, Warning, TEXT("Added item to tileview"));
	}
	
	
}

void UStorageChestWidget::SetOwningStorageChest(AStorageChest* StorageChest)
{
	OwningStorageChest = StorageChest;

	RefreshChestInventory();
}

FVector2D UStorageChestWidget::GetMousePosition()
{
	// Get the mouse position
	FVector2D MousePosition;
	MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	return MousePosition;
}

void UStorageChestWidget::UpdateImagePosition()
{	
	// Get the mouse position
	FVector2D MousePosition = GetMousePosition();
	
	Cast<UCanvasPanelSlot>(SelectedImage->Slot)->SetPosition(MousePosition);
}

void UStorageChestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}

FReply UStorageChestWidget::NativeOnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	//If input is right click, dont do anything
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		return FReply::Handled();
	}

	UE_LOG(LogTemp, Warning, TEXT("Mouse button up"));
	if (SelectedSlot)
	{
		SelectedSlot->bIsSelected = false;
		// If the mouse is over the hotbar, add the item to the hotbar
		bool isHoveringAnything = false;
		for (int i = 0; i < InventorySlots.Num(); i++)
		{
			if (InventorySlots.IsValidIndex(i) && InventorySlots[i]->bIsHovered == true)
			{
				
				//Hotbar to storage
				if (SelectedSlot->DataObj->EquipStruct.SlotType == ESlotType::Hotbar)
				{
					UE_LOG(LogTemp, Warning, TEXT("Hotbar to storage"));
					UChestInventorySlotWidget* TempSlot = InventorySlots[i];
					InventorySlots[i]->DataObj->EquipStruct.Equip = SelectedSlot->DataObj->EquipStruct.Equip;
					//InventorySlots[i]->DataObj->EquipStruct.SlotType = SelectedSlot->DataObj->EquipStruct.SlotType;
					HotbarInventorySlots[SelectedSlot->DataObj->EquipStruct.SlotNum]->DataObj->EquipStruct.Equip = TempSlot->DataObj->EquipStruct.Equip;
					//HotbarInventorySlots[SelectedSlot->DataObj->EquipStruct.SlotNum]->DataObj->EquipStruct.SlotType = TempSlot->DataObj->EquipStruct.SlotType;
					//HotbarWidget->HotbarImages[SelectedSlot->DataObj->EquipStruct.SlotNum]->SetBrushFromTexture(HotbarInventorySlots[SelectedSlot->DataObj->EquipStruct.SlotNum]->DataObj->EquipStruct.Equip->HotbarImage);

					OwningDiverCharacter->InventoryComponent->SwapEquipmentBetweenInventories(OwningStorageChest->Inventory, SelectedSlot->DataObj->EquipStruct.SlotNum, i);
					// Needs to unequip the item from the diver and equip it to the chest

				}
				// Storage to storage
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Storage to storage"));
					UChestInventorySlotWidget* TempSlot = InventorySlots[i];
					InventorySlots[i]->DataObj->EquipStruct.Equip = SelectedSlot->DataObj->EquipStruct.Equip;
					//InventorySlots[i]->DataObj->EquipStruct.SlotType = SelectedSlot->DataObj->EquipStruct.SlotType;
					InventorySlots[SelectedSlot->DataObj->EquipStruct.SlotNum]->DataObj->EquipStruct.Equip = TempSlot->DataObj->EquipStruct.Equip;
					//InventorySlots[SelectedSlot->DataObj->EquipStruct.SlotNum]->DataObj->EquipStruct.SlotType = TempSlot->DataObj->EquipStruct.SlotType;

					OwningStorageChest->Inventory->SwapEquipment(i, SelectedSlot->DataObj->EquipStruct.SlotNum);

				}

			}
		}
		for (int i = 0; i < HotbarInventorySlots.Num(); i++)
		{
			if (HotbarInventorySlots.IsValidIndex(i) && HotbarInventorySlots[i]->bIsHovered == true)
			{
				
				// Storage to hotbar
				if (SelectedSlot->DataObj->EquipStruct.SlotType == ESlotType::Storage)
				{
					UE_LOG(LogTemp, Warning, TEXT("Swapping item from storage to hotbar"));
					UChestInventorySlotWidget* TempSlot = HotbarInventorySlots[i];
					HotbarInventorySlots[i]->DataObj->EquipStruct.Equip = SelectedSlot->DataObj->EquipStruct.Equip;
					//HotbarInventorySlots[i]->DataObj->EquipStruct.SlotType = SelectedSlot->DataObj->EquipStruct.SlotType;
					InventorySlots[SelectedSlot->DataObj->EquipStruct.SlotNum]->DataObj->EquipStruct.Equip = TempSlot->DataObj->EquipStruct.Equip;
					//InventorySlots[SelectedSlot->DataObj->EquipStruct.SlotNum]->DataObj->EquipStruct.SlotType = TempSlot->DataObj->EquipStruct.SlotType;

					OwningDiverCharacter->InventoryComponent->SwapEquipmentBetweenInventories(OwningStorageChest->Inventory, i, SelectedSlot->DataObj->EquipStruct.SlotNum);
					


				}
				// Hotbar to hotbar
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Swapping item from hotbar to hotbar"));
					UChestInventorySlotWidget* TempSlot = HotbarInventorySlots[i];
					HotbarInventorySlots[i]->DataObj->EquipStruct.Equip = SelectedSlot->DataObj->EquipStruct.Equip;
					//HotbarInventorySlots[i]->DataObj->EquipStruct.SlotType = SelectedSlot->DataObj->EquipStruct.SlotType;
					HotbarInventorySlots[SelectedSlot->DataObj->EquipStruct.SlotNum]->DataObj->EquipStruct.Equip = TempSlot->DataObj->EquipStruct.Equip;
					//HotbarInventorySlots[SelectedSlot->DataObj->EquipStruct.SlotNum]->DataObj->EquipStruct.SlotType = TempSlot->DataObj->EquipStruct.SlotType;
					


					OwningDiverCharacter->InventoryComponent->SwapEquipment(i, SelectedSlot->DataObj->EquipStruct.SlotNum);

				}


				//UE_LOG(LogTemp, Warning, TEXT("Mouse is over slot %i"), i);
				//SelectedImage->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		if (isHoveringAnything == false)
		{
			if (SelectedSlot)
			{
				// If the mouse is not over the hotbar, drop the item
				UE_LOG(LogTemp, Warning, TEXT("Mouse is not over slot"));
				SelectedSlot->bIsSelected = false;
				SelectedSlot->IMG_Icon->SetVisibility(ESlateVisibility::Visible);
				if (SelectedSlot->DataObj->EquipStruct.SlotType == ESlotType::Hotbar)
				{
					HotbarWidget->HotbarTileView->RegenerateAllEntries();
				}
				SelectedSlot = nullptr;
			}
		}
		SelectedImage->SetVisibility(ESlateVisibility::Hidden);

		RefreshChestInventory();
	}
	

	return FReply::Handled();
}

FReply UStorageChestWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{

	//If key is escape, close the inventory
	if (InKeyEvent.GetKey() == FKey("Escape"))
	{
		Cast<ADiverController>(GetOwningPlayer())->StartRecentlyInMenusTimer();
		GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
		GetOwningPlayer()->SetShowMouseCursor(false);
		RemoveFromParent();
	}

	if (InKeyEvent.GetKey() == FKey("Tab"))
	{
		GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
		GetOwningPlayer()->SetShowMouseCursor(false);
		RemoveFromParent();
		
	}

	return FReply::Handled();
}

void UStorageChestWidget::MouseReleased()
{


	

}


