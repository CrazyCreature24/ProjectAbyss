// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/HotbarListWidget.h"
#include "DataObject.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UHotbarListWidget::NativeConstruct()
{


}

void UHotbarListWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    UDataObject* Item = Cast<UDataObject>(ListItemObject);

    OwnedObject = Item;
    
    FString num;
    num.AppendInt(Item->HotbarDataStruct.SlotNum);
    SlotImage->SetBrushFromTexture(Item->HotbarDataStruct.Image);
    SlotNum->SetText(FText::FromString(num));

    if (Item->HotbarDataStruct.bIsSelected == true)
        SlotBorder->SetBrushColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
    else
        SlotBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));

}
