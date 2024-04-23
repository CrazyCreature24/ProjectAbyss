// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <ChestInventorySlotWidget.h>
#include <EquipmentBase.h>
#include <HotbarWidget.h>
#include "Components/TileView.h"
#include "Controllers/DiverController.h"
#include "StorageChestWidget.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UStorageChestWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UFUNCTION(NetMulticast, Reliable)
	void RefreshChestInventory();


	void SetOwningStorageChest(class AStorageChest* StorageChest);
	FVector2D GetMousePosition();
	void UpdateImagePosition();
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FReply NativeOnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION()
		void MouseReleased();

	UPROPERTY(meta = (BindWidget))
	UTileView* TileViewWidget;

	UPROPERTY(meta = (BindWidget))
	UTileView* HotbarTileViewWidget;

	UHotbarWidget* HotbarWidget;

	// Array of all the slots in the inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<UChestInventorySlotWidget*> InventorySlots;

	// Array of all the slots in the inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<UChestInventorySlotWidget*> HotbarInventorySlots;

	// Currently selected slot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UChestInventorySlotWidget* SelectedSlot;

	// Stored imaged
	UPROPERTY(meta = (BindWidget))
	class UImage* SelectedImage;

	UPROPERTY(meta = (BindWidget))
	class UBorder* SelectedBorder;

	AStorageChest* OwningStorageChest;
	ADiverCharacter* OwningDiverCharacter;

	//FPlayerMouseUp PlayerMouseUpDelegate;

};
