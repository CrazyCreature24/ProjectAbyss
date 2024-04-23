// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Image.h"
#include "Actors/MissionSystem/MissionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Creature.h"

#include <EquipmentBase.h>

#include "DataObject.generated.h"

/**
 * 
 */

class UComboBoxKey;
class UDataObject;


USTRUCT(BlueprintType)
struct FMissionDataObjectStruct //Nico: I'm guessing this is now just a mission data object struct
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMissionType MissionType; // 1 = main, 2 = side

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MissionNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Details;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName MissionID;
	
	FMissionDataObjectStruct()
	{
		Name = FText::FromString("it is broken");
		MissionType = EMissionType::Main;
		Details = FText::FromString(TEXT("Default Details"));
		MissionID = "ERROR";
		MissionActor = nullptr;
	}

	void StructInit(FName NewMissionID, FMissionDetails NewMissionDetails)
	{
		MissionID = NewMissionID;
		MissionDetails = NewMissionDetails;

		Name = MissionDetails.MissionName;
		MissionType = MissionDetails.Type;
		Details = MissionDetails.LogDescription;
		MissionActor = nullptr;
	}


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMissionDetails MissionDetails;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AMissionBase* MissionActor;
	
};

//Enum for slot type
UENUM(BlueprintType)
enum class ESlotType : uint8
{
	Storage UMETA(DisplayName = "Storage"),
	Hotbar UMETA(DisplayName = "Hotbar")
};

USTRUCT(BlueprintType)
struct FEquipmentStruct
{
	GENERATED_BODY()

public: 

	AEquipmentBase* Equip;
	int SlotNum;
	ESlotType SlotType;
};

USTRUCT(BlueprintType)
struct FShopStruct
{
	GENERATED_BODY()

public:

	UTexture2D* ShopIcon;
	UTexture2D* ShopDetailsIcon;
	FText name;
	int Price;
	FText ItemDetails;
	AEquipmentBase* ShopEquip;
};


USTRUCT(BlueprintType)
struct FJournalDataStruct
{
	GENERATED_BODY()

public:
	UTexture2D* JournalIcon;
	FText JournalName;
	bool bIsCaptured;
	int JournalID;
};

USTRUCT(BlueprintType)
struct FGalleryDataStruct
{
	GENERATED_BODY()
public: 
	// Base material for instances to be created from. Has the photorender texture parameter.
	UMaterial* PhotoGalleryMaterial;
	// The texture to be used for the photo gallery
	UTextureRenderTarget2D* PhotoGalleryTexture;
	// Name for captured fish to be displayed in the gallery
	FText FishName;
};

USTRUCT(BlueprintType)
struct FGalleryDataStructTexture2D
{
	GENERATED_BODY()
public: 
	// Base material for instances to be created from. Has the photorender texture parameter.
	UMaterial* PhotoGalleryMaterial;
	// The texture to be used for the photo gallery
	UTexture2D* PhotoGalleryTexture;

};

USTRUCT(BlueprintType)
struct FHotbarDataStruct
{
	GENERATED_BODY()
public:
	UTexture2D* Image;
	int SlotNum;
	bool bIsSelected;
};

UCLASS(Blueprintable)
class PROJECTABYSS_API UDataObject : public UObject
{
	GENERATED_BODY()

public:

	UDataObject();

	//Structs holding data for objects
	FMissionDataObjectStruct MissionDataStruct;
	FEquipmentStruct EquipStruct;
	FShopStruct ShopStruct;
	FJournalDataStruct JournalDataStruct;
	FGalleryDataStruct GalleryDataStruct;
	FHotbarDataStruct HotbarDataStruct;
	FGalleryDataStructTexture2D GalleryDataStructTexture2D;

	//Used to initialize the different structs
	void InitMission(FText text, EMissionType missiontype, int missionnum, FText details, FName MissionID);
	void InitMission(FName NewMissionID, FMissionDetails NewMissionDetails);
	void Init(AEquipmentBase* equipment, int slot, ESlotType type);
	void Init(UTexture2D* image, UTexture2D* shopdetailsicon, FText name, int price, FText itemdetails, AEquipmentBase* shopequip);
	void Init(UTexture2D* icon, FText name, bool bCaptured, int JournalID);
	void Init(UMaterial* material, UTextureRenderTarget2D* texture/*, FText name*/);
	void Init(UMaterial* material, UTexture2D* texture/*, FText name*/);
	void Init(UTexture2D* image, int slotNum, bool isselected = false);
};



/*
ChangeLog:
-----------------------------
Date:	 	Oct 17 2023
Name:	Tyler Brost
Summary:	Added basic elements
-----------------------------
Date:	 	Nov 28 2023
Name:	Renz Leo Nicolas Dela Cruz
Summary:	- Updated FMissionDataObjectStruct
-----------------------------
*/
