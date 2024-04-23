// Copyright Astralwater Interactive. All Rights Reserved.


#include "Widgets/JournalListWidget.h"
#include "DataObject.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ProfileCardWidget.h"
#include "Creature.h"
#include "Subsystems/MissionSystem/MissionLogSubsystem.h"
#include "PCUserWidget.h"

void UJournalListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Bind the button
	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &UJournalListWidget::OnButtonClicked);
	}

	//Finds PC actor in world using get all actors of class
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APCTerminal::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		WorldPC = Cast<APCTerminal>(FoundActors[0]);
	}

}

void UJournalListWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{

	//Set the image
    DataObject = Cast<UDataObject>(ListItemObject);
	if (DataObject)
	{
		//Set text to # then append (ID)
		FishID->SetText(FText::FromString("#" + FString::FromInt(DataObject->JournalDataStruct.JournalID)));

		if (DataObject->JournalDataStruct.bIsCaptured) 
		{
			Icon->SetBrushFromTexture(DataObject->JournalDataStruct.JournalIcon);
			FishName->SetText(DataObject->JournalDataStruct.JournalName);
		}
		else
		{
			Icon->SetBrushFromTexture(DefaultIcon);
			FishName->SetText(FText::FromString("???"));
		}
	}
}

void UJournalListWidget::OnButtonClicked()
{
	//Creates a profile card widget and adds it to the viewport

	if (ProfileCardWidgetClass)
	{
		if (DataObject->JournalDataStruct.bIsCaptured == false)
		{
			UUserWidget* NotDiscoveredTextWidget = CreateWidget<UUserWidget>(GetWorld(), NotDiscoveredTextWidgetClass);
			if (NotDiscoveredTextWidget)
			{
				NotDiscoveredTextWidget->AddToViewport();
			}
			return;
		}
		UProfileCardWidget* ProfileCardWidget = CreateWidget<UProfileCardWidget>(GetWorld(), ProfileCardWidgetClass);
		if (ProfileCardWidget)
		{
			ProfileCardWidget->PopulateProfileCard(DataObject->JournalDataStruct.JournalID);
			ProfileCardWidget->AddToViewport();
			
			Cast<UPCUserWidget>(WorldPC->StoredPCWidget)->StoredProfileCard = ProfileCardWidget;

			UGameplayStatics::GetGameInstance(this)->GetSubsystem<UMissionLogSubsystem>()->CallObjectiveID("JournalListID"); //Nico Mod for missions
		}


	}

}
