// Copyright Astralwater Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DataObject.h"
#include "Subsystems/MissionSystem/MissionLogSubsystem.h"
#include "Actors/MissionSystem/MissionBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ExpandableArea.h"
#include "ListUserWidget.generated.h"


/**
 * 
 */
class UMissionUserWidget;
class UListView;
class UTreeView;
class UComboBoxKey;
class UHorizontalBox;
class APCTerminal;
class UButton;


UCLASS(Abstract)
class PROJECTABYSS_API UListUserWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:

	void NativeConstruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	class APCTerminal* WorldPC;

public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextName;

	UPROPERTY(meta = (BindWidget))
		 UButton* DropButton; //Quick Accept

	UPROPERTY(meta = (BindWidget))
		UExpandableArea* ExpandMission;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MissionDescription;
	
	UPROPERTY(meta = (BindWidget))
		 UButton* AcceptButton;

		UPROPERTY(meta = (BindWidget))
		 UButton* TrackButton;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SubmitText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TrackText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UMissionUserWidget> mission;

	APCTerminal* PC;

	UDataObject* OwnedObject;

	UMissionLogSubsystem* QuestLog;

	UFUNCTION()
	void Dropdown();

	UFUNCTION()
		void SubmitClicked();

	UFUNCTION()	
		void MissionUpdated(FName MIssionID);
	UFUNCTION()	
	void TrackClicked();

	UFUNCTION()	
	void TrackTextCheck(FName MissionID);
	

	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};

/*
ChangeLog:
-----------------------------
Date:	 	Oct 17 2023
Name:	Tyler Brost
Summary:	Added some basic elements
-----------------------------
Date:	 	Oct 18 2023
Name:	Tyler Brost
Summary:	Started to add functionality with the mission implementation
-----------------------------
Date:	 	Nov 28 2023
Name:	Renz Leo Nicolas Dela Cruz
Summary:	- Setup for new Mission app layout
-----------------------------
Date		Feb 22 2024
Name:		Renz Leo Nicolas Dela Cruz
Summary:	- Made it so Track becomese Untrack when the mission the object is holding is the tracked mission
			- Fixed a bug where sometimes the accept button on the first mission would go missing
-----------------------------
*/
