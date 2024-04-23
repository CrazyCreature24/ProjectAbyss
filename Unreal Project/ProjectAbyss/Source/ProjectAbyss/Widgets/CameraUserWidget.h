// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Actors/PCTerminal.h"
#include "Actors/DiveCage.h"
#include "Actors/Pawns/Boat.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "DiverController.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/GridPanel.h"
#include "Components/SizeBox.h"
#include "Components/ScaleBox.h"
#include "Components/Slider.h"
#include "Components/CanvasPanelSlot.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/WidgetAnimationEvents.h"
#include "BaseUserWidget.h"
#include "CameraUserWidget.generated.h"

class UOverlay;
class ACameraActor;
class UImage;
class UButton;
class UBorder;
/**
 * 
 */



UCLASS()
class PROJECTABYSS_API UCameraUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:

	void NativeConstruct() override;

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	

	//For moving the widget
	UFUNCTION()
	void ClickBorderTest();

	//Minimize the widget
	UFUNCTION()
	void OnMinimize();

	//Button handling
	UFUNCTION()
	void OnHotbarButtonClicked();
	UFUNCTION()
	void CloseButtonClicked();
	UFUNCTION()
	void MarkerButtonClicked();
	UFUNCTION()
	void CaveButtonClicked();
	UFUNCTION()
	void CloseDetailsClicked();
	UFUNCTION()
	void FossilButtonClicked();
	UFUNCTION()
	void KelpButtonClicked();
	UFUNCTION()
	void ShallowsButtonClicked();


	UFUNCTION()
	void FossilAnimFinished();
	UFUNCTION()
	void KelpAnimFinished();
	UFUNCTION()
	void ShallowsAnimFinished();

	UFUNCTION()
	void FossilBorderClicked();
	UFUNCTION()
	void KelpBorderClicked();
	UFUNCTION()
	void ShallowsBorderClicked();

	//Map functionality
	UFUNCTION()
	void MapSliderMoved(float Value);
	UFUNCTION(BlueprintCallable)
	float GetSliderVal();
	UFUNCTION()
	void ZoomMap(const FPointerEvent& InMouseEvent);
	UFUNCTION()
	FEventReply MapClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

protected:

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<APCTerminal> Terminal;

	//Slider for zooming map
	UPROPERTY(meta = (BindWidget))
		USlider* MapSlider;

	//Border clicked for moving widget 
	UPROPERTY(meta = (BindWidget))
		UBorder* MoveBorderCamera;

	UPROPERTY(meta = (BindWidget))
		UBorder* MapBoundsBorder;

	FVector2D MapBorderSize;

	//Map image
	UPROPERTY(meta = (BindWidget))
		UImage* CamImage;

	//Overlays
	UPROPERTY(meta = (BindWidget))
		UOverlay* CamOverlay;
	UPROPERTY(meta = (BindWidget))
		UOverlay* MapPanel;

	UPROPERTY(meta = (BindWidget))
		UBorder* FossilBorder;
	UPROPERTY(meta = (BindWidget))
		UBorder* KelpBorder;
	UPROPERTY(meta = (BindWidget))
		UBorder* ShallowsBorder;

	//Buttons
	UPROPERTY(meta = (BindWidget))
		UButton* CloseButton;
	UPROPERTY(meta = (BindWidget))
		UButton* MinimizeButton;
	UPROPERTY(meta = (BindWidget))
		UButton* MarkerButton;
	UPROPERTY(meta = (BindWidget))
		UButton* CaveMarker;
	UPROPERTY(meta = (BindWidget))
		UButton* CloseDetails;
	UPROPERTY(meta = (BindWidget))
		UButton* FossilYardButton;
	UPROPERTY(meta = (BindWidget))
		UButton* KelpForestButton;
	UPROPERTY(meta = (BindWidget))
		UButton* ShallowsButton;

	//Animations
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* MarkerAnim;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* CaveAnim;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* DetailsAnim;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* MinimizeAnim;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* FossilAnim;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* KelpAnim;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* ShallowsAnim;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* ShowFossilOverlay;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* ShowKelpOverlay;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* ShowShallowsOverlay;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* ZoomOutAnim;

	//References
	APCTerminal* PC;
	ABoat* Boat;

	double PreviousMouseX;
	double PreviousMouseY;

	//Locks zooming if min/max values are reached
	bool bLockZoomIn;
	bool bLockZoomOut;

	//Bools for map and widget movement
	bool bMoveWidget = false;
	bool PanningMap = false;
	bool bMapScrolled = false;
	bool bAreaClicked = false;

	TArray<std::reference_wrapper<bool>> Bools{ bMoveWidget, PanningMap };

protected:
	FWidgetAnimationDynamicEvent FossilFinished;
	FWidgetAnimationDynamicEvent KelpFinished;
	FWidgetAnimationDynamicEvent ShallowsFinished;
};

/*
-----------------------------
Date		Feb 22 2024
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Added a CallObjectiveID in the ZoomMap Method using "MapZoomID". For a tutorial mission
-----------------------------
*/