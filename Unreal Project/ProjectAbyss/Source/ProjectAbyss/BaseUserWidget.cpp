// Copyright Astralwater Interactive. All Rights Reserved.


#include "BaseUserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "CameraUserWidget.h"
#include "DiverController.h"
#include "MissionUserWidget.h"
#include "ShopUserWidget.h"
#include "PCUserWidget.h"
#include "CollectionJournalWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "IndividualPhotoWidget.h"
#include "UserManualWidget.h"


void UBaseUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UBaseUserWidget::UpdateBorder(UUserWidget* widget, UCanvasPanelSlot* slot, double prevMouseX, double prevMouseY, FVector2D currentMouseLoc, float SliderVal)
{

        FVector2D finalpos;
        FVector2D prevpos;
        FVector2D currentpos;
        FVector2D difference;

        //Stores the passed in current and previous mouse locations in temporary variables
        currentpos = FVector2D(currentMouseLoc);
        prevpos = FVector2D(prevMouseX, prevMouseY);

        //Gets the amount the cursor has moved since the last tick
        difference = currentpos - prevpos;

        //Adds the position of the slot currently being clicked with the amount the cursor has moved
        finalpos = slot->GetPosition() + difference;

        //Checks if the widget passed in is the map widget
        if (Cast<UCameraUserWidget>(widget))
        {
            UCameraUserWidget* temp = Cast<UCameraUserWidget>(widget);

            //Checks if the slot passed in is the map in the map widget
            if (slot == Cast<UCanvasPanelSlot>(temp->MapPanel->Slot))
            {
                //Stores the position in a temp variable clamped to keep it within the widget bounds
                finalpos.X = FMath::Clamp(finalpos.X, 0 - (((SliderVal - 1) * 10) * 64), 0 + (((SliderVal - 1) * 10) * 64));
                finalpos.Y = FMath::Clamp(finalpos.Y, 32 - (((SliderVal - 1) * 10) * 34), 32 + (((SliderVal - 1) * 10) * 34));

            }

        }
        //Sets the new position of the passed in slot
        slot->SetPosition(finalpos);
}

void UBaseUserWidget::ScaleRight(UUserWidget* widget, UCanvasPanelSlot* slot, double prevX, FVector2D currentMouseLoc)
{

    double difference;

    //Get the amount the cursor moved on the X axis since the last tick
    difference = currentMouseLoc.X - prevX;

    //Get size and position of the passed in slot
    FVector2D size = Cast<UCanvasPanelSlot>(slot)->GetSize();
    FVector2D position = Cast<UCanvasPanelSlot>(slot)->GetPosition();

    //Moves and scales the X a certain amount to keep it in the same place while scaling
    size.X += difference;
    position.X += (difference) / 2;

    //Set size and position using the temp variable
    Cast<UCanvasPanelSlot>(slot)->SetSize(size);
    Cast<UCanvasPanelSlot>(slot)->SetPosition(position);
}

void UBaseUserWidget::ScaleLeft(UUserWidget* widget, UCanvasPanelSlot* slot, double prevX, FVector2D currentMouseLoc)
{

    double difference;

    //Get the amount the cursor moved on the X axis since the last tick
    difference = currentMouseLoc.X - prevX;

    //Get size and position of the passed in slot
    FVector2D size = Cast<UCanvasPanelSlot>(slot)->GetSize();
    FVector2D position = Cast<UCanvasPanelSlot>(slot)->GetPosition();

    //Moves and scales the X a certain amount to keep it in the same place while scaling
    size.X -= difference;
    position.X += (difference) / 2;

    //Set size and position using the temp variable
    Cast<UCanvasPanelSlot>(slot)->SetSize(size);
    Cast<UCanvasPanelSlot>(slot)->SetPosition(position);
}

void UBaseUserWidget::ScaleBottom(UUserWidget* widget, UCanvasPanelSlot* slot, double prevY, FVector2D currentMouseLoc)
{

    double difference;

    //Get the amount the cursor moved on the Y axis since the last tick
    difference = currentMouseLoc.Y - prevY;

    //Get size and position of the passed in slot
    FVector2D size = Cast<UCanvasPanelSlot>(slot)->GetSize();
    FVector2D position = Cast<UCanvasPanelSlot>(slot)->GetPosition();

    //Moves and scales the Y a certain amount to keep it in the same place while scaling
    size.Y += difference;
    position.Y += difference / 50;

    //Set size and position using the temp variable
    Cast<UCanvasPanelSlot>(slot)->SetSize(size);
    Cast<UCanvasPanelSlot>(slot)->SetPosition(position);
}

void UBaseUserWidget::ScaleCorner(UUserWidget* widget, UCanvasPanelSlot* slot, double prevX, double prevY, FVector2D currentMouseLoc, FString cornername)
{

    if (cornername == "BottomRightBorder")
    {

        double differencex;
        double differencey;

        //Get the amount the cursor moved on the X and Y axis since the last tick
        differencex = currentMouseLoc.X - prevX;
        differencey = currentMouseLoc.Y - prevY;

        //Get size and position of the passed in slot
        FVector2D size = Cast<UCanvasPanelSlot>(slot)->GetSize();
        FVector2D position = Cast<UCanvasPanelSlot>(slot)->GetPosition();

        //Moves and scales the X a certain amount to keep it in the same place while scaling
        size.X += differencex;
        position.X += (differencex) / 2;

        //Moves and scales the Y a certain amount to keep it in the same place while scaling
        size.Y += differencey;
        position.Y += (differencey) / 50;

        //Set size and position using the temp variables
        Cast<UCanvasPanelSlot>(slot)->SetSize(size);
        Cast<UCanvasPanelSlot>(slot)->SetPosition(position);
    }

    if (cornername == "BottomLeftBorder")
    {
        
        double differencex;
        double differencey;

        //Get the amount the cursor moved on the X and Y axis since the last tick
        differencex = currentMouseLoc.X - prevX;
        differencey = currentMouseLoc.Y - prevY;

        //Get size and position of the passed in slot
        FVector2D size = Cast<UCanvasPanelSlot>(slot)->GetSize();
        FVector2D position = Cast<UCanvasPanelSlot>(slot)->GetPosition();

        //Moves and scales the X a certain amount to keep it in the same place while scaling
        size.X -= differencex;
        position.X += (differencex) / 2;

        //Moves and scales the Y a certain amount to keep it in the same place while scaling
        size.Y += differencey;
        position.Y += (differencey) / 50;

        //Set size and position using the temp variables
        Cast<UCanvasPanelSlot>(slot)->SetSize(size);
        Cast<UCanvasPanelSlot>(slot)->SetPosition(position);
    }
    
}

void UBaseUserWidget::UnsetBools()
{
    if (Cast<UMissionUserWidget>(GetPC()->StoredMissionWidget))
    {
        //Reset all bools in the mission widget
        for (bool& b : Cast<UMissionUserWidget>(GetPC()->StoredMissionWidget)->Bools)
        {
            b = false;
        }
    }
    if (Cast<UCameraUserWidget>(GetPC()->StoredCameraWidget))
    {
        //Reset all bools in the map widget
        for (bool& b : Cast<UCameraUserWidget>(GetPC()->StoredCameraWidget)->Bools)
        {
            b = false;
        }
    }
    if (Cast<UShopUserWidget>(GetPC()->StoredShopWidget))
    {
        //Reset all bools in the shop widget
        for (bool& b : Cast<UShopUserWidget>(GetPC()->StoredShopWidget)->Bools)
        {
            b = false;
        }
    }
    if (Cast<UCollectionJournalWidget>(GetPC()->StoredCollectionWidget))
    {
        //Reset all bools in the shop widget
        for (bool& b : Cast<UCollectionJournalWidget>(GetPC()->StoredCollectionWidget)->Bools)
        {
            b = false;
        }
    }
    if (Cast<UUserManualWidget>(GetPC()->StoredManualWidget))
    {
		//Reset all bools in the shop widget
        for (bool& b : Cast<UUserManualWidget>(GetPC()->StoredManualWidget)->Bools)
        {
			b = false;
		}
	}
    //Loop through TArray<UUserWidget*> StoredPhotoWidgets and reset all bools
    for (UUserWidget* widget : GetPC()->StoredPhotoWidgets)
    {
        if (Cast<UIndividualPhotoWidget>(widget))
        {
            for (bool& b : Cast<UIndividualPhotoWidget>(widget)->Bools)
            {
				b = false;
			}
		}
	}


}

void UBaseUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

APCTerminal* UBaseUserWidget::GetPC()
{
    TArray<AActor*> temparray;

    //Get PC terminal actor from world
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APCTerminal::StaticClass(), temparray);

    
    return Cast<APCTerminal>(temparray[0]);
}

FString UBaseUserWidget::GetFocusedWindow()
{
    return FocusedWindow;
}

void UBaseUserWidget::SetFocusedWindow(FString window)
{
    FocusedWindow = window;
}



void UBaseUserWidget::Flicker(UUserWidget* widget)
{
    //Remove widget from rotation while being readded
    Cast<UPCUserWidget>(GetPC()->StoredPCWidget)->WidgetRotation["Current"] = nullptr;

    //Remove widget from screen
    widget->RemoveFromParent();
    
    //Add widget back to screen
    widget->AddToViewport();

}

void UBaseUserWidget::MoveCursor(FKey key, APlayerController* player, double stickstate)
{
    if (key == FKey("Gamepad_LeftX"))
    {
        double x;
        double y;

        //Get the current player mouse location
        player->GetMousePosition(x, y);

        //Stores the position of the stick with a multiplier in a temp variable
        x += 10 * stickstate;
        
        //Sets the mouse location based on the temp variables
        player->SetMouseLocation(x, y);
    }
    
    if (key == FKey("Gamepad_LeftY"))
    {
        double x;
        double y;
        
        //Get the current player mouse location
        player->GetMousePosition(x, y);

        //Stores the position of the stick with a multiplier in a temp variable
        y -= 10 * stickstate;

        //Sets the mouse location based on the temp variables
        player->SetMouseLocation(x, y);
    }
    
}

FReply UBaseUserWidget::NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent)
{
    
    if (InAnalogEvent.GetKey() == FKey("Gamepad_LeftX"))
    {
        //Moves the cursor X based on a controller stick state
        MoveCursor(InAnalogEvent.GetKey(), GetOwningPlayer(), InAnalogEvent.GetAnalogValue());
    }
    
    if (InAnalogEvent.GetKey() == FKey("Gamepad_LeftY"))
    {
        //Moves the cursor Y based on a controller stick state
        MoveCursor(InAnalogEvent.GetKey(), GetOwningPlayer(), InAnalogEvent.GetAnalogValue());
    }

    if (InAnalogEvent.GetKey() == FKey("Gamepad_RightTriggerAxis"))
    {
        //If trigger is pressed in fully
        if (InAnalogEvent.GetAnalogValue() == 1.0f)
        {
            //If there is a widget in the next position and the trigger was not pressed down last tick
            if (Cast<UPCUserWidget>(GetPC()->StoredPCWidget)->WidgetRotation["Next"] != nullptr && bNextSwapOccured == false)
            {
                //If there is a widget in the previous position
                if (Cast<UPCUserWidget>(GetPC()->StoredPCWidget)->WidgetRotation["Previous"] != nullptr)
                {
                    //Store the widget in the next position in a temp variable
                    if (UUserWidget* next = Cast<UPCUserWidget>(GetPC()->StoredPCWidget)->WidgetRotation["Next"])
                    {
                        //Store PC widget in a temp variable
                        UPCUserWidget* temppc = Cast<UPCUserWidget>(GetPC()->StoredPCWidget);
                        UUserWidget* tempcurrent;
                        UUserWidget* tempprev;

                        //Store the current and previous widgets in temp variables
                        tempcurrent = temppc->WidgetRotation["Current"];
                        tempprev = temppc->WidgetRotation["Previous"];

                        //Flicker the next widget to bring it to the front (also sets it to current)
                        temppc->Flicker(next);

                        //Set the previous as the new next and the current as the new previous
                        temppc->WidgetRotation["Next"] = tempprev;
                        temppc->WidgetRotation["Previous"] = tempcurrent;

                    }
                    
                }
                //If there is not a widget in the previous position
                else if (Cast<UPCUserWidget>(GetPC()->StoredPCWidget)->WidgetRotation["Previous"] == nullptr)
                {
                    //Store the widget in the next position in a temp variable
                    if (UUserWidget* next2 = Cast<UPCUserWidget>(GetPC()->StoredPCWidget)->WidgetRotation["Next"])
                    {
                        //Store PC widget in a temp variable
                        UPCUserWidget* temppc = Cast<UPCUserWidget>(GetPC()->StoredPCWidget);
                        UUserWidget* tobenext;

                        //Store the current widget in a temp variable
                        tobenext = temppc->WidgetRotation["Current"];

                        //Flicker the next widget to bring it to the front (also sets it to current)
                        temppc->Flicker(next2);

                        //Set the current as the new next
                        temppc->WidgetRotation["Next"] = tobenext;
                        

                    }
                    //currently breaks a bit if you swap 2 and then close 
                    //the one behind and open it again and swap them
                }
                //Sets the focus to the PC widget
                Cast<UPCUserWidget>(GetPC()->StoredPCWidget)->SetFocus();
                
                //Prevents the swap from occuring every tick
                bNextSwapOccured = true;
            }
            
        }
        //If the trigger is released more than halfway
        if (InAnalogEvent.GetAnalogValue() < 0.5f)
        {
            //Reset the swap lock
            bNextSwapOccured = false;
        }
    }

    if (InAnalogEvent.GetKey() == FKey("Gamepad_LeftTriggerAxis"))
    {
        //If trigger is pressed in fully
        if (InAnalogEvent.GetAnalogValue() == 1.0f)
        {
            //If there is a widget in the next position and the trigger was not pressed down last tick
            if (Cast<UPCUserWidget>(GetPC()->StoredPCWidget)->WidgetRotation["Next"] != nullptr && bPreviousSwapOccured == false)
            {
                //If there is a widget in the previous position
                if (Cast<UPCUserWidget>(GetPC()->StoredPCWidget)->WidgetRotation["Previous"] != nullptr)
                {
                    //Store the widget in the previous position in a temp variable
                    if (UUserWidget* prev = Cast<UPCUserWidget>(GetPC()->StoredPCWidget)->WidgetRotation["Previous"])
                    {
                        //Store PC widget in a temp variable
                        UPCUserWidget* temppc = Cast<UPCUserWidget>(GetPC()->StoredPCWidget);
                        UUserWidget* tempcurrent;
                        UUserWidget* tempnext;


                        //Store the current and next widgets in temp variables
                        tempcurrent = temppc->WidgetRotation["Current"];
                        tempnext = temppc->WidgetRotation["Next"];

                        //Flicker the next widget to bring it to the front (also sets it to current)
                        temppc->Flicker(prev);

                        //Set the next as the new previous and the current as the new next
                        temppc->WidgetRotation["Previous"] = tempnext;
                        temppc->WidgetRotation["Next"] = tempcurrent;

                    }

                }
                //If there is not a widget in the previous position
                else if (Cast<UPCUserWidget>(GetPC()->StoredPCWidget)->WidgetRotation["Previous"] == nullptr)
                {
                    //Store the widget in the next position in a temp variable
                    if (UUserWidget* next2 = Cast<UPCUserWidget>(GetPC()->StoredPCWidget)->WidgetRotation["Next"])
                    {
                        //Store PC widget in a temp variable
                        UPCUserWidget* temppc = Cast<UPCUserWidget>(GetPC()->StoredPCWidget);
                        UUserWidget* tobenext;

                        //Store the current widget in a temp variable
                        tobenext = temppc->WidgetRotation["Current"];

                        //Flicker the next widget to bring it to the front (also sets it to current)
                        temppc->Flicker(next2);

                        //Set the current as the new next
                        temppc->WidgetRotation["Next"] = tobenext;


                    }
                    //currently breaks a bit if you swap 2 and then close 
                    //the one behind and open it again and swap them
                }
                //Sets the focus to the PC widget
                Cast<UPCUserWidget>(GetPC()->StoredPCWidget)->SetFocus();

                //Prevents the swap from occuring every tick
                bPreviousSwapOccured = true;
            }
        }
        //If the trigger is released more than halfway
        if (InAnalogEvent.GetAnalogValue() < 0.5f)
        {
            //Reset the swap lock
            bPreviousSwapOccured = false;
        }
    }
    
    return FReply::Unhandled();
}



