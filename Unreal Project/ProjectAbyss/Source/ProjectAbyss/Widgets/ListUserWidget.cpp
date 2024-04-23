// Copyright Astralwater Interactive. All Rights Reserved.


#include "ListUserWidget.h"
#include "MissionUserWidget.h"
#include "Components/ListView.h"
#include "Components/ComboBoxKey.h"
#include "Components/TreeView.h"
#include "Components/Button.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Styling/SlateColor.h"


void UListUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    
    QuestLog = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>();

    //DropButton->OnClicked.AddUniqueDynamic(mission.GetDefaultObject(), &UMissionUserWidget::EmailClicked);


    DropButton->OnClicked.AddUniqueDynamic(this, &UListUserWidget::SubmitClicked);
    AcceptButton->OnClicked.AddUniqueDynamic(this, &UListUserWidget::SubmitClicked);
    TrackButton->OnClicked.AddUniqueDynamic(this, &UListUserWidget::TrackClicked);

    QuestLog->MissionWasCompleted.AddDynamic(this, &UListUserWidget::MissionUpdated);

    QuestLog->MissionTrackingChange.AddDynamic(this, &UListUserWidget::TrackTextCheck);

    TArray<AActor*> temparray;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APCTerminal::StaticClass(), temparray);
    WorldPC = Cast<APCTerminal>(temparray[0]);


}

void UListUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    UDataObject* Item = Cast<UDataObject>(ListItemObject);

    OwnedObject = Item;

    FName MissionID = Item->MissionDataStruct.MissionID;

    MissionDescription->SetText(Item->MissionDataStruct.Details);

    //Fixes the issues where the first mission in the inbox would sometimes have the accept button missing
    AcceptButton->SetVisibility(ESlateVisibility::Visible); 
    DropButton->SetVisibility(ESlateVisibility::Visible);


    ExpandMission->bIsExpanded = false;
    ExpandMission->BorderBrush.DrawAs = ESlateBrushDrawType::Box;

    //Border Color
    if (QuestLog->AvailableMainMissions.Contains(MissionID) || QuestLog->AvailableSideMissions.Contains(MissionID))
    {
        FSlateColor SlateColor(FColor::Transparent);

        //ExpandMission->DrawAS
        //ExpandMission->BorderBrush.TintColor = SlateColor;
        //ExpandMission->BorderBrush.
        //ExpandMission->BorderColor = SlateColor;
        //FSlateColor::Redz;
        TrackButton->SetVisibility(ESlateVisibility::Collapsed);

    }
    else if (QuestLog->CurrentActiveMissions.Contains(MissionID))
    {
        if (Item->MissionDataStruct.MissionActor != nullptr)
        {
            if (Item->MissionDataStruct.MissionActor->bIsComplete)
            {
                FSlateColor SlateColor(FColor::Green);
                ExpandMission->BorderBrush.TintColor = SlateColor;
            }
            else
            {
                FSlateColor SlateColor(FColor::Yellow);
                ExpandMission->BorderBrush.TintColor = SlateColor;
            }
            TrackButton->SetVisibility(ESlateVisibility::Visible);
            //AcceptButton->SetVisibility(ESlateVisibility::Visible);

            //this f***ing sucks and feels inefficent but text would not appear properly if I did not do this
            if (QuestLog->CurrentTrackedMission == OwnedObject->MissionDataStruct.MissionID) 
            {
                TrackText->SetText(FText::FromString("Untrack"));
            }
            else
            {
                TrackText->SetText(FText::FromString("Track"));

            }

        }
      
    }
    else
    {
        TrackButton->SetVisibility(ESlateVisibility::Collapsed);
        AcceptButton->SetVisibility(ESlateVisibility::Collapsed);
        DropButton->SetVisibility(ESlateVisibility::Collapsed);
    }

    TextName->SetText(Item->MissionDataStruct.Name);
    TextName->SetColorAndOpacity(FLinearColor::White);
    
    FText temp;
    if (Item->MissionDataStruct.MissionActor == nullptr)
    {
        SubmitText->SetText(temp.FromString("Accept"));
    }
    else
    {
        SubmitText->SetText(temp.FromString("Submit"));
    }


}


//WIP
void UListUserWidget::Dropdown()
{
    


}

void UListUserWidget::SubmitClicked()
{
   // Cast<UMissionUserWidget>(tempPC->StoredMissionWidget)->EmailClicked(this, QuestLog, OwnedObject);

    Cast<UMissionUserWidget>(WorldPC->StoredMissionWidget)->SendButtonClicked(Cast<UDataObject>(OwnedObject));
}

void UListUserWidget::MissionUpdated(FName MissionID)
{
    if (OwnedObject->MissionDataStruct.MissionID == MissionID)
    {
        NativeOnListItemObjectSet(Cast<UObject>(OwnedObject));
    }
}

void UListUserWidget::TrackClicked()
{
    Cast<UMissionUserWidget>(WorldPC->StoredMissionWidget)->TrackMission(OwnedObject->MissionDataStruct.MissionID);

}

void UListUserWidget::TrackTextCheck(FName MissionID)
{
    if (MissionID == OwnedObject->MissionDataStruct.MissionID)
    {
        TrackText->SetText(FText::FromString("Untrack"));
    }
    else
    {
        TrackText->SetText(FText::FromString("Track"));
    }
}

FReply UListUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    SetFocus();
    SetKeyboardFocus();
    if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
    {
        FSlateApplication& SlateApp = FSlateApplication::Get();

        FPointerEvent MouseDownEvent(0, SlateApp.CursorPointerIndex, SlateApp.GetCursorPos(), SlateApp.GetLastCursorPos(), SlateApp.GetPressedMouseButtons(), EKeys::LeftMouseButton, 0, SlateApp.GetPlatformApplication()->GetModifierKeys());

        //send the mouse event to the slate handler
        TSharedPtr<FGenericWindow> GenWindow;
        SlateApp.ProcessMouseButtonDownEvent(GenWindow, MouseDownEvent);


    }

    return FReply::Handled();
}

FReply UListUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    WorldPC->StoredMissionWidget->SetFocus();
    WorldPC->StoredMissionWidget->SetKeyboardFocus();
    if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
    {

        FSlateApplication& SlateApp = FSlateApplication::Get();

        FPointerEvent MouseUpEvent(0, SlateApp.CursorPointerIndex, SlateApp.GetCursorPos(), SlateApp.GetLastCursorPos(), SlateApp.GetPressedMouseButtons(), EKeys::LeftMouseButton, 0, SlateApp.GetPlatformApplication()->GetModifierKeys());

        TSharedPtr<FGenericWindow> GenWindow;
        SlateApp.ProcessMouseButtonUpEvent(MouseUpEvent);
        
    }
    return FReply::Handled();
}

void UListUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

}

void UListUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{

}

