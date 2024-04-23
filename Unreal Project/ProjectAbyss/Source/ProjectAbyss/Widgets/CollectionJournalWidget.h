// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseUserWidget.h"
#include "CollectionJournalWidget.generated.h"

class UButton;
class UOverlay;
class UBorder;
class UWidgetSwitcher;
class UTileView;
class UTextBlock;

UCLASS()
class PROJECTABYSS_API UCollectionJournalWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	UCollectionJournalWidget();
	
	UFUNCTION()
		void MoveWidget();
	UFUNCTION()
		void Close();
	UFUNCTION()
		void OnHotbarButtonClicked();
	UFUNCTION()
		void MinimizeButtonClicked();

	UFUNCTION()
		void RefreshTabs();
	UFUNCTION()
		void RefreshGallery();
protected:


	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
		void FishTabClicked();
	UFUNCTION()
		void PlantTabClicked();
	UFUNCTION()
		void SquidTabClicked();
	UFUNCTION()
		void CrabTabClicked();
	UFUNCTION()
		void GalleryTabClicked();
	UFUNCTION()
		void UploadButtonClicked();
	UFUNCTION()
		void GalleryNextButtonClicked();
	UFUNCTION()
		void GalleryPrevButtonClicked();

	UFUNCTION()
		void SetFiles();
	UFUNCTION()
		void UpdateGalleryPageText();

public:

	bool bMoveWidget = false;
	UPROPERTY(meta = (BindWidget))
		UOverlay* MovableOverlay;

	TArray<std::reference_wrapper<bool>> Bools{ bMoveWidget };

	UPROPERTY()
		FString JournalPhotosFilePath;
protected:
	
	double PreviousMouseX;
	double PreviousMouseY;

	UPROPERTY(meta = (BindWidget))
		UButton* CloseButton;
	UPROPERTY(meta = (BindWidget))
		UButton* MinimizeButton;
	//Animation for minimizing widget
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* MinimizeAnimation;

	UPROPERTY(meta = (BindWidget))
		UBorder* MoveBorder;


	//TABS
	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* JournalTabs;

	UPROPERTY(meta = (BindWidget))
		UBorder* FishTab;
	UPROPERTY(meta = (BindWidget))
		UBorder* PlantTab;
	UPROPERTY(meta = (BindWidget))
		UBorder* SquidTab;
	UPROPERTY(meta = (BindWidget))
		UBorder* CrabTab;
	UPROPERTY(meta = (BindWidget))
		UBorder* GalleryTab;

	//BUTTONS
	UPROPERTY(meta = (BindWidget))
		UButton* FishButton;
	UPROPERTY(meta = (BindWidget))
		UButton* PlantButton;
	UPROPERTY(meta = (BindWidget))
		UButton* SquidButton;
	UPROPERTY(meta = (BindWidget))
		UButton* CrabButton;
	UPROPERTY(meta = (BindWidget))
		UButton* GalleryButton;

	UPROPERTY(meta = (BindWidget))
		UButton* GalleryNext;	
	UPROPERTY(meta = (BindWidget))
		UButton* GalleryPrev;

	UPROPERTY(meta = (BindWidget))
		UButton* UploadButton;

	//TILE VIEWS
	UPROPERTY(meta = (BindWidget))
		UTileView* FishTileView;
	UPROPERTY(meta = (BindWidget))
		UTileView* PlantTileView;
	UPROPERTY(meta = (BindWidget))
		UTileView* SquidTileView;
	UPROPERTY(meta = (BindWidget))
		UTileView* CrabTileView;
	UPROPERTY(meta = (BindWidget))
		UTileView* GalleryTileView;

	int MoneyEarnedFromLastUpload = 0;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* MoneyEarnedText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* GalleryPageText;


	TArray<class UTextureRenderTarget2D*> PhotoGallery;// No longer in use


	// Array of images used to simplify widget code once bindigns are finished.
	TArray<class UImage*> PhotoGalleryImages;

	// Base material for instances to be created from. Has the photorender texture parameter.
	UMaterial* PhotoGalleryMaterialBase;


		APCTerminal* PC;


	// Current gallery of textures, set by the photography component
	UPROPERTY()
	TArray<class UTexture2D*> PhotoGalleryT2D;//

	UPROPERTY()
	int GalleryIndex = 1; 

	UPROPERTY()
	int MaxGalleryPages = 1; 

	UPROPERTY()
	int MaxPhotosInGallery = 4; 



	UPROPERTY()
	TArray<FString> PhotoFiles;
};

/*
-----------------------------
Date		Feb 22 2024
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Added a CallObjectiveID in the NativeConstruct Method using "JournalID". For a tutorial mission
-----------------------------
*/