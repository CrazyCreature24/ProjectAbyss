// Copyright Astralwater Interactive. All Rights Reserved.


#include "MissionUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PCUserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/Border.h"
#include "Components/ListView.h"
#include "Components/TreeView.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ExpandableArea.h"
#include "Components/ScrollBox.h"


void UMissionUserWidget::NativeConstruct()
{
	Super::NativeConstruct();


	UMissionLogSubsystem* QuestLog = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>();

	//Store reference to the QuestLog
	quests = QuestLog;

	//Store reference to the PCTerminal
	PC = GetPC();
	
	//Refresh quests if this widget has not been added to the screen
	if (bHasBeenAdded == false)
	{
		RefreshQuests();

		bHasBeenAdded = true;
	}

	//Set visibility of different elements on widget creation
	MainMissionList->SetVisibility(ESlateVisibility::Visible);
	SideMissionList->SetVisibility(ESlateVisibility::Visible);
	CurrentMissionList->SetVisibility(ESlateVisibility::Visible); 
	CompletedMissionList->SetVisibility(ESlateVisibility::Visible);
	QuestDetailsDisplay->SetVisibility(ESlateVisibility::Collapsed);
	BackButton->SetVisibility(ESlateVisibility::Hidden);
	SendButton->SetVisibility(ESlateVisibility::Hidden);
	TrackMissionButton->SetVisibility(ESlateVisibility::Hidden);

	//Bind buttons to methods
	MinimizeButton->OnClicked.AddUniqueDynamic(this, &UMissionUserWidget::OnMinimize);
	CloseButton->OnClicked.AddUniqueDynamic(this, &UMissionUserWidget::OnCloseButtonClicked);
	MainEmailTab->OnClicked.AddUniqueDynamic(this, &UMissionUserWidget::MainTabClicked);
	CompletedEmailTab->OnClicked.AddUniqueDynamic(this, &UMissionUserWidget::CompletedTabClicked);
	CurrentMissionsEmailTab->OnClicked.AddUniqueDynamic(this, &UMissionUserWidget::CurrentMissionTabClicked);
	BackButton->OnClicked.AddUniqueDynamic(this, &UMissionUserWidget::BackButtonClicked);

	//Binds for movement and scaling of widget
	MoveBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("ClickBorderTest"));
	RightBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("RightScale"));
	LeftBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("LeftScale"));
	BottomBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("BottomScale"));
	BottomRightBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("BottomCornerScale"));
	BottomLeftBorder->OnMouseButtonDownEvent.BindUFunction(this, FName("BottomCornerScale"));
	
	//Controller support (adds to rotation of widgets on screen to cycle through)
	Cast<UPCUserWidget>(PC->StoredPCWidget)->AddToRotation(this, "Current");

	quests->CallObjectiveID("Open Missions");

}



void UMissionUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//Move widget handling
	if (bMoveWidget == true)
	{
		UpdateBorder(this, Cast<UCanvasPanelSlot>(ScaleOverlay->Slot), PreviousMouseX, PreviousMouseY, UWidgetLayoutLibrary::GetMousePositionOnViewport(this));
	}

	//Scale widget right handling
	if (bScaleRight == true)
	{
		ScaleRight(this, Cast<UCanvasPanelSlot>(ScaleOverlay->Slot), PreviousMouseX, UWidgetLayoutLibrary::GetMousePositionOnViewport(this));
	}

	//Scale widget left handling
	if (bScaleLeft == true)
	{
		ScaleLeft(this, Cast<UCanvasPanelSlot>(ScaleOverlay->Slot), PreviousMouseX, UWidgetLayoutLibrary::GetMousePositionOnViewport(this));
	}

	//Scale widget up/down handling
	if (bScaleBottom == true)
	{
		ScaleBottom(this, Cast<UCanvasPanelSlot>(ScaleOverlay->Slot), PreviousMouseY, UWidgetLayoutLibrary::GetMousePositionOnViewport(this));
	}

	//Scale widget corners handling
	if (bScaleBottomCorner == true)
	{
		ScaleCorner(this, Cast<UCanvasPanelSlot>(ScaleOverlay->Slot), PreviousMouseX, PreviousMouseY, UWidgetLayoutLibrary::GetMousePositionOnViewport(this), CornerName);
	}


	FVector2D mouse;
	//Get mouse position after scaling/moving
	mouse = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	PreviousMouseX = mouse.X;
	PreviousMouseY = mouse.Y;
}

void UMissionUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	SetFocus();
}

void UMissionUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	
}

FReply UMissionUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{


	Flicker(this);


	return FReply::Unhandled();
}

FReply UMissionUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//Stop any scaling or moving action
	UnsetBools();

	return FReply::Handled();
}

FReply UMissionUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	//Force a MouseUp Event when the bottom face gamepad button is released
	if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
	{
		FSlateApplication& SlateApp = FSlateApplication::Get();

		FPointerEvent MouseUpEvent(0, SlateApp.CursorPointerIndex, SlateApp.GetCursorPos(), SlateApp.GetLastCursorPos(), SlateApp.GetPressedMouseButtons(), EKeys::LeftMouseButton, 0, SlateApp.GetPlatformApplication()->GetModifierKeys());

		TSharedPtr<FGenericWindow> GenWindow;
		SlateApp.ProcessMouseButtonUpEvent(MouseUpEvent);
	}
	
	return FReply::Unhandled();
}

FReply UMissionUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == FKey("Tab"))
	{
		Cast<UPCUserWidget>(PC->StoredPCWidget)->Close();
	}

	//Force a MouseDown Event when the bottom face gamepad button is pressed
	if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
	{
		FSlateApplication& SlateApp = FSlateApplication::Get();

		FPointerEvent MouseDownEvent(0, SlateApp.CursorPointerIndex, SlateApp.GetCursorPos(), SlateApp.GetLastCursorPos(), SlateApp.GetPressedMouseButtons(), EKeys::LeftMouseButton, 0, SlateApp.GetPlatformApplication()->GetModifierKeys());

		TSharedPtr<FGenericWindow> GenWindow;
		SlateApp.ProcessMouseButtonDownEvent(GenWindow, MouseDownEvent);

	}
	//Calls the method to close the window when the right face gamepad button is pressed
	if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Right)
	{
		OnCloseButtonClicked();
	}
	
	return FReply::Unhandled();
}

void UMissionUserWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{

}

void UMissionUserWidget::OnCloseButtonClicked()
{

	Cast<UPCUserWidget>(PC->StoredPCWidget)->MissionHotbar->SetVisibility(ESlateVisibility::Hidden);
	MainEmailTab->SetVisibility(ESlateVisibility::Visible);
	CurrentMissionsEmailTab->SetVisibility(ESlateVisibility::Visible); //Nico MOD
	CompletedEmailTab->SetVisibility(ESlateVisibility::Visible); //Nico MOD

	//Remove widget from screen
	RemoveFromParent();

	//Store Map key of mission widget in temp variable (for controller support)
	const FString* key = Cast<UPCUserWidget>(PC->StoredPCWidget)->WidgetRotation.FindKey(PC->StoredMissionWidget);
	if (key)
	{
		//Remove mission widget from the onscreen widget rotation (used for controller to bring windows to the front)
		Cast<UPCUserWidget>(PC->StoredPCWidget)->WidgetRotation[*key] = nullptr;
	}

	//Clear reference to mission widget for garbage collection
	PC->StoredMissionWidget = nullptr;
	PC->StoredPCWidget->SetFocus();
}

void UMissionUserWidget::OnMinimize()
{
	//Hide window
	PlayAnimation(Minimize);
	Cast<UPCUserWidget>(PC->StoredPCWidget)->MissionIsMinimized->SetVisibility(ESlateVisibility::Visible);
}

void UMissionUserWidget::OnHotbarButtonClicked()
{
	//Quickly removes and readds the window to bring it to the front
	Flicker(this);

	//Reveal window
	Cast<UPCUserWidget>(PC->StoredPCWidget)->MissionIsMinimized->SetVisibility(ESlateVisibility::Hidden);
	PlayAnimation(Reveal);
}

FEventReply UMissionUserWidget::ClickBorderTest(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	

	//Allows window to be dragged
	bMoveWidget = true;

	return FEventReply(false);
}

void UMissionUserWidget::ScaleBorderHovered()
{

}

FEventReply UMissionUserWidget::RightScale(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	//Allows window to be scaled right
	bScaleRight = true;

	Cast<ADiverController>(GetOwningPlayer())->InputKey(EKeys::LeftMouseButton, EInputEvent::IE_Pressed, 0.0f, false);

	//const FEventReply test = UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent, this, EKeys::LeftMouseButton);

	return FEventReply(false);
}

FEventReply UMissionUserWidget::LeftScale(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	//Allows window to be scaled left
	bScaleLeft = true;

	Cast<ADiverController>(GetOwningPlayer())->InputKey(EKeys::LeftMouseButton, EInputEvent::IE_Pressed, 0.0f, false);

	return FEventReply(false);
}

FEventReply UMissionUserWidget::BottomScale(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	//Allows window to be scaled down
	bScaleBottom = true;

	Cast<ADiverController>(GetOwningPlayer())->InputKey(EKeys::LeftMouseButton, EInputEvent::IE_Pressed, 0.0f, false);

	return FEventReply(false);
}

FEventReply UMissionUserWidget::BottomCornerScale(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	//Allows window to be scaled from the corners
	bScaleBottomCorner = true;

	if (MyGeometry == BottomRightBorder->GetCachedGeometry())
	{

		Cast<ADiverController>(GetOwningPlayer())->InputKey(EKeys::LeftMouseButton, EInputEvent::IE_Pressed, 0.0f, false);
		CornerName = "BottomRightBorder";
	}
	if (MyGeometry == BottomLeftBorder->GetCachedGeometry())
	{

		Cast<ADiverController>(GetOwningPlayer())->InputKey(EKeys::LeftMouseButton, EInputEvent::IE_Pressed, 0.0f, false);
		CornerName = "BottomLeftBorder";
	}

	return FEventReply(false);
}

void UMissionUserWidget::TrackMission(FName MissionID)
{
	quests->TrackMission(MissionID);
}

void UMissionUserWidget::PlayerMouseReleased()
{
	
}


void UMissionUserWidget::UpdateQuests()
{

	RefreshQuests();
}

void UMissionUserWidget::RefreshQuests()
{
	//Clears the quest list
	MainMissionList->ClearListItems();
	SideMissionList->ClearListItems();
	CurrentMissionList->ClearListItems();
	CompletedMissionList->ClearListItems();

	int inboxCountNum = 0;
	int acceptedCountNum = 0;
	int completedCountNum = 0;
	for (int i = quests->AvailableMainMissions.Num() - 1; i > -1; i--) //Nico Mod
	{
		//Create a temp object for storing quest data
		UDataObject* tempobject = NewObject<UDataObject>(UDataObject::StaticClass());

		FName MissionID = quests->AvailableMainMissions[i]; //Gets QuestID

		//Create a temp variable to store details from the mission at i
		FMissionDetails mission = quests->GetMissionDetails(MissionID);

		//Initializes the object with details of the mission
		tempobject->InitMission(mission.MissionName, mission.Type, i + 1, mission.LogDescription, MissionID);

		//Adds the object with the mission details to the mission list
		MainMissionList->AddItem(tempobject);
		inboxCountNum++;
	}

	for (int i = quests->AvailableSideMissions.Num() - 1; i > -1; i--) //Nico Mod
	{
		UDataObject* tempobject = NewObject<UDataObject>(UDataObject::StaticClass());

		FName MissionID = quests->AvailableSideMissions[i]; //Gets QuestID

		FMissionDetails mission = quests->GetMissionDetails(MissionID);

		tempobject->InitMission(mission.MissionName, mission.Type, i + 1, mission.LogDescription, MissionID);

		MainMissionList->AddItem(tempobject);
		inboxCountNum++;
	}

	for (int i = quests->CurrentActiveMissions.Num() - 1; i > -1; i--) //Nico Mod
	{
		UDataObject* tempobject = NewObject<UDataObject>(UDataObject::StaticClass());

		FName MissionID = quests->CurrentActiveMissions[i]; //Gets QuestID

		FMissionDetails mission = quests->GetMissionDetails(MissionID);

		tempobject->InitMission(mission.MissionName, mission.Type, i + 1, mission.LogDescription, MissionID);

		tempobject->MissionDataStruct.MissionActor = quests->GetMissionPtr(MissionID); //might break

		CurrentMissionList->AddItem(tempobject);
		acceptedCountNum++;
	}	
	
	for (int i = quests->CompletedMissions.Num()-1; i > -1; i--) //Nico Mod
	{
		UDataObject* tempobject = NewObject<UDataObject>(UDataObject::StaticClass());

		FName MissionID = quests->CompletedMissions[i]; //Gets QuestID

		FMissionDetails mission = quests->GetMissionDetails(MissionID);

		tempobject->InitMission(mission.MissionName, mission.Type, i + 1, mission.LogDescription, MissionID);

		CompletedMissionList->AddItem(tempobject);
		completedCountNum++;
	}
	
	InboxCount->SetText(FText::FromString(FString::FromInt(inboxCountNum))); //ewww
	InboxCount->SetColorAndOpacity(FColor::Red);

	AcceptedCount->SetText(FText::FromString(FString::FromInt(acceptedCountNum))); //ewww
	AcceptedCount->SetColorAndOpacity(FColor::Red);

	CompletedCount->SetText(FText::FromString(FString::FromInt(completedCountNum))); //ewww
	CompletedCount->SetColorAndOpacity(FColor::Red);


}

void UMissionUserWidget::EmailClicked(UListUserWidget* test, UMissionLogSubsystem* quest, UDataObject* MissionObject)
{
	bIsInEmail = true;

	//Email ObjectiveID
	quest->CallObjectiveID("Email");

	//Hide the mission lists
	MainMissionList->SetVisibility(ESlateVisibility::Collapsed);
	SideMissionList->SetVisibility(ESlateVisibility::Collapsed);
	CurrentMissionList->SetVisibility(ESlateVisibility::Collapsed);//Nico Mod
	CompletedMissionList->SetVisibility(ESlateVisibility::Collapsed);//Nico Mod
	ExpandMain->SetVisibility(ESlateVisibility::Collapsed);

	//Display the details of the mission
	QuestDetailsDisplay->SetText(test->OwnedObject->MissionDataStruct.Details);
	QuestDetailsDisplay->SetVisibility(ESlateVisibility::Visible);

	//Display the buttons
	BackButton->SetVisibility(ESlateVisibility::Visible);
	SendButton->SetVisibility(ESlateVisibility::Visible);
	if(MissionObject->MissionDataStruct.MissionActor != nullptr)
		TrackMissionButton->SetVisibility(ESlateVisibility::Visible);


	CurrentSelectedMission = MissionObject;
}

void UMissionUserWidget::MainTabClicked()
{
	//Swap to main email tab
	EmailTabs->SetActiveWidgetIndex(0);
	SetFocus();
}

void UMissionUserWidget::SideTabClicked()
{
	//Swap to side email tab
	EmailTabs->SetActiveWidgetIndex(1);
	SetFocus();
}

void UMissionUserWidget::CurrentMissionTabClicked() //Nico Mod
{
	//Swap to current email tab
	EmailTabs->SetActiveWidgetIndex(2);
	SetFocus();
}

void UMissionUserWidget::CompletedTabClicked() //nico mod
{
	//Swap to completed email tab
	EmailTabs->SetActiveWidgetIndex(3);
	SetFocus();
}

void UMissionUserWidget::BackButtonClicked()
{
	//When the back button is clicked when your in an email
	bIsInEmail = false;
	//Hides the email away
	QuestDetailsDisplay->SetVisibility(ESlateVisibility::Collapsed);
	BackButton->SetVisibility(ESlateVisibility::Collapsed);
	SendButton->SetVisibility(ESlateVisibility::Collapsed);
	TrackMissionButton->SetVisibility(ESlateVisibility::Collapsed);
	

	//Make the lists visible 
	MainMissionList->SetVisibility(ESlateVisibility::Visible);
	SideMissionList->SetVisibility(ESlateVisibility::Visible);
	CurrentMissionList->SetVisibility(ESlateVisibility::Visible);
	CompletedMissionList->SetVisibility(ESlateVisibility::Visible);

	//Make the tabs visible
	MainEmailTab->SetVisibility(ESlateVisibility::Visible);
	//TrackedEmailTab->SetVisibility(ESlateVisibility::Visible);
	CurrentMissionsEmailTab->SetVisibility(ESlateVisibility::Visible); //Nico Mod
	CompletedEmailTab->SetVisibility(ESlateVisibility::Visible); //Nico Mod
	ExpandMain->SetVisibility(ESlateVisibility::Visible);

	CurrentSelectedMission = nullptr; //Nico Mod
}

//This gets called from ListUserWidget because I'm so s**t at coding -Nico
void UMissionUserWidget::SendButtonClicked(UDataObject* MissionDataObject) //Nico Mod 
{
	quests->CallObjectiveID("Email");

	FName MissionID = MissionDataObject->MissionDataStruct.MissionID;
	AMissionBase* CurrentMission = quests->GetMissionPtr(MissionID);
	AMissionBase* CurrentMissionTest = MissionDataObject->MissionDataStruct.MissionActor;

	if (CurrentMission == nullptr && !quests->CompletedMissions.Contains(MissionID))
	{
		quests->CallObjectiveID("Accept");
		quests->AddNewMission(MissionID);
		quests->CheckPrequisites();
		TrackMission(MissionID);
		RefreshQuests();
	}
	else if(CurrentMission != nullptr)
	{
		if (CurrentMission->bIsComplete)
		{
			quests->CallObjectiveID("Submit");
			quests->CompleteMission(CurrentMission);
			RefreshQuests();
		}
	}


}
