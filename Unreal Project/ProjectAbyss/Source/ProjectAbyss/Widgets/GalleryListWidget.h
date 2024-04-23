// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GalleryListWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UGalleryListWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public: 

	//Native construct
	virtual void NativeConstruct() override;

	void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	void OnExpandPhotoButtonClicked();

	//TSubclass of individual photo widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> IndividualPhotoWidgetClass;

	//Stored variable of the individual photo widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		class UIndividualPhotoWidget* StoredIndividualPhotoWidget;

	UMaterialInstanceDynamic* PhotoGalleryMaterialTest;

	//On button clicked
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ExpandPhotoButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* Photo;

	class UDataObject* DataObject;

};
