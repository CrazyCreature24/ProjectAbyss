// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/ShopListUserWidget.h"
#include "Widgets/ShopUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "PCTerminal.h"
#include "Kismet/GameplayStatics.h"

void UShopListUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    TArray<AActor*> temparray;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APCTerminal::StaticClass(), temparray);

    PC = Cast<APCTerminal>(temparray[0]);
    
    //Button binding
    ShopButton->OnClicked.AddUniqueDynamic(this, &UShopListUserWidget::ShowDetails);
}

void UShopListUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    UDataObject* Item = Cast<UDataObject>(ListItemObject);

    //Sets the object in the class as the object passed through when the item was added
    OwnedObject = Item;

    //Sets data based on information initialized in the object
    ShopEquipmentIcon->SetBrushFromTexture(Item->ShopStruct.ShopIcon);
    ShopEquipmentTitle->SetText(Item->ShopStruct.name);
    ShopEquipmentPrice = Item->ShopStruct.Price;

    //Sets the displayed shop price to the price of this object
    ShopPriceText->SetText(FText::AsNumber(ShopEquipmentPrice));
}

FReply UShopListUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    PC->StoredShopWidget->SetFocus();
    PC->StoredShopWidget->SetKeyboardFocus();
    //Force a MouseDown Event when the bottom face gamepad button is released
    if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
    {
        FSlateApplication& SlateApp = FSlateApplication::Get();

        FPointerEvent MouseUpEvent(0, SlateApp.CursorPointerIndex, SlateApp.GetCursorPos(), SlateApp.GetLastCursorPos(), SlateApp.GetPressedMouseButtons(), EKeys::LeftMouseButton, 0, SlateApp.GetPlatformApplication()->GetModifierKeys());

        TSharedPtr<FGenericWindow> GenWindow;
        SlateApp.ProcessMouseButtonUpEvent(MouseUpEvent);
    }
    return FReply::Unhandled();
}

FReply UShopListUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    SetFocus();
    SetKeyboardFocus();
    //Force a MouseDown Event when the bottom face gamepad button is pressed
    if (InKeyEvent.GetKey() == EKeys::Gamepad_FaceButton_Bottom)
    {
        FSlateApplication& SlateApp = FSlateApplication::Get();

        FPointerEvent MouseDownEvent(0, SlateApp.CursorPointerIndex, SlateApp.GetCursorPos(), SlateApp.GetLastCursorPos(), SlateApp.GetPressedMouseButtons(), EKeys::LeftMouseButton, 0, SlateApp.GetPlatformApplication()->GetModifierKeys());

        TSharedPtr<FGenericWindow> GenWindow;
        SlateApp.ProcessMouseButtonDownEvent(GenWindow, MouseDownEvent);
    }
    return FReply::Unhandled();
}

void UShopListUserWidget::ShowDetails()
{
    //Set details about the item displayed in the shop to the selected item
    Cast<UShopUserWidget>(PC->StoredShopWidget)->ItemDetails->SetText(OwnedObject->ShopStruct.ItemDetails);
    Cast<UShopUserWidget>(PC->StoredShopWidget)->ItemIcon->SetBrushFromTexture(OwnedObject->ShopStruct.ShopDetailsIcon);
    Cast<UShopUserWidget>(PC->StoredShopWidget)->SelectedItem = OwnedObject->ShopStruct.ShopEquip;
}
