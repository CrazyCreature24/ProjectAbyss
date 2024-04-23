// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/HotbarWidget.h"
#include "Components/TileView.h"
#include "HotbarListWidget.h"
#include "DataObject.h"

void UHotbarWidget::NativeConstruct()
{
	/*HotbarImages.Add(Slot1);
	HotbarImages.Add(Slot2);
	HotbarImages.Add(Slot3);
	HotbarImages.Add(Slot4);

	HotbarBorders.Add(Slot1Border);
	HotbarBorders.Add(Slot2Border);
	HotbarBorders.Add(Slot3Border);
	HotbarBorders.Add(Slot4Border);*/

	for (int i = 0; i < HotbarSize; i++)
	{
		UDataObject* tempobject = NewObject<UDataObject>(UDataObject::StaticClass());

		UTexture2D* DefaultTex = DefaultTexture;

		tempobject->Init(DefaultTex, i + 1);

		HotbarTileView->AddItem(tempobject);
	}

	

	//for (int i = 1; i < HotbarImages.Num(); i++)
	//{
	//	HotbarImages[i]->SetBrushFromTexture(DefaultTexture);
	//}

	SetSelectedSlot(0);
}

void UHotbarWidget::SetImage(int32 HotbarSlot, UTexture2D* texture)
{
	if (texture == nullptr)
	{
		texture = DefaultTexture;
	}

	//if (HotbarSlot < 0 || HotbarSlot >= HotbarImages.Num())
	//{
	//	return;
	//}

	int tempnum = HotbarTileView->GetNumItems();
	TArray<UObject*> tempobjects = HotbarTileView->GetListItems();
	HotbarTileView->ClearListItems();

	for (int i = 0; i < tempnum; i++)
	{
		UDataObject* tempobject = NewObject<UDataObject>(UDataObject::StaticClass());

		if (i == HotbarSlot)
		{
			tempobject->Init(texture, i+1);
		}
		else
		{
			tempobject->Init(Cast<UDataObject>(tempobjects[i])->HotbarDataStruct.Image, i+1);
		}
		HotbarTileView->AddItem(tempobject);
	}

	//Cast<UDataObject>(HotbarTileView->GetItemAt(HotbarSlot))->HotbarDataStruct.Image = texture;
	//
	//HotbarTileView->RegenerateAllEntries();

	//HotbarImages[HotbarSlot]->SetBrushFromTexture(texture);
}

void UHotbarWidget::SetSelectedSlot(int32 HotbarSlot)
{
	
		for (int i = 0; i < HotbarTileView->GetNumItems(); i++)
		{
			if(i == HotbarSlot)
				Cast<UDataObject>(HotbarTileView->GetItemAt(i))->HotbarDataStruct.bIsSelected = true;
			else
				Cast<UDataObject>(HotbarTileView->GetItemAt(i))->HotbarDataStruct.bIsSelected = false;

		}
		if (HotbarSlot == 4)
		{
			CamSlotBorder->SetBrushColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			CamSlotBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
		}

		HotbarTileView->RegenerateAllEntries();

	
}