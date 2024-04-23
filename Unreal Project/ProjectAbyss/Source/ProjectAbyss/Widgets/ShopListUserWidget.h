// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DataObject.h"
#include "ShopListUserWidget.generated.h"

/**
 * 
 */

class UImage;
class UTextBlock;
class UButton;
UCLASS()
class PROJECTABYSS_API UShopListUserWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	//Shows details of item when it is clicked
	UFUNCTION()
	void ShowDetails();

protected:

	void NativeConstruct() override;

	//Called when an item is added to the list/tree view of the shop
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:

	//Reference to the object for the widget
	UDataObject* OwnedObject;

	//The button for the list item in the shop
	UPROPERTY(meta = (BindWidget))
		UButton* ShopButton;

	//The icon for the list item in the shop
	UPROPERTY(meta = (BindWidget))
		UImage* ShopEquipmentIcon;

	//The name and price of the list item in the shop
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ShopEquipmentTitle;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ShopPriceText;

	//Price of the item
	UPROPERTY(meta = (BindWidget))
		int ShopEquipmentPrice;

	

protected:

	//Reference to PCTerminal
	class APCTerminal* PC;

};
