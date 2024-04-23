// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseUserWidget.h"
#include "ShopUserWidget.generated.h"


class UDataObject;

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UShopUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:

	//Reveals the minimized widget
	UFUNCTION()
	void OnHotbarButtonClicked();

	
protected:

	virtual void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//Refreshes information in shop
	UFUNCTION()
	void RefreshShop();

	//Button handling
	UFUNCTION()
	void CloseButtonClicked();
	UFUNCTION()
	void MinimizeButtonClicked();
	UFUNCTION()
	void BuyButtonClicked();

	//Widget movement
	UFUNCTION()
	FEventReply ClickBorderTest(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:

	//Text Blocks
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDetails;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerMoney;

	//Icon of shop item
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemIcon;

	//Item player has selected in the shop
	class AEquipmentBase* SelectedItem;

	TArray<std::reference_wrapper<bool>> Bools{ bBorderIsClicked };

protected:

	//Buttons
	UPROPERTY(meta = (BindWidget))
		class UButton* CloseButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* MinimizeButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* BuyButton;

	//List view with shop items
	UPROPERTY(meta = (BindWidget))
		class UListView* ShopListView;

	//Border for moving widget
	UPROPERTY(meta = (BindWidget))
		class UBorder* MoveBorder;

	//Box containing all other elements
	UPROPERTY(meta = (BindWidget))
		class UScaleBox* ContainingScaleBox;

	//Animation for minimizing widget
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* MinimizeAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class APhotographyCamera> CameraClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ADeployableLight> LightClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ASoundDecoy> SoundDecoyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ASeascooter> SeaScooterClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AFishFinder> FishFinderClass;

	//References
	class APCTerminal* PC;
	class AStorageChest* StorageChest;

	//Storage for shop equipment
	TArray<AEquipmentBase*> ShopEquipment;

	//Bools
	bool bBorderIsClicked = false;
	bool bEquipmentAdded = false;


	double PreviousMouseX;
	double PreviousMouseY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* temp;

};

/*
-----------------------------
Date		Feb 22 2024
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Added a CallObjectiveID in the BuyButtonClicked Method using "BuyEquipmentID". For a tutorial mission
-----------------------------
*/