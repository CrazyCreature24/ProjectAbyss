// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "HotbarWidget.generated.h"

class UTileView;

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UHotbarWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	void NativeConstruct() override;

	// Set image to the texture passed in
	void SetImage(int32 HotbarSlot, UTexture2D* texture);

	// Changes current selected slot border color to red
	void SetSelectedSlot(int32 HotbarSlot);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hotbar")
		UTexture2D* DefaultTexture;

	int HotbarSize = 4;

	TArray<class UImage*> HotbarImages;
	TArray<class UBorder*> HotbarBorders;

	UPROPERTY(meta = (BindWidget))
	UTileView* HotbarTileView;

	UPROPERTY(meta = (BindWidget));
	UImage* CamSlot;
	/*UPROPERTY(meta = (BindWidget));
	UImage* Slot2;
	UPROPERTY(meta = (BindWidget));
	UImage* Slot3;
	UPROPERTY(meta = (BindWidget));
	UImage* Slot4;*/

	UPROPERTY(meta = (BindWidget));
	UBorder* CamSlotBorder;
	/*UPROPERTY(meta = (BindWidget));
	UBorder* Slot2Border;
	UPROPERTY(meta = (BindWidget));
	UBorder* Slot3Border;
	UPROPERTY(meta = (BindWidget));
	UBorder* Slot4Border;*/
};
