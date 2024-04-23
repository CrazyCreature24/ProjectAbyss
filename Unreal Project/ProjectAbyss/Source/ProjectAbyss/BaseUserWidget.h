// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Actors/PCTerminal.h"
#include "BaseUserWidget.generated.h"


class UCanvasPanelSlot;

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//Moves widgets and map
	void UpdateBorder(UUserWidget* widget, UCanvasPanelSlot* slot, double prevMouseX, double prevMouseY, FVector2D currentMouseLoc, float SliderVal = 0);

	//Scales widgets with scaling functionality
	void ScaleRight(UUserWidget* widget, UCanvasPanelSlot* slot, double prevX, FVector2D currentMouseLoc);
	void ScaleLeft(UUserWidget* widget, UCanvasPanelSlot* slot, double prevX, FVector2D currentMouseLoc);
	void ScaleBottom(UUserWidget* widget, UCanvasPanelSlot* slot, double prevY, FVector2D currentMouseLoc);
	void ScaleCorner(UUserWidget* widget, UCanvasPanelSlot* slot, double prevX, double prevY, FVector2D currentMouseLoc, FString cornername);

	//Set all bools to false throughout all PC widgets
	void UnsetBools();

	//Remove and readd widget to bring it to front
	void Flicker(UUserWidget* widget);

	//Moves the cursor using a controller stick
	void MoveCursor(FKey key, APlayerController* player, double stickstate);

	//Called when input with analog value is changed
	FReply NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent) override;

	//Returns reference to PCTerminal
	APCTerminal* GetPC();

	FString FocusedWindow;

	FString GetFocusedWindow();
	void SetFocusedWindow(FString window);

	//
	bool bLeftHeld = false;
	bool bRightHeld = false;
	bool bUpHeld = false;
	bool bDownHeld = false;
	//

	//Bools to stop swapping widgets every tick (for controller support)
	bool bNextSwapOccured = false;
	bool bPreviousSwapOccured = false;

	
	

};
