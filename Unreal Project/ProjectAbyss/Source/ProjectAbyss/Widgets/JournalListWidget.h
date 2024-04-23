// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "JournalListWidget.generated.h"

class UImage;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UJournalListWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Icon;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* Button;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* FishName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* FishID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DefaultIcon;

	//Profile card widget class
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ProfileCardWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> NotDiscoveredTextWidgetClass;

	class UDataObject* DataObject;

	class APCTerminal* WorldPC;

	//Native construct
	virtual void NativeConstruct() override;
	
	void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	//On button clicked
	UFUNCTION()
	void OnButtonClicked();
};
