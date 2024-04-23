// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DiverController.h"
#include "ChestInventorySlotWidget.generated.h"

class UDataObject;
class UStorageChestWidget;
/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UChestInventorySlotWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public: 
	//Construct
	void NativeConstruct() override;

	// Tick
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// On hovered
	void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void SetSlot(class AEquipmentBase* Equipment);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	UTexture2D* EmptySlotImage;

	UFUNCTION()
	FEventReply OnSlotClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UFUNCTION()
		void Test();

	UDataObject* DataObj;

	// Parent chest widget
	UStorageChestWidget* ParentChestWidget;

	bool bIsSelected = false;
	bool bIsHovered = false;
	bool bHasMoved = false;

	UPROPERTY(meta = (BindWidget))
	class UImage* IMG_Icon;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* Overlay1;
};
