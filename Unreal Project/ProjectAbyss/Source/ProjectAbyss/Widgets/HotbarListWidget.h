// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "HotbarListWidget.generated.h"

class UImage;
class UTextBlock;
class UBorder;
class UDataObject;
/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UHotbarListWidget : public UUserWidget , public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:

protected:
	void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	UPROPERTY(meta = (BindWidget))
		UImage* SlotImage;

	UPROPERTY(meta = (BindWidget))
		UBorder* SlotBorder;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* SlotNum;

		UDataObject* OwnedObject;

protected:

};
