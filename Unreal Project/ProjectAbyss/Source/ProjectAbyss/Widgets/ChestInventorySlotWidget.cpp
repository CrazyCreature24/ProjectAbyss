// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/ChestInventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "StorageChestWidget.h"
#include "Kismet/GameplayStatics.h"
#include "StorageChestWidget.h"	
#include "StorageChest.h"
#include "DataObject.h"
#include "HotbarListWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "EquipmentBase.h"
#include <DiverController.h>


void UChestInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStorageChest::StaticClass(), FoundActors);

	//Make dive cage ref
	if (FoundActors.Num() > 0)
		ParentChestWidget = Cast<AStorageChest>(FoundActors[0])->StorageChestWidget;


	Cast<ADiverController>(GetOwningPlayer())->PlayerMouseUpDelegate.AddUniqueDynamic(this, &UChestInventorySlotWidget::Test);

	IMG_Icon->SetVisibility(ESlateVisibility::Visible);

	IMG_Icon->OnMouseButtonDownEvent.BindUFunction(this, FName("OnSlotClicked"));
}

void UChestInventorySlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bIsSelected)
	{
		ParentChestWidget->UpdateImagePosition();
		
		if (bHasMoved)
		{
			ParentChestWidget->SelectedImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			IMG_Icon->SetVisibility(ESlateVisibility::Hidden);
			bHasMoved = false;
		}
	}
}

void UChestInventorySlotWidget::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseEnter(MyGeometry, MouseEvent);
	UE_LOG(LogTemp, Warning, TEXT("OnMouseEnter"));
	bIsHovered = true;
}

void UChestInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseLeave(MouseEvent);
	UE_LOG(LogTemp, Warning, TEXT("OnMouseLeave"));
	bIsHovered = false;
}

void UChestInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UDataObject* Equipment = Cast<UDataObject>(ListItemObject);
	DataObj = Equipment;
	if (Equipment != nullptr)
	{
		if (Equipment->EquipStruct.Equip)
		{
			IMG_Icon->SetBrushFromTexture(Equipment->EquipStruct.Equip->HotbarImage);
		}
		else
		{
			IMG_Icon->SetBrushFromTexture(EmptySlotImage);
		}
	}
	if (DataObj->EquipStruct.SlotType == ESlotType::Storage)
	{
		ParentChestWidget->InventorySlots.Add(this);
	}
	if (DataObj->EquipStruct.SlotType == ESlotType::Hotbar)
	{
		ParentChestWidget->HotbarInventorySlots.Add(this);
	}

}

void UChestInventorySlotWidget::SetSlot(AEquipmentBase* Equipment)
{
}


FEventReply UChestInventorySlotWidget::OnSlotClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("OnSlotClicked"));
	
	Cast<ADiverController>(GetOwningPlayer())->InputKey(EKeys::LeftMouseButton, EInputEvent::IE_Pressed, 0.0f, false);/////////

	if (IMG_Icon->Brush.GetResourceObject() == EmptySlotImage)
	{
		UE_LOG(LogTemp, Warning, TEXT("EmptySlotImage"));
		return FEventReply(false);
	}

	ParentChestWidget->SelectedSlot = this;

	// If selected slot is a hotbar type, then hide the hotbar image
	if (DataObj->EquipStruct.SlotType == ESlotType::Hotbar)
	{
		//Cast to diver controller
		ADiverController* DiverController = Cast<ADiverController>(ParentChestWidget->OwningDiverCharacter->GetController());
		Cast<UHotbarListWidget>(DiverController->HotbarWidget->HotbarTileView->GetEntryWidgetFromItem(DiverController->HotbarWidget->HotbarTileView->GetItemAt(DataObj->EquipStruct.SlotNum)))->SlotImage->SetBrushFromTexture(EmptySlotImage);
	}
	
	if (DataObj->EquipStruct.Equip)
	if (DataObj->EquipStruct.Equip->HotbarImage)
	{
		ParentChestWidget->SelectedImage->SetBrushFromTexture(DataObj->EquipStruct.Equip->HotbarImage);
	}
	bIsSelected = true;
	bHasMoved = true;
	return FEventReply(true);
}

void UChestInventorySlotWidget::Test()
{
	ParentChestWidget->MouseReleased();
}


//if (ParentChestWidget == nullptr)
//{
//	UE_LOG(LogTemp, Error, TEXT("ParentChestWidget is null!"));
//	return;
//}
//
//if (CurrentEquipment == nullptr)
//{
//	UE_LOG(LogTemp, Error, TEXT("CurrentEquipment is null!"));
//	if (ParentChestWidget->CurrentSelectedEquipment != nullptr)
//	{
//		CurrentEquipment = ParentChestWidget->CurrentSelectedEquipment;
//		ParentChestWidget->CurrentSelectedEquipment = nullptr;
//	}
//}
//
//if (CurrentEquipment != nullptr)
//{
//	if (ParentChestWidget->CurrentSelectedEquipment != nullptr)
//	{
//		// Swap the equipment
//		AEquipmentBase* TempEquipment = CurrentEquipment;
//		CurrentEquipment = ParentChestWidget->CurrentSelectedEquipment;
//		ParentChestWidget->CurrentSelectedEquipment = TempEquipment;
//	}
//	else
//	{
//		// Select the equipment
//		ParentChestWidget->CurrentSelectedEquipment = CurrentEquipment;
//
//	}
//}