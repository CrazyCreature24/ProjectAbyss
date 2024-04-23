// Copyright Astralwater Interactive. All Rights Reserved.


#include "DataObject.h"


UDataObject::UDataObject()
{


}

void UDataObject::InitMission(FText text, EMissionType missiontype, int missionnum, FText details, FName MissionID)
{
    MissionDataStruct.Name = text;
    MissionDataStruct.MissionType = missiontype;
    MissionDataStruct.MissionNum = missionnum;
    MissionDataStruct.Details = details;
    MissionDataStruct.MissionID = MissionID;
}

void UDataObject::InitMission(FName NewMissionID, FMissionDetails NewMissionDetails)
{
    MissionDataStruct.StructInit(NewMissionID, NewMissionDetails);
}

void UDataObject::Init(AEquipmentBase* equipment, int slot, ESlotType type)
{
    EquipStruct.Equip = equipment;
    EquipStruct.SlotNum = slot;
    EquipStruct.SlotType = type;
}

void UDataObject::Init(UTexture2D* image, UTexture2D* shopdetailsicon, FText name, int price, FText itemdetails, AEquipmentBase* shopequip)
{
    ShopStruct.ShopIcon = image;
    ShopStruct.ShopDetailsIcon = shopdetailsicon;
    ShopStruct.name = name;
    ShopStruct.Price = price;
    ShopStruct.ItemDetails = itemdetails;
    ShopStruct.ShopEquip = shopequip;
}

void UDataObject::Init(UTexture2D* icon, FText name, bool bCaptured, int JournalID)
{
    JournalDataStruct.JournalIcon = icon;
	JournalDataStruct.JournalName = name;
    JournalDataStruct.bIsCaptured = bCaptured;
    JournalDataStruct.JournalID = JournalID;
}

void UDataObject::Init(UMaterial* material, UTextureRenderTarget2D* texture/*, FText name*/)
{
    GalleryDataStruct.PhotoGalleryMaterial = material;
    GalleryDataStruct.PhotoGalleryTexture = texture;
    //GalleryDataStruct.FishName = name;
}

void UDataObject::Init(UMaterial* material, UTexture2D* texture)
{
    GalleryDataStructTexture2D.PhotoGalleryMaterial = material;
    GalleryDataStructTexture2D.PhotoGalleryTexture = texture;
}

void UDataObject::Init(UTexture2D* image, int slotNum, bool isselected)
{
    HotbarDataStruct.Image = image;
    HotbarDataStruct.SlotNum = slotNum;
    HotbarDataStruct.bIsSelected = isselected;
}
