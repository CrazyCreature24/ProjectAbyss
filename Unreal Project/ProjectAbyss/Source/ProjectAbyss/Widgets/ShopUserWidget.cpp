// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/ShopUserWidget.h"
#include "Components/ListView.h"
#include "DataObject.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/ScaleBox.h"
#include "Animation/WidgetAnimation.h"
#include "PCUserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Actors/PhotographyCamera.h"
#include "Actors/Equipment/EquipmentBase.h"
#include "Actors/StorageChest.h"
#include "SoundDecoy.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameMode.h"
#include "Equipment/DeployableLight.h"
#include "Equipment/Seascooter.h"
#include "Equipment/FishFinder.h"

//Nico mods
#include "Kismet/GameplayStatics.h"
#include "Subsystems/MissionSystem/MissionLogSubsystem.h"

void UShopUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>()->CallObjectiveID("ShopID"); //Nico Mod

    PC = GetPC();

    TArray<AActor*> temparray;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStorageChest::StaticClass(), temparray);

    StorageChest = Cast<AStorageChest>(temparray[0]);

    //Button bindings
    CloseButton->OnClicked.AddUniqueDynamic(this, &UShopUserWidget::CloseButtonClicked);
    MinimizeButton->OnClicked.AddUniqueDynamic(this, &UShopUserWidget::MinimizeButtonClicked);
    BuyButton->OnClicked.AddUniqueDynamic(this, &UShopUserWidget::BuyButtonClicked);

    //Border binding for moving widget
    MoveBorder->OnMouseButtonDownEvent.BindUFunction(this, "ClickBorderTest");

    //Get and store gamemode in temp variable
    AMainGameMode* Gaming = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    //Set player money in shop
    PlayerMoney->SetText(FText::FromString(FString::FromInt(Gaming->GetMoney())));

    //Add equipment to shop
    if (bEquipmentAdded == false)
    {
        ShopEquipment.Add(LightClass.GetDefaultObject());
        ShopEquipment.Add(SoundDecoyClass.GetDefaultObject());
        ShopEquipment.Add(SeaScooterClass.GetDefaultObject());
        ShopEquipment.Add(FishFinderClass.GetDefaultObject());
        RefreshShop();
        bEquipmentAdded = true;
    }

    //Add shop widget to widget rotation (for controller support)
    Cast<UPCUserWidget>(PC->StoredPCWidget)->AddToRotation(this, "Current");
}

void UShopUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    //Widget movement handling
    if (bBorderIsClicked == true)
    {
        UpdateBorder(this, Cast<UCanvasPanelSlot>(ContainingScaleBox->Slot), PreviousMouseX, PreviousMouseY, UWidgetLayoutLibrary::GetMousePositionOnViewport(this));
    }

    FVector2D mouse;
    //Get mouse position after scaling/moving
    mouse = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
    PreviousMouseX = mouse.X;
    PreviousMouseY = mouse.Y;
}

void UShopUserWidget::RefreshShop()
{
    //Uses the shop equipment array to populate the shop
    for (int i = 0; i < ShopEquipment.Num(); i++)
    {
        UDataObject* tempobject = NewObject<UDataObject>(UDataObject::StaticClass());

        if(ShopEquipment[i])
        tempobject->Init(temp, ShopEquipment[i]->HotbarImage, FText::FromString(ShopEquipment[i]->GetItemName()),ShopEquipment[i]->GetShopCost(), ShopEquipment[i]->GetItemDetails(), ShopEquipment[i]);

        ShopListView->AddItem(tempobject);
    }

}

void UShopUserWidget::CloseButtonClicked()
{
    //Hides shop taskbar icon
    Cast<UPCUserWidget>(PC->StoredPCWidget)->ShopHotbar->SetVisibility(ESlateVisibility::Hidden);
    //Removes shop widget from the screen
    RemoveFromParent();
    //Gets map key for shop widget
    const FString* key = Cast<UPCUserWidget>(PC->StoredPCWidget)->WidgetRotation.FindKey(PC->StoredShopWidget);
    if (key)
    {
        //Removes shop from widget rotation (for controller support)
        Cast<UPCUserWidget>(PC->StoredPCWidget)->WidgetRotation[*key] = nullptr;
    }
    //Sets widget to null for garbage collection
    PC->StoredShopWidget = nullptr;
    PC->StoredPCWidget->SetFocus();
}

void UShopUserWidget::MinimizeButtonClicked()
{
    //Play animation to hide widget
    PlayAnimation(MinimizeAnimation);
    Cast<UPCUserWidget>(PC->StoredPCWidget)->ShopIsMinimized->SetVisibility(ESlateVisibility::Visible);
}

void UShopUserWidget::BuyButtonClicked()
{
    //Get reference to gamemode
    AMainGameMode* Gamemode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	if (SelectedItem)
	{
		if (Gamemode->GetMoney() >= SelectedItem->GetShopCost())
		{
            bool bIsFull = StorageChest->Inventory->IsInventoryFull();

            // If the storage chest has less than 20 items, add to inventory
            if (bIsFull == false)
            {
                //Take money away from the player based on the item shop cost
                Gamemode->SubtractMoney(SelectedItem->GetShopCost());


                //Adds item to the storage chest
                StorageChest->Inventory->AddToInventory(Cast<AEquipmentBase>(GetWorld()->SpawnActor(SelectedItem->GetClass())));

                //Updates the player money
                PlayerMoney->SetText(FText::FromString(FString::FromInt(Gamemode->GetMoney())));

                UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>()->CallObjectiveID("BuyEquipmentID");
            }
            else
            {
				//Displays a message to the player if the storage chest is full
				UE_LOG(LogTemp, Warning, TEXT("Storage chest is full"));
			}
		}
	}
   

    SetFocus();
    SetKeyboardFocus();
}

FEventReply UShopUserWidget::ClickBorderTest(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
    //Allows widget to be moved
    bBorderIsClicked = true;

    Flicker(this);

    return FEventReply(false);
}

void UShopUserWidget::OnHotbarButtonClicked()
{
    //Displays the widget again and hides the image
    Cast<UPCUserWidget>(PC->StoredPCWidget)->ShopIsMinimized->SetVisibility(ESlateVisibility::Hidden);
    PlayAnimation(MinimizeAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse);
}

FReply UShopUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Flicker(this);

    return FReply::Unhandled();
}

FReply UShopUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    //Sets all bools related to movement/scaling to false
    UnsetBools();

    return FReply::Handled();
}

FReply UShopUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
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

FReply UShopUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
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
    //Force the widget to close when the right face gamepad button is pressed
    if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Right)
    {
        CloseButtonClicked();
    }
    return FReply::Unhandled();
}

