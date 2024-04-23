// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JournalWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTABYSS_API UJournalWidget : public UUserWidget
{
	GENERATED_BODY()

	void NativeConstruct() override;
	
	/// <summary>
	/// Updates the photo gallery images to the current state of the photo gallery.
	/// </summary>
	void UpdatePhotoGallery();

	/// <summary>
	/// For loop to update an array of photo gallery images.
	/// </summary>
	void UpdateImages();

public:
	// Current gallery of textures, set by the photography component
	TArray<class UTextureRenderTarget2D*> PhotoGallery; //UTextureRenderTarget2D

	// Array of images used to simplify widget code once bindigns are finished.
	TArray<class UImage*> PhotoGalleryImages;

	UPROPERTY(meta = (BindWidget));
	UImage* Image1;
	UPROPERTY(meta = (BindWidget));
	UImage* Image2;
	UPROPERTY(meta = (BindWidget));
	UImage* Image3;
	UPROPERTY(meta = (BindWidget));
	UImage* Image4;
	UPROPERTY(meta = (BindWidget));
	UImage* Image5;
	UPROPERTY(meta = (BindWidget));
	UImage* Image6;
	UPROPERTY(meta = (BindWidget));
	UImage* Image7;
	UPROPERTY(meta = (BindWidget));
	UImage* Image8;
	UPROPERTY(meta = (BindWidget));
	UImage* Image9;
	UPROPERTY(meta = (BindWidget));
	UImage* Image10;
	UPROPERTY(meta = (BindWidget));
	UImage* Image11;
	UPROPERTY(meta = (BindWidget));
	UImage* Image12;
	UPROPERTY(meta = (BindWidget));
	UImage* Image13;
	UPROPERTY(meta = (BindWidget));
	UImage* Image14;
	UPROPERTY(meta = (BindWidget));
	UImage* Image15;
	UPROPERTY(meta = (BindWidget));
	UImage* Image16;
	UPROPERTY(meta = (BindWidget));
	UImage* Image17;
	UPROPERTY(meta = (BindWidget));
	UImage* Image18;
	UPROPERTY(meta = (BindWidget));
	UImage* Image19;
	UPROPERTY(meta = (BindWidget));
	UImage* Image20;

	// Material used to display the photo gallery
	class UMaterialInstanceDynamic* PhotoGalleryMaterial;

	// Base material for instances to be created from. Has the photorender texture parameter.
	UMaterial* PhotoGalleryMaterialBase;
};

/*
ChangeLog:
-----------------------------
Date		Oct 29 2023
Name:		Austin Morris
Summary:	Added base journal functionality for testing with bound widgets.
-----------------------------
*/
