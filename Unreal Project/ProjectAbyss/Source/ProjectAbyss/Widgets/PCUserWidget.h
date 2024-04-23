// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/Border.h"
#include "Containers/Map.h"
#include "Components/VerticalBox.h"
#include "Components/ProgressBar.h"
#include "Styling/SlateTypes.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/WidgetAnimationEvents.h"
#include "Delegates/Delegate.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "MissionUserWidget.h"
#include "JournalWidget.h"
#include "Layout/WidgetPath.h"
#include "PCUserWidget.generated.h"


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetAnimationFinished, UWidgetAnimation*, RevealWindow);

class UListUserWidget;
class UCameraUserWidget;
class UShopUserWidget;
class ADiverCharacter;

USTRUCT(BlueprintType)
struct FUIButtons
{
	GENERATED_BODY()

public:



};



UCLASS()
class PROJECTABYSS_API UPCUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:

	//Close widget
	UFUNCTION()
		void Close();

	//Bound to the animation state for the startup animation
	UFUNCTION()
		void OnAnimChanged();


	FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//Adds widget to rotation of widgets to cycle through (for controller support)
	void AddToRotation(UUserWidget* widgettoadd, FString index);

protected:


	void NativeConstruct() override;

	//Methods for buttons on the main PC screen
	UFUNCTION()
		void OnButtonClicked();
	UFUNCTION()
		void OnMenuClicked();
	UFUNCTION()
		void OnMissionButtonClicked();
	UFUNCTION()
		void OnCameraButtonClicked();
	UFUNCTION()
		void OnShopButtonClicked();
	UFUNCTION()
		void OnCollectionJournalButtonClicked();
	UFUNCTION()
		void OnManualButtonClicked();

	UFUNCTION()
		void OnMissionHotbarButtonClicked();
	UFUNCTION()
		void OnMapHotbarButtonClicked();
	UFUNCTION()
		void OnShopHotbarButtonClicked();
	UFUNCTION()
		void OnJournalHotbarButtonClicked();
	UFUNCTION()
		void OnManualHotbarButtonClicked();

	UFUNCTION()
		void OnCloseAllButtonClicked();

	//Plays click sound effect
	void PlayClick();

	//Plays the startup animation
	void PlayStartup();

public:

	//Images that show below taskbar buttons when a widget is minimized
	UPROPERTY(meta = (BindWidget))
		UImage* MissionIsMinimized;
	UPROPERTY(meta = (BindWidget))
		UImage* MapIsMinimized;
	UPROPERTY(meta = (BindWidget))
		UImage* ShopIsMinimized;
	UPROPERTY(meta = (BindWidget))
		UImage* JournalIsMinimized;
	UPROPERTY(meta = (BindWidget))
		UImage* ManualIsMinimized;

	//Buttons on the taskbar
	UPROPERTY(meta = (BindWidget))
		UButton* MissionHotbar;
	UPROPERTY(meta = (BindWidget))
		UButton* MapHotbar;
	UPROPERTY(meta = (BindWidget))
		UButton* ShopHotbar;
	UPROPERTY(meta = (BindWidget))
		UButton* JournalHotbar;
	UPROPERTY(meta = (BindWidget))
		UButton* ManualHotbar;

	//TASK BAR DETAILS
	UPROPERTY(meta = (BindWidget))
		UTextBlock* DateText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* TimeText;

	UPROPERTY(meta = (BindWidget))
		UButton* CloseAllButton;

	//References
	UUserWidget* Journal;
	APCTerminal* PC;
	ADiverCharacter* StoredPlayer;
	ADiverCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* StoredProfileCard;

	//Delegate for startup animation
	FWidgetAnimationDynamicEvent StartupFinishedDelegate;

	//Storage for rotation of widgets on screen (for controller support)
	TMap<FString, UUserWidget*> WidgetRotation;

	int MoneyEarnedFromLastUpload = 0;

protected:


	UPROPERTY(EditAnywhere)
		TSubclassOf<UMissionUserWidget> MissionWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraUserWidget> CameraWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UJournalWidget> JournalWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UListUserWidget> ListWidget;

	int NumButtons = 3;

	//Buttons
	/*UPROPERTY(meta = (BindWidget))
		UButton* Button;*/
	UPROPERTY(meta = (BindWidget))
		UButton* MissionButton;
	UPROPERTY(meta = (BindWidget))
		UButton* CameraButton;
	UPROPERTY(meta = (BindWidget))
		UButton* ShopButton;
	UPROPERTY(meta = (BindWidget))
		UButton* JournalButton;
	UPROPERTY(meta = (BindWidget))
		UButton* ManualButton;

	//UPROPERTY(meta = (BindWidget))
		//UButton* ClosePC;
	UPROPERTY(meta = (BindWidget))
		UButton* Menu;

	
	//Border containing all other elements
	UPROPERTY(meta = (BindWidget))
		UBorder* RegionBorder;

	//Startup animation elements
	UPROPERTY(meta = (BindWidget))
		UProgressBar* MenuProgress;
	UPROPERTY(meta = (BindWidget))
		UOverlay* StartupOverlay;

	//Startup animations
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* RevealWindow;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* HideWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundBase* ClickSound;

};

/*
ChangeLog:
-----------------------------
Date:	 	Oct 13 2023
Name:	Tyler Brost
Summary:	Added basic functionality
-----------------------------
Date:	 	Oct 14 2023
Name:	Tyler Brost
Summary:	Added startup animation
-----------------------------
Date:	 	Oct 14 2023
Name:	Tyler Brost
Summary:	Set up mission widget and connected PCUserWidget with it
-----------------------------
Date:	 	April 7th
Name:		Alexander McTernan
Summary:	Removed the Close button and made the Menu button use its functionality.
-----------------------------
*/
