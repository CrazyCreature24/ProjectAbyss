// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "DataObject.h"
#include "Animation/WidgetAnimation.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScaleBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "DiverController.h"
#include "ListUserWidget.h"
#include "Actors/PCTerminal.h"
#include "BaseUserWidget.h"

#include "MissionUserWidget.generated.h"

/**
 * 
 */

class UButton;
class UImage;
class UOverlay;
class UBorder;
class UListView;
class UTreeView;
class UWidgetSwitcher;
class UExpandableArea;
class UScrollBox;


UCLASS()
class PROJECTABYSS_API UMissionUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
		void UpdateQuests();

	UFUNCTION()
		void RefreshQuests();

	UFUNCTION()
		void EmailClicked(UListUserWidget* test, UMissionLogSubsystem* quest, UDataObject* MissionObject);

	// Buttons
	UFUNCTION()
		void MainTabClicked();

	UFUNCTION()
		void SideTabClicked();

	UFUNCTION() //Nico Mod
		void CurrentMissionTabClicked();

	UFUNCTION() //nico mod
		void CompletedTabClicked();

	UFUNCTION()
		void BackButtonClicked();

	UFUNCTION()
		void SendButtonClicked(UDataObject* MissionDataObject);

	UFUNCTION()
		void OnMinimize();

	UFUNCTION()
		void OnHotbarButtonClicked();

	UFUNCTION()
		void ScaleBorderHovered();

	//Moving and scaling widget
	UFUNCTION()
		FEventReply ClickBorderTest(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	UFUNCTION()
		FEventReply RightScale(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	UFUNCTION()
		FEventReply LeftScale(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	UFUNCTION()
		FEventReply BottomScale(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	UFUNCTION()
		FEventReply BottomCornerScale(FGeometry MyGeometry, const FPointerEvent& MouseEvent);


	UFUNCTION()//nico mod
		void TrackMission(FName MissionID);

	UFUNCTION()
		void PlayerMouseReleased();

	UFUNCTION()
		void OnCloseButtonClicked();

protected:
		
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//
	void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;


	
	
public:
	//Bools for moving/scaling widget
	bool bMoveWidget = false;
	bool bScaleRight = false;
	bool bScaleLeft = false;
	bool bScaleBottom = false;
	bool bScaleBottomCorner = false;
	TArray<std::reference_wrapper<bool>> Bools{bMoveWidget, bScaleRight, bScaleLeft, bScaleBottom, bScaleBottomCorner };

	APCTerminal* PC;

	//Mission trees
	UPROPERTY(meta = (BindWidget))
		UTreeView* MainMissionList;
	UPROPERTY(meta = (BindWidget))
		UTreeView* SideMissionList;
	UPROPERTY(meta = (BindWidget)) //Nico Mod
		UTreeView* CurrentMissionList;
	UPROPERTY(meta = (BindWidget)) //Nico Mod
		UTreeView* CompletedMissionList;
		
	//Expandable part of tab to show subtabs
	UPROPERTY(meta = (BindWidget))
		UExpandableArea* ExpandMain;

	UPROPERTY(meta = (BindWidget))
		UScrollBox* MissionTabButtons;

	//Animations to show and hide the widget
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* Reveal;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* Minimize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UListUserWidget> listwidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMissionLogSubsystem* quests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDataObject> objects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) //Nico Mod
		UDataObject* CurrentSelectedMission;

	

protected:

	//Main overlay that contains all other elements for movement
	UPROPERTY(meta = (BindWidget))
		UOverlay* MovableOverlay;

	//Borders used to move/scale the widget
	UPROPERTY(meta = (BindWidget))
		UBorder* MoveBorder;
	UPROPERTY(meta = (BindWidget))
		UBorder* RightBorder;
	UPROPERTY(meta = (BindWidget))
		UBorder* LeftBorder;
	UPROPERTY(meta = (BindWidget))
		UBorder* BottomBorder;
	UPROPERTY(meta = (BindWidget))
		UBorder* BottomRightBorder;
	UPROPERTY(meta = (BindWidget))
		UBorder* BottomLeftBorder;

	//Buttons within the widget
	UPROPERTY(meta = (BindWidget))
		UButton* CloseButton;
	UPROPERTY(meta = (BindWidget))
		UButton* MinimizeButton;
	UPROPERTY(meta = (BindWidget))
		UButton* MainEmailTab;
	UPROPERTY(meta = (BindWidget))
		UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
		UButton* SendButton;


	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* EmailTabs;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* QuestDetailsDisplay;

	UPROPERTY(meta = (BindWidget))
		UScaleBox* ScaleOverlay;


	//Nico Mods
	UPROPERTY(meta = (BindWidget))
		UButton* CurrentMissionsEmailTab;
	UPROPERTY(meta = (BindWidget))
		UButton* CompletedEmailTab;
	UPROPERTY(meta = (BindWidget))
		UButton* TrackMissionButton;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* InboxCount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* AcceptedCount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* CompletedCount;

	
	bool bIsInEmail = false;

	//Used for differentiating between corners when scaling bottom left/right
	FString CornerName;

	//Holds the previous X/Y value of the mouse for movement and scaling
	double PreviousMouseX;
	double PreviousMouseY;

	bool bHasBeenAdded = false;

};

/*
ChangeLog:
-----------------------------
Date:	 	Oct 14 2023
Name:	Tyler Brost
Summary:	Made mission tab able to be moved
-----------------------------
Date:	 	Oct 14 2023
Name:	Tyler Brost
Summary:	Made mission tab able to be closed
-----------------------------
Date:	 	Oct 16 2023
Name:	Tyler Brost
Summary:	Made mission tab able to be minimized
-----------------------------
Date:	 	Oct 17 2023
Name:	Tyler Brost
Summary:	Started to add functionality with ListView
-----------------------------
Date:	 	Nov 22 2023
Name:	Tyler Brost
Summary:	- started adding more missiong implementation with accepting a mission
-----------------------------
Date		Feb 22 2024
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Added a CallObjectiveID in the NativeConstruct method using "Open Missions". For a tutorial mission
			- Added a comment to SendButtionClicked method
-----------------------------
Date		Feb 22 2024
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Changed the ObjectiveID in Interacted from "TestBoat" to "DriveBoatID"
-----------------------------
*/
