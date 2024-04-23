// Copyright Astralwater Interactive. All Rights Reserved.


#include "PCUserWidget.h"
#include "JournalWidget.h"
#include "CameraUserWidget.h"
#include "ShopUserWidget.h"
#include "CollectionJournalWidget.h"
#include <PhotographyCollectionSubsystem.h>
#include "PhotographyComponent.h"
#include "PhotographyCamera.h"
#include "Fish.h"
#include "MissionObjectiveIDComponent.h"
#include "MainGameMode.h"
#include "InFocusWidget.h"
#include "Subsystems/CollectionSystemBase.h"
#include "UserManualWidget.h"
#include "Components/AudioComponent.h"

void UPCUserWidget::NativeConstruct()
{
    Super::NativeConstruct();


    TArray<AActor*> temparray;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APCTerminal::StaticClass(), temparray);

    PC = Cast<APCTerminal>(temparray[0]);

    PC->ToggleNotification(false);

    PlayStartup();

    StoredPlayer = Cast<ADiverCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    //Button bindings
    //Button->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::OnButtonClicked);    
    MissionButton->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::OnMissionButtonClicked);
    CameraButton->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::OnCameraButtonClicked);
    ShopButton->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::OnShopButtonClicked);
    JournalButton->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::OnCollectionJournalButtonClicked);
    //ClosePC->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::Close);
    MissionHotbar->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::OnMissionHotbarButtonClicked);
    MapHotbar->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::OnMapHotbarButtonClicked);
    ShopHotbar->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::OnShopHotbarButtonClicked);
    JournalHotbar->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::OnJournalHotbarButtonClicked);
    Menu->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::Close);
    CloseAllButton->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::OnCloseAllButtonClicked);
    ManualButton->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::OnManualButtonClicked);
    ManualHotbar->OnClicked.AddUniqueDynamic(this, &UPCUserWidget::OnManualHotbarButtonClicked);

    //Default taskbar visibility states
    MissionIsMinimized->SetVisibility(ESlateVisibility::Hidden);
    MissionHotbar->SetVisibility(ESlateVisibility::Hidden);
    MapIsMinimized->SetVisibility(ESlateVisibility::Hidden);
    MapHotbar->SetVisibility(ESlateVisibility::Hidden);
    ShopIsMinimized->SetVisibility(ESlateVisibility::Hidden);
    ShopHotbar->SetVisibility(ESlateVisibility::Hidden);
    JournalIsMinimized->SetVisibility(ESlateVisibility::Hidden);
    JournalHotbar->SetVisibility(ESlateVisibility::Hidden);
    ManualIsMinimized->SetVisibility(ESlateVisibility::Hidden);
    ManualHotbar->SetVisibility(ESlateVisibility::Hidden);

    //Default state of button to close PC
    //ClosePC->SetVisibility(ESlateVisibility::Visible);

    //Binds Delegate to OnAnimChanged Function
    StartupFinishedDelegate.BindDynamic(this, &UPCUserWidget::OnAnimChanged);
    
    //Binds Delegate to Animation
    BindToAnimationFinished(RevealWindow, StartupFinishedDelegate);

    
    SetFocus();

    //Fills the widget rotation map with placeholders
    WidgetRotation.Add("Current", nullptr);
    WidgetRotation.Add("Previous", nullptr);
    WidgetRotation.Add("Next", nullptr);

    //Stores reference to player pawn
    Player = Cast<ADiverCharacter>(GetOwningPlayer()->GetPawn());

    //Sets date text to the current date
    DateText->SetText(FText::FromString(FDateTime::Now().ToString(TEXT("%d/%m/%Y"))));

}

void UPCUserWidget::PlayClick()
{
    //Plays a click sound effect
    PlaySound(ClickSound);
}

void UPCUserWidget::PlayStartup()
{
    //Plays the startup animation
    PlayAnimation(RevealWindow);
    PC->ComputerSounds->SetTriggerParameter(TEXT("Fans"));
}


void UPCUserWidget::Close()
{
    FInputModeGameOnly temp;

    if (PC->StoredMissionWidget != nullptr)
    {
        //Remove widget from screen and set it to null for garbage collection
        PC->StoredMissionWidget->RemoveFromParent();
        PC->StoredMissionWidget = nullptr;
    }
    if (PC->StoredCameraWidget != nullptr)
    {
        //Remove widget from screen and set it to null for garbage collection
        PC->StoredCameraWidget->RemoveFromParent();
        PC->StoredCameraWidget = nullptr;
    }
    if (PC->StoredPCWidget != nullptr)
    {
        //Remove widget from screen and set it to null for garbage collection
        PC->StoredPCWidget->RemoveFromParent();
        PC->StoredPCWidget = nullptr;
    }
    if (PC->StoredShopWidget != nullptr)
    {
        //Remove widget from screen and set it to null for garbage collection
        PC->StoredShopWidget->RemoveFromParent();
        PC->StoredShopWidget = nullptr;
    }
    
    if (PC->StoredCollectionWidget != nullptr)
    {
        //Remove widget from screen and set it to null for garbage collection
        if (StoredProfileCard)
        {
            StoredProfileCard->RemoveFromParent();
            StoredProfileCard = nullptr;
        }

        PC->StoredCollectionWidget->RemoveFromParent();
        PC->StoredCollectionWidget = nullptr;
    }
    if (PC->StoredPhotoWidgets.Num() > 0)
    {
        for (int i = 0; i < PC->StoredPhotoWidgets.Num(); i++)
        {
			PC->StoredPhotoWidgets[i]->RemoveFromParent();
            PC->StoredPhotoWidgets[i] = nullptr;
		}
		PC->StoredPhotoWidgets.Empty();
    }
    if (PC->StoredManualWidget != nullptr)
    {
        PC->StoredManualWidget->RemoveFromParent();
        PC->StoredManualWidget = nullptr;
	}

    //Flushes any pressed keys, sets the input mode to GameOnly, and stops showing the mouse cursor
    APlayerController* PController = Cast<APlayerController>(GetOwningPlayer());
    PController->SetInputMode(FInputModeGameOnly());
    PController->FlushPressedKeys();
    PController->bShowMouseCursor = false;
    
    PC->ComputerSounds->SetTriggerParameter(TEXT("Shutdown"));

}

void UPCUserWidget::OnAnimChanged()
{
    //Animation to hide the startup animation window
    PlayAnimation(HideWindow);
    PC->ComputerSounds->SetTriggerParameter(TEXT("Startup"));

}

FReply UPCUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    //Forces a MouseUpEvent if the bottom face button of a gamepad is released
    if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
    {
        FSlateApplication& SlateApp = FSlateApplication::Get();

        FPointerEvent MouseUpEvent(0, SlateApp.CursorPointerIndex, SlateApp.GetCursorPos(), SlateApp.GetLastCursorPos(), SlateApp.GetPressedMouseButtons(), EKeys::LeftMouseButton, 0, SlateApp.GetPlatformApplication()->GetModifierKeys());

        TSharedPtr<FGenericWindow> GenWindow;
        SlateApp.ProcessMouseButtonUpEvent(MouseUpEvent);
    }
    return FReply::Unhandled();
}

FReply UPCUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    //Closes PC if Escape or E are pressed
    if (InKeyEvent.GetKey() == FKey("Tab"))
    {
        Close();
    }
    if (InKeyEvent.GetKey() == FKey("Escape"))
    {
        Cast<ADiverController>(GetOwningPlayer())->StartRecentlyInMenusTimer();
        Close();
    }
    //Closes PC if right face button of a gamepad is pressed
    if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Right)
    {
        Close();
        
    }
    //Forces a MouseDownEvent if the bottom face button of a gamepad is pressed
    if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
    {
        FSlateApplication& SlateApp = FSlateApplication::Get();

        FPointerEvent MouseDownEvent(0, SlateApp.CursorPointerIndex, SlateApp.GetCursorPos(), SlateApp.GetLastCursorPos(), SlateApp.GetPressedMouseButtons(), EKeys::LeftMouseButton, 0, SlateApp.GetPlatformApplication()->GetModifierKeys() );

        TSharedPtr<FGenericWindow> GenWindow;
        SlateApp.ProcessMouseButtonDownEvent(GenWindow, MouseDownEvent);
    }
    return FReply::Unhandled();
}

FReply UPCUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    //Sets all bools for movement and scaling to false in the mission widget
    if (Cast<UMissionUserWidget>(PC->StoredMissionWidget))
    {
        for (bool& b : Cast<UMissionUserWidget>(PC->StoredMissionWidget)->Bools)
        {
            b = false;
        }
    }
    //Sets all bools for movement and scaling to false in the map widget
    if (Cast<UCameraUserWidget>(PC->StoredCameraWidget))
    {
        for (bool& b : Cast<UCameraUserWidget>(PC->StoredCameraWidget)->Bools)
        {
            b = false;
        }
    }
    //Sets all bools for movement and scaling to false in the shop widget
    if (Cast<UShopUserWidget>(PC->StoredShopWidget))
    {
        for (bool& b : Cast<UShopUserWidget>(PC->StoredShopWidget)->Bools)
        {
            b = false;
        }
    }

    return FReply::Unhandled();
}

void UPCUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    //Updates time text to the current time
    if (TimeText)
    {
        FDateTime Now = FDateTime::Now();
        int32 Hour = Now.GetHour();
        FString Minute = FString::FromInt(Now.GetMinute());

        FString AMPM = "AM";
        if (Hour >= 12)
        {
            AMPM = "PM";
            if (Hour > 12)
            {
                Hour -= 12;
            }
        }
        if (Hour == 0)
        {
            Hour = 12;
        }

        FString HourString = FString::FromInt(Hour);
        if (Hour < 10)
        {
            HourString = "0" + HourString;
        }

        if (Now.GetMinute() < 10)
        {
            Minute = "0" + Minute;
        }

        TimeText->SetText(FText::FromString(HourString + ":" + Minute + " " + AMPM));
    }
}



void UPCUserWidget::OnButtonClicked()
{
    PlayClick();

}

void UPCUserWidget::OnMenuClicked()
{
    //if (ClosePC->GetVisibility() == ESlateVisibility::Collapsed)
    //{
    //    //Show close PC button
    //    ClosePC->SetVisibility(ESlateVisibility::Visible);
    //}
    //else
    //{
    //    //Hide close PC button
    //    ClosePC->SetVisibility(ESlateVisibility::Collapsed);
    //}
}

void UPCUserWidget::OnMissionButtonClicked()
{
    PlayClick();

    if (!IsValid(PC->StoredMissionWidget))
    {
        //Create and add the mission widget to the screen
        PC->StoredMissionWidget = CreateWidget(Cast<APlayerController>(StoredPlayer->Controller), PC->MissionWidget);
        PC->StoredMissionWidget->AddToViewport();

        //Display the mission widget and set focus to it
        MissionHotbar->SetVisibility(ESlateVisibility::Visible);
        Cast<UMissionUserWidget>(PC->StoredMissionWidget)->SetFocus();
        Cast<UMissionUserWidget>(PC->StoredMissionWidget)->SetKeyboardFocus();
    }

}

void UPCUserWidget::OnCameraButtonClicked()
{
    if (!IsValid(PC->StoredCameraWidget))
    {
        //Create and add the map widget to the screen
        PC->StoredCameraWidget = CreateWidget(Cast<APlayerController>(StoredPlayer->Controller), PC->CameraWidget);
        PC->StoredCameraWidget->AddToViewport();

        //Display the map widget and set focus to it
        MapHotbar->SetVisibility(ESlateVisibility::Visible);
        Cast<UCameraUserWidget>(PC->StoredCameraWidget)->SetFocus();
        Cast<UCameraUserWidget>(PC->StoredCameraWidget)->SetKeyboardFocus();
    }
}

void UPCUserWidget::OnShopButtonClicked()
{
    if (!IsValid(PC->StoredShopWidget))
    {
        //Create and add the shop widget to the screen
        PC->StoredShopWidget = CreateWidget(Cast<APlayerController>(StoredPlayer->Controller), PC->ShopWidget);
        PC->StoredShopWidget->AddToViewport();

        //Display the shop widget and set focus to it
        ShopHotbar->SetVisibility(ESlateVisibility::Visible);
        Cast<UShopUserWidget>(PC->StoredShopWidget)->SetFocus();
        Cast<UShopUserWidget>(PC->StoredShopWidget)->SetKeyboardFocus();
    }
}

void UPCUserWidget::OnCollectionJournalButtonClicked()
{
    if (!IsValid(PC->StoredCollectionWidget))
    {
        //Create and add the shop widget to the screen
        PC->StoredCollectionWidget = CreateWidget(Cast<APlayerController>(StoredPlayer->Controller), PC->CollectionWidget);
        PC->StoredCollectionWidget->AddToViewport();
        Cast<UCollectionJournalWidget>(PC->StoredCollectionWidget)->RefreshGallery();
        Cast<UCollectionJournalWidget>(PC->StoredCollectionWidget)->RefreshTabs();

        //Display the shop widget and set focus to it
        JournalHotbar->SetVisibility(ESlateVisibility::Visible);
        Cast<UCollectionJournalWidget>(PC->StoredCollectionWidget)->SetFocus();
        Cast<UCollectionJournalWidget>(PC->StoredCollectionWidget)->SetKeyboardFocus();
    }
}

void UPCUserWidget::OnManualButtonClicked()
{
    if (!IsValid(PC->StoredManualWidget))
    {
		//Create and add the shop widget to the screen
		PC->StoredManualWidget = CreateWidget(Cast<APlayerController>(StoredPlayer->Controller), PC->ManualWidget);
		PC->StoredManualWidget->AddToViewport();

		//Display the shop widget and set focus to it
		ManualHotbar->SetVisibility(ESlateVisibility::Visible);
		Cast<UUserManualWidget>(PC->StoredManualWidget)->SetFocus();
		Cast<UUserManualWidget>(PC->StoredManualWidget)->SetKeyboardFocus();
	}
}

void UPCUserWidget::OnMissionHotbarButtonClicked()
{
    //Reveals the minimized mission widget
    Cast<UMissionUserWidget>(PC->StoredMissionWidget)->OnHotbarButtonClicked();
    Cast<UMissionUserWidget>(PC->StoredMissionWidget)->SetFocus();
}

void UPCUserWidget::OnMapHotbarButtonClicked()
{
    //Reveals the minimized map widget
    Cast<UCameraUserWidget>(PC->StoredCameraWidget)->OnHotbarButtonClicked();
    Cast<UCameraUserWidget>(PC->StoredCameraWidget)->SetFocus();
}

void UPCUserWidget::OnShopHotbarButtonClicked()
{
    //Reveals the minimized shop widget
    Cast<UShopUserWidget>(PC->StoredShopWidget)->OnHotbarButtonClicked();
    Cast<UShopUserWidget>(PC->StoredShopWidget)->SetFocus();
}

void UPCUserWidget::OnJournalHotbarButtonClicked()
{
    //Reveals the minimized journal widget
	Cast<UCollectionJournalWidget>(PC->StoredCollectionWidget)->OnHotbarButtonClicked();
	Cast<UCollectionJournalWidget>(PC->StoredCollectionWidget)->SetFocus();
}

void UPCUserWidget::OnManualHotbarButtonClicked()
{
    //Reveals the minimized manual widget
	Cast<UUserManualWidget>(PC->StoredManualWidget)->OnHotbarButtonClicked();
	Cast<UUserManualWidget>(PC->StoredManualWidget)->SetFocus();
}

void UPCUserWidget::OnCloseAllButtonClicked()
{

    if (PC->StoredMissionWidget != nullptr)
    {
        //Remove widget from screen and set it to null for garbage collection
        PC->StoredMissionWidget->RemoveFromParent();
        PC->StoredMissionWidget = nullptr;
    }
    if (PC->StoredCameraWidget != nullptr)
    {
        //Remove widget from screen and set it to null for garbage collection
        PC->StoredCameraWidget->RemoveFromParent();
        PC->StoredCameraWidget = nullptr;
    }
    if (PC->StoredShopWidget != nullptr)
    {
        //Remove widget from screen and set it to null for garbage collection
        PC->StoredShopWidget->RemoveFromParent();
        PC->StoredShopWidget = nullptr;
    }

    if (PC->StoredCollectionWidget != nullptr)
    {
        //Remove widget from screen and set it to null for garbage collection
        if (StoredProfileCard)
        {
            StoredProfileCard->RemoveFromParent();
            StoredProfileCard = nullptr;
        }

        PC->StoredCollectionWidget->RemoveFromParent();
        PC->StoredCollectionWidget = nullptr;
    }
    if (PC->StoredPhotoWidgets.Num() > 0)
    {
        for (int i = 0; i < PC->StoredPhotoWidgets.Num(); i++)
        {
            PC->StoredPhotoWidgets[i]->RemoveFromParent();
            PC->StoredPhotoWidgets[i] = nullptr;
        }
        PC->StoredPhotoWidgets.Empty();
    }
    if (PC->StoredManualWidget != nullptr)
    {
		//Remove widget from screen and set it to null for garbage collection
		PC->StoredManualWidget->RemoveFromParent();
		PC->StoredManualWidget = nullptr;
	}

    //Hide all hotbars
    MissionHotbar->SetVisibility(ESlateVisibility::Collapsed);
    MapHotbar->SetVisibility(ESlateVisibility::Collapsed);
    ShopHotbar->SetVisibility(ESlateVisibility::Collapsed);
    JournalHotbar->SetVisibility(ESlateVisibility::Collapsed);
    ManualHotbar->SetVisibility(ESlateVisibility::Collapsed);

}

void UPCUserWidget::AddToRotation(UUserWidget* widgettoadd, FString index)
{
    //Adds widget to "Current" index if it is empty
   
    if (WidgetRotation["Current"] == nullptr)
    {
        WidgetRotation[index] = widgettoadd;
        UE_LOG(LogTemp, Warning, TEXT("Curr"));

    }
    //Adds widget to "Next" index if it is empty
    else if (WidgetRotation["Next"] == nullptr)
    {
        WidgetRotation["Next"] = widgettoadd;
        UE_LOG(LogTemp, Warning, TEXT("Next"));
    }
    //Adds widget to "Previous" index if all others are full
    else
    {
        WidgetRotation["Previous"] = widgettoadd;
        UE_LOG(LogTemp, Warning, TEXT("Prev"));
    }
    
}